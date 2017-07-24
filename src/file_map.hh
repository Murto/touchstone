#ifndef FILE_MAP_HPP
#define FILE_MAP_HPP

#include <sys/mman.h>

#include <cstring>
#include <stdexcept>
#include <string>

class file_map {
public:
	using value_type = char;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const reference;
	using pointer = value_type*;
	using const_pointer = const pointer;
	
	class iterator {
	public:
		using difference_type = size_type;
		using value_type = file_map::value_type;
		using pointer = file_map::pointer;
		using reference = file_map::reference;
		using iterator_category = std::random_access_iterator_tag;

		iterator(file_map& map, const size_type pos = 0);
		iterator& operator++() noexcept;
		iterator operator++(int) noexcept;
		reference operator[](const size_type pos) noexcept;
		reference operator*() noexcept;
		iterator operator+(const size_type rhs) const noexcept;
		friend iterator operator+(const size_type lhs, const iterator& rhs) noexcept;
		iterator operator-(const size_type rhs) const noexcept;
		iterator::difference_type operator-(const iterator& rhs) const noexcept;
		bool operator<(const iterator& rhs) const noexcept;
		bool operator>(const iterator& rhs) const noexcept;
		bool operator<=(const iterator& rhs) const noexcept;
		bool operator>=(const iterator& rhs) const noexcept;
		bool operator==(const iterator& rhs) const noexcept;
		bool operator!=(const iterator& rhs) const noexcept;
		iterator& operator+=(const size_type rhs) noexcept;
		iterator& operator-=(const size_type rhs) noexcept;

	private:
		file_map& map;
		size_type file_pos;
	};

	class const_iterator {
	public:
		using difference_type = size_type;
		using value_type = const file_map::value_type;
		using pointer = file_map::pointer;
		using reference = file_map::const_reference;
		using iterator_category = std::random_access_iterator_tag;

		const_iterator(const file_map& map, const size_type pos = 0);
		const_iterator& operator++() noexcept;
		const_iterator operator++(int) noexcept;
		reference operator[](const size_type pos) const noexcept;
		reference operator*() const noexcept;
		const_iterator operator+(const size_type rhs) const noexcept;
		friend const_iterator operator+(const size_type lhs, const const_iterator& rhs) noexcept;
		const_iterator operator-(const size_type rhs) const noexcept;
		const_iterator::difference_type operator-(const const_iterator& rhs) const noexcept;
		bool operator<(const const_iterator& rhs) const noexcept;
		bool operator>(const const_iterator& rhs) const noexcept;
		bool operator<=(const const_iterator& rhs) const noexcept;
		bool operator>=(const const_iterator& rhs) const noexcept;
		bool operator==(const const_iterator& rhs) const noexcept;
		bool operator!=(const const_iterator& rhs) const noexcept;
		const_iterator& operator+=(const size_type rhs) noexcept;
		const_iterator& operator-=(const size_type rhs) noexcept;

	private:
		const file_map& map;
		size_type file_pos;
	};

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	file_map(const char* file_path);
	file_map(const std::string& file_path);
	file_map(const file_map&) = delete;
	file_map(file_map&& other);
	~file_map();
	reference operator[](const size_type pos) noexcept;
	const_reference operator[](const size_type pos) const noexcept;
	file_map& operator=(file_map&& other) noexcept;
	reference at(const size_type pos);
	const_reference at(const size_type pos) const;
	reference front() noexcept;
	const_reference front() const noexcept;
	reference back() noexcept;
	const_reference back() const noexcept;
	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;
	bool empty() const noexcept;
	size_type size() const noexcept;

private:
	void remap() const noexcept;

	static const size_type page_size;
	int file_desc;
	size_type file_size;
	mutable size_type file_offset;
	mutable void* map_addr;
	size_type map_size;
};

#endif

// Public file_map member functions:

inline file_map::reference file_map::operator[](const size_type pos) noexcept {
	if (pos < file_offset || pos >= file_offset + page_size) {
		file_offset = (pos / page_size) * page_size;
		remap();
	}

	return static_cast<char*>(map_addr)[pos - file_offset];
}

inline file_map::const_reference file_map::operator[](const size_type pos) const noexcept {
	if (pos < file_offset || pos >= file_offset + page_size) {
		file_offset = (pos / page_size) * page_size;
		remap();
	}

	return static_cast<char*>(map_addr)[pos - file_offset];
}

inline file_map& file_map::operator=(file_map&& other) noexcept {
	file_desc = other.file_desc;
	file_size = other.file_size;
	file_offset = other.file_offset;
	map_addr = other.map_addr;
	map_size = other.map_size;
}

inline file_map::reference file_map::at(const size_type pos) {
	if (pos < 0 || pos >= file_size)
		return (*this)[pos];
	throw std::runtime_error("Outside of bounds!");
}

inline file_map::const_reference file_map::at(const size_type pos) const {
	if (pos < 0 || pos >= file_size)
		return (*this)[pos];
	throw std::runtime_error("Outside of bounds!");
}

inline file_map::reference file_map::front() noexcept {
	return (*this)[0];	
}

inline file_map::const_reference file_map::front() const noexcept {
	return (*this)[0];	
}

inline file_map::reference file_map::back() noexcept {
	return (*this)[file_size - 1];
}

inline file_map::const_reference file_map::back() const noexcept {
	return (*this)[file_size - 1];
}

inline file_map::iterator file_map::begin() noexcept {
	return iterator(*this);
}

inline file_map::iterator file_map::end() noexcept {
	return iterator(*this, file_size);
}

inline file_map::const_iterator file_map::cbegin() const noexcept {
	return const_iterator(*this);
}

inline file_map::const_iterator file_map::cend() const noexcept {
	return const_iterator(*this, file_size);
}

inline file_map::reverse_iterator file_map::rbegin() noexcept {
	return reverse_iterator(begin());
}

inline file_map::reverse_iterator file_map::rend() noexcept {
	return reverse_iterator(end());
}

inline file_map::const_reverse_iterator file_map::crbegin() const noexcept {
	return const_reverse_iterator(cbegin());
}

inline file_map::const_reverse_iterator file_map::crend() const noexcept {
	return const_reverse_iterator(cend());
}

inline bool file_map::empty() const noexcept {
	return !map_addr;
}

inline file_map::size_type file_map::size() const noexcept {
	return file_size;
}


// Private file_map member functions:

inline void file_map::remap() const noexcept {
	munmap(map_addr, page_size);
	map_addr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, file_desc, file_offset);
}


// Public file_map iterator member functions:

inline file_map::iterator& file_map::iterator::operator++() noexcept {
	++file_pos;
	return *this;
}

inline file_map::iterator file_map::iterator::operator++(int) noexcept {
	auto temp{*this};
	++*this;
	return temp;
}

inline file_map::iterator::reference file_map::iterator::operator[](const size_type pos) noexcept {
	return map[file_pos + pos];
}

inline file_map::iterator::reference file_map::iterator::operator*() noexcept {
	return map[file_pos];
}

inline file_map::iterator file_map::iterator::operator+(const size_type rhs) const noexcept {
	iterator temp{*this};
	return temp += rhs;
}

inline file_map::iterator operator+(const file_map::size_type lhs, const file_map::iterator& rhs) noexcept {
	return rhs + lhs;
}

inline file_map::iterator file_map::iterator::operator-(const size_type rhs) const noexcept {
	iterator temp{*this};
	return temp -= rhs;
}

inline file_map::iterator::difference_type file_map::iterator::operator-(const iterator& rhs) const noexcept {
	return file_pos - rhs.file_pos;
}

inline bool file_map::iterator::operator<(const iterator& rhs) const noexcept {
	return *this - rhs < 0;
}

inline bool file_map::iterator::operator>(const iterator& rhs) const noexcept {
	return rhs < *this;
}

inline bool file_map::iterator::operator<=(const iterator& rhs) const noexcept {
	return !(*this > rhs);
}

inline bool file_map::iterator::operator>=(const iterator& rhs) const noexcept {
	return rhs <= *this;
}

inline bool file_map::iterator::operator==(const iterator& rhs) const noexcept {
	return file_pos == rhs.file_pos;
}

inline bool file_map::iterator::operator!=(const iterator& rhs) const noexcept {
	return !(*this == rhs);
}

inline file_map::iterator& file_map::iterator::operator+=(const size_type rhs) noexcept {
	file_pos += rhs;
	return *this;
}

inline file_map::iterator& file_map::iterator::operator-=(const size_type rhs) noexcept {
	file_pos -= rhs;
	return *this;
}


// Public file_map const_iterator member functions:

inline file_map::const_iterator& file_map::const_iterator::operator++() noexcept {
	++file_pos;
	return *this;
}

inline file_map::const_iterator file_map::const_iterator::operator++(int) noexcept {
	auto temp{*this};
	++*this;
	return temp;
}

inline file_map::const_iterator::reference file_map::const_iterator::operator[](const size_type pos) const noexcept {
	return map[file_pos + pos];
}

inline file_map::const_iterator::reference file_map::const_iterator::operator*() const noexcept {
	return map[file_pos];
}

inline file_map::const_iterator file_map::const_iterator::operator+(const size_type rhs) const noexcept {
	const_iterator temp{*this};
	return temp += rhs;
}

inline file_map::const_iterator operator+(const file_map::size_type lhs, const file_map::const_iterator& rhs) noexcept {
	return rhs + lhs;
}

inline file_map::const_iterator file_map::const_iterator::operator-(const size_type rhs) const noexcept {
	const_iterator temp{*this};
	return temp -= rhs;
}

inline file_map::const_iterator::difference_type file_map::const_iterator::operator-(const const_iterator& rhs) const noexcept {
	return file_pos - rhs.file_pos;
}

inline bool file_map::const_iterator::operator<(const const_iterator& rhs) const noexcept {
	return *this - rhs < 0;
}

inline bool file_map::const_iterator::operator>(const const_iterator& rhs) const noexcept {
	return rhs < *this;
}

inline bool file_map::const_iterator::operator<=(const const_iterator& rhs) const noexcept {
	return !(*this > rhs);
}

inline bool file_map::const_iterator::operator>=(const const_iterator& rhs) const noexcept {
	return rhs <= *this;
}

inline bool file_map::const_iterator::operator==(const const_iterator& rhs) const noexcept {
	return file_pos == rhs.file_pos;
}

inline bool file_map::const_iterator::operator!=(const const_iterator& rhs) const noexcept {
	return !(*this == rhs);
}

inline file_map::const_iterator& file_map::const_iterator::operator+=(const size_type rhs) noexcept {
	file_pos += rhs;
	return *this;
}

inline file_map::const_iterator& file_map::const_iterator::operator-=(const size_type rhs) noexcept {
	file_pos -= rhs;
	return *this;
}
