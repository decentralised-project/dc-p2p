#include "p2p_host.hpp"

namespace dcp2p
{
	p2p_host::p2p_host()
	{

	}

	p2p_host::~p2p_host()
	{

	}

	std::vector<p2p_host> p2p_host::LoadAll(std::string filePath,
		std::vector<std::string> dnsSeeds,
		int defaultPort)
	{
		std::vector<p2p_host> hosts;

		// TODO: file

		for (std::vector<std::string>::iterator it = dnsSeeds.begin(); it != dnsSeeds.end(); ++it) {

			boost::asio::io_service io_service;
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(it->c_str(), "");
			for (boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
				i != boost::asio::ip::tcp::resolver::iterator();
				++i)
			{
				p2p_host host;

				boost::asio::ip::tcp::endpoint end = *i;
				host.Ip = end.address().to_string();
				host.Port = defaultPort;

				hosts.push_back(host);
			}
		}

		return hosts;
	}
}