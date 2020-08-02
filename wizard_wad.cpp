#include "wizard_wad.h"

void inflate_buf(const int zsize, byte* wad_in, const int size, byte* data)
{
	z_stream infstream;
	infstream.zalloc = nullptr;
	infstream.zfree = nullptr;
	infstream.opaque = nullptr;
	infstream.avail_in = zsize;
	infstream.next_in = wad_in;
	infstream.avail_out = size;
	infstream.next_out = data;
	inflateInit(&infstream);
	inflate(&infstream, Z_NO_FLUSH);
	inflateEnd(&infstream);
}

std::vector<file_dat> get_wad(const char* filter, const char* type, bool vector)
{
	std::vector<file_dat> file_map;
	
	std::ifstream in("C:/ProgramData/KingsIsle Entertainment/Wizard101/Data/GameData/Root.wad", std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);
	auto file_size = in.tellg();
	in.seekg(0, std::ios::beg);
	
	std::vector<byte> file_data(file_size);
	in.read(reinterpret_cast<char*>(&file_data[0]), file_size);
	
	auto* wad_in = file_data.data();
	auto* wad_back = file_data.data();
	auto* header = reinterpret_cast<wad_header*>(wad_in);
	wad_in += sizeof(wad_header);
	
#ifdef _DEBUG
	printf("Header: %s | Version: %x | Files: %x | Padding: %x (header size %x)\n", header->kiwad, header->version, header->files, header->padding, sizeof(wad_header));
	printf("Number of files: %x\n", header->files);
#endif
	
	for (auto i = 0; i < header->files; ++i) {
		auto* file = reinterpret_cast<wad_file*>(wad_in);
		wad_in += sizeof(wad_file);

		auto* file_name = reinterpret_cast<char*>(malloc(file->namesz));

		if (!file_name && file_name == nullptr) {
			return {};
		}
		
		memcpy(file_name, wad_in, file->namesz);
		
		wad_in += file->namesz;

		if (strstr(file_name, type) == nullptr)
			continue;
		
		if (strstr(file_name, filter) == nullptr)
			continue;

		auto tmp = wad_in;
		wad_in = &wad_back[0];
		wad_in += file->offset;

		if (*wad_in == 0) {
			continue;
		}
		std::vector<byte> output(file->size);
		if (file->zip == 0) {
			memcpy(output.data(), wad_in, file->size);
			
#ifdef _DEBUG
			printf("[non-zip] %s %x %x\n", file_name, file->offset, file->size);
#endif
			
		}
		else {
			inflate_buf(file->zsize, wad_in, file->size, output.data());
			
#ifdef _DEBUG
			printf("[zip] %s %x %x %x\n", file_name, file->offset, file->zsize, file->size);
#endif
			
		}
		if (vector) {
			file_map.push_back(file_dat{ std::string(file_name), std::vector<byte>(output.begin(), output.end()) }); // string was needed to create a copy of fileName.data() , really annoying
		}
		else {
			std::string direc(file_name);
			auto sub = direc.find_last_of('/');
			if (sub != std::string::npos) {
				CreateDirectoryA(direc.substr(0, sub).c_str(), nullptr);
			}
			std::ofstream fout;
			fout.open(file_name, std::ios::binary | std::ios::out);
			fout.write(reinterpret_cast<char*>(output.data()), output.size());
			fout.close();
		}
#ifdef _DEBUG
		printf("[%x] Offset: %x | Size: %x | ZSize: %x | Zip?: %x | Crc: %x | NameSz: %x | Name: %s\n", i, file->offset, file->size, file->zsize, file->zip, file->crc, file->namesz, file_name);
#endif
		
		free(file_name);
		wad_in = tmp;
	}
	return file_map;
}