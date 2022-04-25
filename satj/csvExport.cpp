#include "csvExport.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
using namespace std;

int csvExport::write(int *threads_array, int size) { // метод записи результатов в файл
    ofstream myfile;
    myfile.open("result.csv");
    myfile << "Размер;Гаусс линейно;";
    for (int i = 0; i < size; i++) {
        myfile << "Гаусс параллель(" << threads_array[i] << ");";
    }
    myfile << "CG линейно;";
    for (int i = 0; i < size; i++) {
        myfile << "CG параллель(" << threads_array[i] << ");";
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
