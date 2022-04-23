#include "CGParallel.h"
#include "math.h"
#include <omp.h>


void CGParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 
	double* new_r, * prev_r, * new_x, * prev_x, * new_z, * prev_z, * A_prev_z;
	double alpha = 0, beta = 0, sum_new_r = 0, sum_prev_r = 0;
	int Iter = 1;
	float Accuracy = 0.01f; //шешім қателігі
	new_r = new double[Size];
	prev_r = new double[Size];
	new_x = new double[Size];
	prev_x = new double[Size];
	new_z = new double[Size];
	prev_z = new double[Size];
	A_prev_z = new double[Size];
	double CurrentGradient_sum = 0, PreviousGradient_sum = 0;
	// Бастапқы мәндерді енгізіп қою

#pragma omp parallel for reduction(+:sum_new_r)
	for (int i = 0; i < Size; i++) {
		new_x[i] = 0;
		new_r[i] = pVector[i];
		new_z[i] = new_r[i];
		sum_new_r += new_r[i] * new_r[i];
	}
	do {
		prev_r = new_r;
		prev_x = new_x;
		prev_z = new_z;
		sum_prev_r = sum_new_r;

		double ip = 0;

		// 1 этап вычисление alpha

#pragma omp parallel for lastprivate(A_prev_z[i]) reduction(+:ip)
		for (int i = 0; i < Size; i++) {
			A_prev_z[i] = 0;
			for (int j = 0; j < Size; j++) {
				A_prev_z[i] += pMatrix[i][j] * prev_z[j];
			}
			ip += A_prev_z[i] * prev_z[i];  //  бөлімі
		}
		alpha = sum_prev_r / ip;

		// 2 этап вычисление new_x
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			new_x[i] = prev_x[i] + alpha * prev_z[i];
		}

		// 3 этап вычисление new_r
		sum_new_r = 0;
#pragma omp parallel for reduction(+:sum_new_r)
		for (int i = 0; i < Size; i++) {
			new_r[i] = prev_r[i] - alpha * A_prev_z[i];
			sum_new_r += new_r[i] * new_r[i];
		}

		// 4 этап вычисление beta
		beta = sum_new_r / sum_prev_r;

		// 5 этап вычисление new_z
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			new_z[i] = new_r[i] + beta * prev_z[i];
		}
		Iter++;

	} while (sqrt(sum_new_r) > Accuracy);

#pragma omp parallel for
	for (int i = 0; i < Size; i++)
		pResult[i] = new_x[i];
	iterationsCount = Iter;
}