#include "p2p_listener.hpp"
#include "p2p_manager.hpp"

namespace dcp2p
{
	p2p_listener::p2p_listener(boost::asio::io_service &io_service, int incomingPort, boost::uuids::uuid &localId)
		: _io_service(io_service), acceptor_(_io_service, tcp::endpoint(tcp::v4(), incomingPort)), _localId(localId)
	{
	}

	p2p_listener::~p2p_listener() 		
	{
		_io_service.stop();
	}

	void p2p_listener::ListenForIncoming(p2p_manager* manager)
	{
		manager_ = manager;

		p2p_connection::pointer new_connection = p2p_connection::Create(_io_service, _localId);
		new_connection->NodeConnected.connect(boost::bind(&p2p_manager::on_node_connected, manager_, _1, _2, _3));
		new_connection->Log.connect(boost::bind(&p2p_manager::on_log_recieved, manager_, _1));
		new_connection->ReceivedData.connect(boost::bind(&p2p_manager::on_data_recieved, manager_, _1, _2));
		new_connection->NodeDisconnected.connect(boost::bind(&p2p_manager::on_node_disconnected, manager_, _1));

		acceptor_.async_accept(new_connection->Socket(),
			boost::bind(&p2p_listener::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
	}

	void p2p_listener::handle_accept(p2p_connection::pointer new_connection, const boost::system::error_code& error)
	{
		if (!error)
		{
			new_connection->Start();
			ListenForIncoming(manager_);
		}
	}
}