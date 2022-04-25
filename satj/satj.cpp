// satj.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include "stdio.h"
#include "omp.h"
#include "string.h"

#include "dataGen.h" // кездейсоқ матрица құратын класс
#include "matrixHelpers.h" // шешімді тексеру класс
#include "gaussSerial.h"  // Гаусс сызықты орындайтын коды
#include "gaussParallel.h" // Гаусс параллель орындайтын коды
#include "CGSerial.h" // CG сызықты орындайтын коды
#include "CGParallel.h" // CG параллель орындайтын коды
#include "csvExport.h" // *.csv файлмен жұмыс
#include <string>
using namespace std;


int main() {

	printf("Max threads count = %d", omp_get_max_threads());
	int threads_array[] = { 2, 4, 8, 12 }; // тут количество потоков
	int m = sizeof(threads_array) / sizeof(threads_array[0]);
	csvExport::write(threads_array, m);

	for (int mSize = 1000; mSize < 5000; mSize += 1000) { // тут размер матриц 
		cout << "\n Matrix size = " << mSize;
		double** originalA, ** pMatrix; //Коэффицент матрицасы (екі өлшемді)
		double* originalB, * pVector; //Сызықтық жүйенің оң жағы
		double* pResult; //Нәтиже векторы
		string times = "";

		originalB = new double[mSize];
		pVector = new double[mSize];
		pResult = new double[mSize];
		originalA = new double* [mSize];
		pMatrix = new double* [mSize];
		for (int i = 0; i < mSize; i++) {
			originalA[i] = new double[mSize];
			pMatrix[i] = new double[mSize];
		}

		//Деректерді генерациялау, pMatrix пен pVector кездейсоқ сандармен толтыру
		dataGen::randomDataInitialization(originalA, originalB, mSize);

		//Объекты бәрін құрастырамыз
		gaussSerial* gaussSerialSolver;
		gaussParallel* gaussParallelSolver;
		CGSerial* CGSerialSolver;
		CGParallel* CGParallelSolver;

		for (int i = 0; i < mSize; i++) {
			for (int j = 0; j < mSize; j++)
				pMatrix[i][j] = pMatrix[j][i] = originalA[i][j];
			pVector[i] = originalB[i];
		}

		double startTime = omp_get_wtime(); //запускаем таймер
		// Гаусс сызықты алгоритмі
		gaussSerialSolver = new gaussSerial(mSize);
		gaussSerialSolver->resultCalculation(pMatrix, pVector, pResult);
		double finishTime = omp_get_wtime();  // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время
		printf("\nTime: %f second, method: %s, pResult[0]= %f", finishTime - startTime, "Gauss serial", pResult[0]);
		times += to_string(finishTime - startTime);
		matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);	//Нәтижені тексеру

		for (int i = 0; i < m; i++) {
			for (int i = 0; i < mSize; i++) {
				for (int j = 0; j < mSize; j++)
					pMatrix[i][j] = pMatrix[j][i] = originalA[i][j];
				pVector[i] = originalB[i];
			}
			pResult = new double[mSize];// Обнуляем вектор решений 
			startTime = omp_get_wtime(); //запускаем таймер
			// Гаусс параллель алгоритмі threads[i] поток
			gaussParallelSolver = new gaussParallel(mSize);// создаем объект - добавляем в друзья
			gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult, threads_array[i]); // вызываем метод объекта - получаем услугу у друга	
			finishTime = omp_get_wtime(); // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время 
			printf("\nTime: %f second, method: %s, threads count: %d, pResult[0]= %f", finishTime - startTime, "Gauss parallel", threads_array[i], pResult[0]);
			times += ";";
			times += to_string(finishTime - startTime);
			matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);//Нәтижені тексеру

		}
		for (int i = 0; i < mSize; i++) {
			for (int j = 0; j < mSize; j++)
				pMatrix[i][j] = pMatrix[j][i] = originalA[i][j];
			pVector[i] = originalB[i];
		}
		pResult = new double[mSize];// Обнуляем вектор решений 
		if (!matrixHelpers::checkSymmetrical(pMatrix, mSize)) {
			printf("\n Matrix is not symmetrical");
			matrixHelpers::printMatrix(originalA, mSize);
		}
		else {
			startTime = omp_get_wtime(); //запускаем таймер
			// CG сызықты алгоритмі
			CGSerialSolver = new CGSerial();
			CGSerialSolver->resultCalculation(pMatrix, pVector, pResult, mSize);
			finishTime = omp_get_wtime(); // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время 
			printf("\nTime: %f second, method: %s, pResult[0]= %f", finishTime - startTime, "CG serial ", pResult[0]);
			times += ";";
			times += to_string(finishTime - startTime);
			printf(" iteration sani: %d", CGSerialSolver->iterationsCount); //CG әдісінің бірі болса, онда қайталанулар санын көрсетеміз
			matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);//Нәтижені тексеру
			for (int i = 0; i < m; i++) {
				for (int i = 0; i < mSize; i++) {
					for (int j = 0; j < mSize; j++)
						pMatrix[i][j] = pMatrix[j][i] = originalA[i][j];
					pVector[i] = originalB[i];
				}
				pResult = new double[mSize];// Обнуляем вектор решений 
				startTime = omp_get_wtime(); //запускаем таймер
				// CG параллель алгоритмі threads[i] поток
				CGParallelSolver = new CGParallel();
				CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads_array[i]);
				finishTime = omp_get_wtime(); // останавливаем таймер, от полученного времени убавляем время старта, чтобы получить потраченнное время
				printf("\nTime: %f second, method: %s, threads count: %d, pResult[0]= %f", finishTime - startTime, "CG parallel", threads_array[i], pResult[0]);
				times += ";";
				times += to_string(finishTime - startTime);
				printf(", iteration sani: %d", CGParallelSolver->iterationsCount); //CG әдісінің бірі болса, онда қайталанулар санын көрсетеміз
				matrixHelpers::testSolvingResult(originalA, originalB, pResult, mSize);//Нәтижені тексеру
			}
		}
		const char* ti = times.c_str();
		csvExport::addTimes(mSize, ti);
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
