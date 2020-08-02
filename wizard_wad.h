#pragma once
#include <Windows.h>
#include <fstream>
#include <vector>
#include <ostream>
#include "zlib/zlib.h"

#pragma pack(push, 1)
struct wad_header {
	char kiwad[5]; // "KIWAD"
	int version;
	int files;
	byte padding;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct wad_file {
	int offset;
	int size;
	int zsize;
	byte zip;
	int crc;
	int namesz;
};
#pragma pack(pop)

struct file_dat {
	std::string name;
	std::vector<byte> file;
};

void inflate_buf(int zsize, byte* wad_in, int size, byte* data);

std::vector<file_dat> get_wad(const char* filter, const char* type, bool vector);