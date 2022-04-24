#include "csvExport.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
using namespace std;

int csvExport::writeGauss(int Size, const char *times) {
    std::ofstream myfile;
    myfile.open("result.csv");
    myfile << "Размер;Гаусс линейно;Гаусс параллель(1);Гаусс параллель(2);Гаусс параллель(4);Гаусс параллель(8);Гаусс параллель(12);";
    myfile << "CG линейно;CG параллель(1);CG параллель(2);CG параллель(4);CG параллель(8);CG параллель(12);\n";
    myfile << Size << ";" << times << "\n";
    myfile.close();
    return 0;
}
