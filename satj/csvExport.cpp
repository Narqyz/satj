#include "csvExport.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
using namespace std;

int csvExport::write(int *threads_array, int size) { // ����� ������ ����������� � ����
    ofstream myfile;
    myfile.open("result.csv");
    myfile << "������;����� �������;";
    for (int i = 0; i < size; i++) {
        myfile << "����� ���������(" << threads_array[i] << ");";
    }
    myfile << "CG �������;";
    for (int i = 0; i < size; i++) {
        myfile << "CG ���������(" << threads_array[i] << ");";
    }
    myfile.close();
    return 0;
}

int csvExport::addTimes(int Size, const char* times)
{
    ofstream myfile;
    myfile.open("result.csv",ios::app);
    myfile << "\n" <<Size << ";" << times ;
    myfile.close();
    return 0;
}
void csvExport::replaceAll(std::string& str, const std::string& from, const std::string& to){
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
