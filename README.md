# wizard101-wad-reader
Reads wads from the game Wizard101, and reads the packet protocols embedded within the wad files.

```c
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
```
iterates current protocols in the wad files, and outputs them as shown.

to get a vector of wad files, do
```c
const auto files = get_wad("filter", "extension-type", true);
```

to write files from wad to disk, do
```c
get_wad("filter", "extension-type", false);
```
