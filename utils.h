#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string getExecutablePath();

bool createDirectory(const std::string& path);

#endif