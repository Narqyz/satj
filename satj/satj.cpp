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


int main(){
	//Матрица өлшемі
	int mSize;
	printf("Enter matrix size \n");
	std::cin >> mSize;
	int threads[4] = {2, 4, 8, 16};

	double **pMatrix; //Коэффицент матрицасы (екі өлшемді)
	double *pVector; //Сызықтық жүйенің оң жағы
	double *pResult; //Нәтиже векторы

	pVector = new double[mSize];
	pResult = new double[mSize];
	pMatrix = new double *[mSize];
	for (int i = 0; i < mSize; i++) {
		pMatrix[i] = new double[mSize];
	}

	//Деректерді генерациялау, pMatrix пен pVector кездейсоқ сандармен толтыру
	dataGen::randomDataInitialization(pMatrix, pVector, mSize);

	//Объекты бәрін құрастырамыз
	gaussSerial* gaussSerialSolver;
	gaussParallel* gaussParallelSolver;
	CGSerial* CGSerialSolver;
	CGParallel* CGParallelSolver;

	// Гаусс сызықты алгоритмі
	//Уақытты өлшеу
	double startTime = omp_get_wtime();
	gaussSerialSolver = new gaussSerial(mSize);
	gaussSerialSolver->resultCalculation(pMatrix, pVector, pResult);
	//Жұмсалған уақыт
	double finishTime = omp_get_wtime();
	printf("\nJumsalgan uakit: %f second, method: %s", finishTime - startTime, "Gauss serial");
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	Sleep(600);

	for (int i = 0; i < 4; i++){
		// Гаусс параллель алгоритмі threads[i] поток
		//Уақытты өлшеу
		startTime = omp_get_wtime();
		gaussParallelSolver = new gaussParallel(mSize);
		gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult, threads[i]);
		//Жұмсалған уақыт
		finishTime = omp_get_wtime();
		printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "Gauss parallel", threads[i]);
		//Нәтижені тексеру
		matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
		Sleep(600);
	}
	

	// CG сызықты алгоритмі
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	CGSerialSolver = new CGSerial();
	CGSerialSolver->resultCalculation(pMatrix, pVector, pResult, mSize);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	printf("\nJumsalgan uakit: %f second, method: %s", finishTime - startTime, "CG serial ");
	//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
	printf("iteration sani: %d", CGSerialSolver->iterationsCount);
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	Sleep(600);

	for (int i = 0; i < 4; i++) {
		// CG параллель алгоритмі threads[i] поток
		//Уақытты өлшеу
		startTime = omp_get_wtime();
		CGParallelSolver = new CGParallel();
		CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads[i]);
		//Жұмсалған уақыт
		finishTime = omp_get_wtime();
		printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "CG parallel", threads[i]);
		//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
		printf(", iteration sani: %d", CGParallelSolver->iterationsCount);
		//Нәтижені тексеру
		matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);

		Sleep(600);
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
