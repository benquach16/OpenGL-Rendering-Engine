#pragma once

#include <fstream>
#include <iostream>
#include <string>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace util {
std::string loadFile(const std::string& path);
};
