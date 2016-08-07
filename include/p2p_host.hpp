#ifndef __P2P_HOST_H_INCLUDED__
#define __P2P_HOST_H_INCLUDED__

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <iostream>

namespace dcp2p
{
	class p2p_host
	{
	public:
		std::string Ip;
		int Port;

		p2p_host();
		~p2p_host();

		std::vector<p2p_host> LoadAll(std::string filePath, 
									  std::vector<std::string> dnsSeeds, 
									  int defaultPort);

	};
}

#endif