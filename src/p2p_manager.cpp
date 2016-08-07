#include "p2p_manager.hpp"

namespace dcp2p
{
	p2p_manager::p2p_manager()
	{

	}

	p2p_manager::~p2p_manager()
	{
		delete _listener;

		for (size_t i = 0; i < _threads.size(); i++)
		{
			_threads[i]->interrupt();
			_threads[i]->join();
			delete _threads[i];
		}
	}

	void p2p_manager::Run(int incomingPort)
	{
		_networkId = boost::uuids::random_generator()();

		std::stringstream ss;
		ss << "Network Id for this session is " << _networkId;
		Log(ss.str());

		// first, start the listener thread
		boost::thread* listenerThread = new boost::thread(&p2p_manager::listener_run, this, incomingPort);
		_threads.push_back(listenerThread);

		// create a vector of dns seeds for the host manager
		const std::string arr[] = { "dnsseed.uape.co.uk" };
		std::vector<std::string> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

		p2p_host hostManager;
		std::vector<p2p_host> hosts;

		try
		{
			// 6453 is the default port
			hosts = hostManager.LoadAll("hosts", vec, 6453);
		}
		catch (std::exception const &ex) {
			Log(ex.what());
		}
	
		std::string msg("Found ");

		std::stringstream hostsLengthStr;
		hostsLengthStr << hosts.size();
		msg.append(hostsLengthStr.str());

		msg.append(" known peers from hosts file");
		Log(msg);

		boost::thread* workerA = new boost::thread(&p2p_manager::outgoing_run, this, hosts);
		_threads.push_back(workerA);
	}

	void p2p_manager::listener_run(int incomingPort)
	{
		boost::asio::io_service io;
		_listener = new p2p_listener(io, incomingPort, _networkId);
		_listener->ListenForIncoming(this);

		std::string msg = "Listening on port ";

		std::stringstream ss;
		ss << incomingPort;
		msg.append(ss.str());

		Log(msg);

		io.run();
	}

	void p2p_manager::outgoing_run(std::vector<p2p_host> hosts)
	{
		boost::asio::io_service io;

		int chosenIndex = rand() % hosts.size();

		std::string msg;
		msg.append("Connecting to ");
		msg.append(hosts[chosenIndex].Ip);
		msg.append(":");

		std::stringstream ss;
		ss << hosts[chosenIndex].Port;
		msg.append(ss.str());

		Log(msg);

		// outgoing connection
		p2p_connection::pointer new_connection = p2p_connection::Create(io, _networkId);
		new_connection->Log.connect(boost::bind(&p2p_manager::on_log_recieved, this, _1));
		new_connection->NodeConnected.connect(boost::bind(&p2p_manager::on_node_connected, this, _1, _2, _3));
		new_connection->ReceivedData.connect(boost::bind(&p2p_manager::on_data_recieved, this, _1, _2));
		new_connection->NodeDisconnected.connect(boost::bind(&p2p_manager::on_node_disconnected, this, _1));
		new_connection->Connect(hosts[chosenIndex].Ip, hosts[chosenIndex].Port);

		io.run();
	}

	void p2p_manager::on_node_connected(bool isIncoming, p2p_connection::pointer connection, boost::uuids::uuid remoteId)
	{
		NodeConnected(isIncoming, connection, remoteId);
	}

	void p2p_manager::on_log_recieved(std::string msg)
	{
		Log(msg);
	}

	void p2p_manager::on_data_recieved(p2p_connection::pointer connection, p2p_packet packet)
	{
		DataReceived(connection, packet);
	}

	void p2p_manager::on_node_disconnected(boost::uuids::uuid remoteId)
	{
		NodeDisconnected(remoteId);
	}
}