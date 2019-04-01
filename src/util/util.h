#pragma once

#include <string>
#include <iostream>
#include <fstream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace util
{
std::string loadFile(const std::string &path);
};
