#include "CGParallel.h"
#include "math.h"
#include <omp.h>


void CGParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 
	double* new_g, * prev_g, * new_x, * prev_x, * new_d, * prev_d, * A_prev_d;
	double step = 0, beta = 0, sum_new_g = 0, sum_prev_g = 0;
	int Iter = 0;
	float Accuracy = 0.01f; //шешім қателігі
	new_g = new double[Size];
	prev_g = new double[Size];
	new_x = new double[Size];
	prev_x = new double[Size];
	new_d = new double[Size];
	prev_d = new double[Size];
	A_prev_d = new double[Size];

	// Бастапқы мәндерді енгізіп қою
#pragma omp parallel for reduction(+:sum_new_g)
	for (int i = 0; i < Size; i++) {
		new_x[i] = 0;
		new_d[i] = new_g[i] = pVector[i];
		sum_new_g += new_g[i] * new_g[i];
	}
	do {
		Iter++;
		prev_g = new_g;
		prev_x = new_x;
		prev_d = new_d;
		sum_prev_g = sum_new_g;

		double ip = 0;

		// 1 этап вычисление step - қадам ұзындығы
#pragma omp parallel for reduction(+:ip)
		for (int i = 0; i < Size; i++) {
			A_prev_d[i] = 0;
			for (int j = 0; j < Size; j++) {
				A_prev_d[i] += pMatrix[i][j] * prev_d[j];
			}
			ip += A_prev_d[i] * prev_d[i];  //  бөлімі
		}
		step = sum_prev_g / ip;

		// 2 - 3 этап вычисление new_x - жаңа шешім, new_g - жаңа градиент
		sum_new_g = 0;
#pragma omp parallel for reduction(+:sum_new_g)
		for (int i = 0; i < Size; i++) {
			new_x[i] = prev_x[i] + step * prev_d[i];
			pResult[i] = new_x[i];
			new_g[i] = prev_g[i] - step * A_prev_d[i];
			sum_new_g += new_g[i] * new_g[i];
		}		

		if (sqrt(sum_new_g) <= Accuracy) break;

		// 4 этап вычисление beta - параметр сопряженности
		beta = sum_new_g / sum_prev_g;

		// 5 этап вычисление new_d - жаңа бағыт
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			new_d[i] = new_g[i] + beta * prev_d[i];
		}

		iterationsCount = Iter;
	} while (sqrt(sum_new_g) > Accuracy);
}