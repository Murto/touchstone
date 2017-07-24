#include "file_map.hh"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cerrno>

// Private file_map static member variables:

const file_map::size_type file_map::page_size{static_cast<size_type>(sysconf(_SC_PAGE_SIZE))};


// Public file_map member functions:

file_map::file_map(const char* file_path) : file_offset{0} {
	if ((file_desc = open(file_path, O_RDWR)) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}

	struct stat file_info;
	if (fstat(file_desc, &file_info) == -1) {
		close(file_desc);
		throw std::runtime_error{std::strerror(errno)};
	}

	if ((file_size = file_info.st_size) > 0) {
		if ((map_addr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, file_desc, file_offset)) == MAP_FAILED)
			throw std::runtime_error{std::strerror(errno)};
	} else {
		map_addr = nullptr;
	}
}

file_map::file_map(const std::string& file_path) : file_map{file_path.c_str()} {}

file_map::file_map(file_map&& other) : file_desc{other.file_desc}, file_size{other.file_size}, file_offset{other.file_offset}, map_addr{other.map_addr}, map_size{other.map_size} {}

file_map::~file_map() {
	if (map_addr) {
		munmap(map_addr, page_size);
		map_addr = nullptr;
	}

	close(file_desc);
}

// Public file_map iterator member functions:

file_map::iterator::iterator(file_map& map, const size_type pos) : map{map}, file_pos{pos} {}

// Public file_map const_iterator member functions:

file_map::const_iterator::const_iterator(const file_map& map, const size_type pos) : map{map}, file_pos{pos} {}
