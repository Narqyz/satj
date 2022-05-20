#include "csvExport.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
using namespace std;

int csvExport::write(int *threads_array, int experiment_number, int size) { // метод записи результатов в файл
    ofstream myfile;
    string filename = "result";
    filename += to_string(experiment_number);
    filename +=".csv";
    cout << '\n' << filename;
    myfile.open(filename);
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

int csvExport::addTimes(int Size, int experiment_number, const char* times)
{
    ofstream myfile;
    string filename = "result";
    filename += to_string(experiment_number);
    filename += ".csv";
    myfile.open(filename,ios::app); // файлды ашып, соңына алып барады(так, чтобы предыдущие не стерлись)
    myfile << "\n" <<Size << "x" << Size << ";" << times;
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
