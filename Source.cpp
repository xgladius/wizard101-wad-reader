#include <Windows.h>
#include <iostream>
#include "wizard_xml.h"	

int main() {
	//get_wad("", "", true);
	for (const auto& p : get_protocols()) {
		printf("ID: %03d | Type: %s | Version: %d | Description: %s\n", p.service_id, p.protocol_type.c_str(), p.protocol_version, p.protocol_description.c_str());
		auto i = 1;
		for (auto& msg : p.messages) {
			printf("	[%x] %s: %s", i, msg.msg_name.c_str(), msg.msg_description.c_str());
			for (const auto& arg : msg.params) {
				printf("\n		%s %s ", arg.type.c_str(), arg.name.c_str());
			}
			printf("\n");
			i++;
		}
	}
}