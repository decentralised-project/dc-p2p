#include "p2p_packet.hpp"

namespace dcp2p
{
	p2p_packet::p2p_packet()
	{

	}

	p2p_packet::~p2p_packet()
	{

	}

	char* p2p_packet::data()
	{
		return data_;
	}

	size_t p2p_packet::length() const
	{
		return header_length + body_length_;
	}

	const char* p2p_packet::body() const
	{
		return data_ + header_length;
	}

	char* p2p_packet::body()
	{
		return data_ + header_length;
	}

	size_t p2p_packet::body_length() const
	{
		return body_length_;
	}

	void p2p_packet::body_length(size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool p2p_packet::decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void p2p_packet::encode_header()
	{
		using namespace std; // For sprintf and memcpy.
		char header[header_length + 1] = "";
		sprintf(header, "%4d", body_length_);
		memcpy(data_, header, header_length);
	}
}