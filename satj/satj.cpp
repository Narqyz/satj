// satj.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstdlib>
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
	int threads2 = 2;
	int threads4 = 4;
	int threads8 = 8;
	int threads16 = 16;

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

	//Объекты бәрін жасаймыз
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
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s", finishTime - startTime, "Gauss serial");


	// Гаусс параллель алгоритмі 2 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	gaussParallelSolver = new gaussParallel(mSize);
	gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "Gauss parallel", threads2);

	// Гаусс параллель алгоритмі 4 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	gaussParallelSolver = new gaussParallel(mSize);
	gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, Threads count: %d", finishTime - startTime, "Gauss parallel", threads4);
	
	// Гаусс параллель алгоритмі 8 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	gaussParallelSolver = new gaussParallel(mSize);
	gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, Threads count: %d", finishTime - startTime, "Gauss parallel", threads8);
	
	// Гаусс параллель алгоритмі 16 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	gaussParallelSolver = new gaussParallel(mSize);
	gaussParallelSolver->resultCalculation(pMatrix, pVector, pResult, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, Threads count: %d", finishTime - startTime, "Gauss parallel", threads16 );


	// CG сызықты алгоритмі
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	CGSerialSolver = new CGSerial();
	CGSerialSolver->resultCalculation(pMatrix, pVector, pResult, mSize);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s", finishTime - startTime, "CG serial ");
	//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
	printf("iteration sani: %d", CGSerialSolver->iterationsCount);


	// CG параллель алгоритмі 2 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	CGParallelSolver = new CGParallel();
	CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "CG parallel", threads2);
	//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
	printf(", iteration sani: %d", CGParallelSolver->iterationsCount);

	// CG параллель алгоритмі 4 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	CGParallelSolver = new CGParallel();
	CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "CG parallel", threads4);
	//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
	printf(", iteration sani: %d", CGParallelSolver->iterationsCount);

	// CG параллель алгоритмі 8 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	CGParallelSolver = new CGParallel();
	CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "CG parallel", threads8);
	//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
	printf(", iteration sani: %d", CGParallelSolver->iterationsCount);

	// CG параллель алгоритмі 16 поток
	//Уақытты өлшеу
	startTime = omp_get_wtime();
	CGParallelSolver = new CGParallel();
	CGParallelSolver->resultCalculation(pMatrix, pVector, pResult, mSize, threads2);
	//Жұмсалған уақыт
	finishTime = omp_get_wtime();
	//Нәтижені тексеру
	matrixHelpers::testSolvingResult(pMatrix, pVector, pResult, mSize);
	printf("\nJumsalgan uakit: %f second, method: %s, threads count: %d", finishTime - startTime, "CG parallel", threads16);
	//CG әдісінің бірі болса, онда қайталанулар санын көрсету керек
	printf(", iteration sani: %d", CGParallelSolver->iterationsCount);




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
