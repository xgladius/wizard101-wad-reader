#include "wizard_xml.h"

std::vector<protocol_info> get_protocols() {
	std::vector<protocol_info> protocols;
	auto file_map = get_wad("Messages.xml", ".xml", true);
	for (auto x : file_map) {
		x.file.push_back(0x00);
		xml_document<> doc;
		doc.parse<0>(reinterpret_cast<char*>(x.file.data()));
		auto* pinfo_val = doc.first_node()->first_node("_ProtocolInfo")->first_node("RECORD");

		std::vector<xml_message> messages;
		for (auto* message = doc.first_node()->first_node("_ProtocolInfo")->next_sibling(); message; message = message->next_sibling()) {
			xml_message msg{ std::string(message->first_node("RECORD")->first_node("_MsgName")->value()),
							  std::string(message->first_node("RECORD")->first_node("_MsgDescription")->value()),
							  std::string(message->first_node("RECORD")->first_node("_MsgHandler")->value()),
							  std::vector<attribute>{}
			};
			for (auto* message_info = message->first_node("RECORD")->first_node("_MsgHandler")->next_sibling(); message_info; message_info = message_info->next_sibling())
			{
				std::string type;
				for (auto* attr = message_info->first_attribute("TYPE"); attr; attr = attr->next_attribute())
				{
					if (strstr(attr->name(), "TYPE") != nullptr)
						type = std::string(attr->value());
				}
				msg.params.push_back({ message_info->name(), type });
			}
			messages.push_back(msg);
		}

		protocol_info pi = { static_cast<byte>(atoi(pinfo_val->first_node("ServiceID")->value())), std::string(pinfo_val->first_node("ProtocolType")->value()),
							 atoi(pinfo_val->first_node("ProtocolVersion")->value()),  std::string(pinfo_val->first_node("ProtocolDescription")->value()),
							 messages };
		protocols.push_back(pi);
	}
	return protocols;
}