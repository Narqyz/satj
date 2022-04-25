#pragma once
#include <string>
class csvExport{
public:
	static int write(int *threads_array, int size);
	static int addTimes(int Size, const char *times);
	static void replaceAll(std::string& str, const std::string& from, const std::string& to);
};

