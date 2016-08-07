#ifndef __P2P_CONNECTION_H_INCLUDED__
#define __P2P_CONNECTION_H_INCLUDED__

#include <iostream>
#include <string>
#include <deque>
#include <boost/bind.hpp>
#include <boost/thread.hpp> 
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>        
#include "p2p_packet.hpp"

using namespace boost::asio::ip;

namespace dcp2p
{
	typedef std::deque<p2p_packet> packet_queue;

	class p2p_connection
		: public boost::enable_shared_from_this<p2p_connection>
	{
	public:
		typedef boost::shared_ptr<p2p_connection> pointer;

		static pointer Create(boost::asio::io_service& io_service, boost::uuids::uuid &localId)
		{
			return pointer(new p2p_connection(io_service, localId));
		}

		boost::signals2::signal<void(std::string)>											Log;
		boost::signals2::signal<void(bool, p2p_connection::pointer, boost::uuids::uuid)>	NodeConnected;
		boost::signals2::signal<void(p2p_connection::pointer, p2p_packet)>					ReceivedData;
		boost::signals2::signal<void(boost::uuids::uuid)>									NodeDisconnected;
		tcp::socket& Socket();

		void Start();
		void Connect(std::string host, int port);

		void Send(p2p_packet packet);
		void Send(char* data, size_t length);
		void Send(std::string message);

	private:

		p2p_connection(boost::asio::io_service& io_service, boost::uuids::uuid &localId);

		// callback declarations
		void handle_read_header(const boost::system::error_code& error);
		void handle_read_body(const boost::system::error_code& error);
		void handle_write(const boost::system::error_code& error);

		tcp::socket socket_;
		boost::asio::io_service &_io_service;
		p2p_packet packet_;
		packet_queue write_queue_;
		boost::uuids::uuid &_localId;
		boost::uuids::uuid _remoteId;
	};
}

#endif