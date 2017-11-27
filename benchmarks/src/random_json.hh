#ifndef RANDOM_JSON_HH
#define RANDOM_JSON_HH

#include <string>

void create_json(const std::string& path = "random.json", unsigned long records = 1);
void create_json(const char* path = "random.json", unsigned long records = 1);

#endif
