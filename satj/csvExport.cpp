#include "csvExport.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
using namespace std;

int csvExport::writeGauss(int Size, const char *times) {
    std::ofstream myfile;
    myfile.open("result.csv");
    myfile << "������;����� �������;����� ���������(1);����� ���������(2);����� ���������(4);����� ���������(8);����� ���������(12);";
    myfile << "CG �������;CG ���������(1);CG ���������(2);CG ���������(4);CG ���������(8);CG ���������(12);\n";
    myfile << Size << ";" << times << "\n";
    myfile.close();
    return 0;
}
