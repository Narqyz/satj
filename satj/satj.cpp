// satj.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include "stdio.h"
#include "omp.h"
#include "string.h"
#include <string>
using namespace std;


#include "dataGen.h" // кездейсоқ матрица құратын класс
#include "matrixHelpers.h" // шешімді тексеру класс
#include "gaussSerial.h"  // Гаусс сызықты орындайтын коды
#include "gaussParallel.h" // Гаусс параллель орындайтын коды
#include "CGSerial.h" // CG сызықты орындайтын коды
#include "CGParallel.h" // CG параллель орындайтын коды
#include "csvExport.h" // *.csv файлмен жұмыс

int main() {
	/*
	* НОМЕР ЭКСРЕПИМЕНТА
	* 1 - диагональная матрица, результаты храниться в result1.csv
	* 2 - трехдиагональная матрица, результаты храниться в result2.csv
	* 3 - полная матрица, результаты храниться в result3.csv
	*/
	int experiment_number = 3; // номер эксперимента
	printf("Max threads count = %d", omp_get_max_threads());
	int threads_array[] = { 2, 4, 6, 8, 10, 12 }; // тут количество потоков
	int m = sizeof(threads_array) / sizeof(threads_array[0]);
	csvExport::write(threads_array, experiment_number, m);

	for (int mSize = 100; mSize <= 3000; mSize += 500) { // тут размер матриц 
		cout << "\n Matrix size = " << mSize;
		double** originalA, ** pMatrix; //Коэффицент матрицасы (екі өлшемді)
		double* originalB, * pVector; //Сызықтық жүйенің оң жағы
		double* pResult; //Нәтиже векторы
		string times = ""; //строка для csv

		originalB = new double[mSize];
		pVector = new double[mSize];
		pResult = new double[mSize];
		originalA = new double* [mSize];
		pMatrix = new double* [mSize];
		for (int i = 0; i < mSize; i++) {
			originalA[i] = new double[mSize];
			pMatrix[i] = new double[mSize];
		}
		if (experiment_number == 1) dataGen::DiagonalDataInitialization(originalA, originalB, mSize);		//Деректерді генерациялау, pMatrix пен pVector кездейсоқ сандармен толтыру
		else if (experiment_number == 2)  dataGen::ThreeDiagonalDataInitialization(originalA, originalB, mSize);		//Деректерді генерациялау, pMatrix пен pVector кездейсоқ сандармен толтыру
		else dataGen::randomDataInitialization(originalA, originalB, mSize);		//Деректерді генерациялау, pMatrix пен pVector кездейсоқ сандармен толтыру

		//Объекты бәрін құрастырамыз
		gaussSerial* gaussSerialSolver;
		gaussParallel* gaussParallelSolver;
		CGSerial* CGSerialSolver;
		CGParallel* CGParallelSolver;

		matrixHelpers::setDefault(originalA, originalB, mSize, pMatrix, pVector, pResult);
		double startTime = omp_get_wtime(); //запускаем таймер
		gaussSerialSolver = new gaussSerial(mSize);  	// Гаусс сызықты алгоритмі
		gaussSerialSolver->resultCalculation(pMatrix, pVector, pResult); // вызываем метод объекта - получаем услугу у друга
		double finishTime = omp_get_wtime();  // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время
		printf("\nTime: %f second, method: %s, pResult[0]= %f", finishTime - startTime, "Gauss serial", pResult[0]);
		times += to_string(finishTime - startTime); //для сохранения в файле
		matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);	//Нәтижені тексеру
		// matrixHelpers::printVector(pResult, mSize);
		for (int k = 0; k < m; k++) {
			matrixHelpers::setDefault(originalA, originalB, mSize, pMatrix, pVector, pResult);
			startTime = omp_get_wtime(); //запускаем таймер
			gaussParallelSolver = new gaussParallel(mSize, threads_array[k]);// создаем объект - добавляем в друзья
			gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult); // Гаусс параллель алгоритмі threads[i] поток
			finishTime = omp_get_wtime(); // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время 
			printf("\nTime: %f second, method: %s, threads count: %d, pResult[0]= %f", finishTime - startTime, "Gauss parallel", threads_array[k], pResult[0]);
			times += ";" + to_string(finishTime - startTime); //для сохранения в файле
			matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);//Нәтижені тексеру
		}

		matrixHelpers::setDefault(originalA, originalB, mSize, pMatrix, pVector, pResult);
		if (!matrixHelpers::checkSymmetrical(pMatrix, mSize)) {
			printf("\n Matrix is not symmetrical");
			matrixHelpers::printMatrix(pMatrix, mSize);
		} else if(!matrixHelpers::checkPositiveDefinite(pMatrix, mSize)) {
			printf("\n Matrix is not Positive Definite");
			matrixHelpers::printMatrix(pMatrix, mSize);
		} else {
			startTime = omp_get_wtime(); //запускаем таймер
			CGSerialSolver = new CGSerial();  // CG сызықты алгоритмі
			CGSerialSolver->resultCalculation(pMatrix, pVector, pResult, mSize);  
			finishTime = omp_get_wtime(); // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время 
			printf("\nTime: %f second, method: %s, pResult[0]= %f", finishTime - startTime, "CG serial ", pResult[0]);
			times += ";" + to_string(finishTime - startTime); //для сохранения в файле
			printf(" iteration sani: %d", CGSerialSolver->iterationsCount); //CG әдісінің бірі болса, онда қайталанулар санын көрсетеміз
			matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);//Нәтижені тексеру
			for (int k = 0; k < m; k++) {

				matrixHelpers::setDefault(originalA, originalB, mSize, pMatrix, pVector, pResult);
				startTime = omp_get_wtime(); //запускаем таймер
				CGParallelSolver = new CGParallel();  // CG параллель алгоритмі threads[i] поток
				CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads_array[k]);
				finishTime = omp_get_wtime(); // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время
				printf("\nTime: %f second, method: %s, threads count: %d, pResult[0]= %f", finishTime - startTime, "CG parallel", threads_array[k], pResult[0]);
				times += ";" + to_string(finishTime - startTime); //для сохранения в файле
				printf(", iteration sani: %d", CGParallelSolver->iterationsCount); //CG әдісінің бірі болса, онда қайталанулар санын көрсетеміз
				matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);//Нәтижені тексеру
			}
		}
		
		csvExport::replaceAll(times,".", ",");
		const char* ti = times.c_str(); // меняем стринг на чар, точку на запятую
		csvExport::addTimes(mSize, experiment_number, ti);
		if (mSize == 100) mSize = 0;
	}

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
