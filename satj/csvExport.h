#pragma once
#include <string>
class csvExport{
public:
	static int write(int *threads_array, int experiment_number, int size);
	static int addTimes(int Size, int experiment_number, const char *times);
	static void replaceAll(std::string& str, const std::string& from, const std::string& to);
};

