#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "wizard_wad.h"
#include "rapidxml.hpp"

using namespace rapidxml;

struct attribute {
	std::string name;
	std::string type;
};

struct xml_message {
	std::string msg_name;
	std::string msg_description;
	std::string msg_handler;
	std::vector<attribute> params;
};

struct protocol_info {
	byte service_id;
	std::string protocol_type;
	int protocol_version;
	std::string protocol_description;
	std::vector<xml_message> messages;
};

std::vector<protocol_info> get_protocols();