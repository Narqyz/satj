#include "CGSerial.h"
#include "math.h"
#include "stdio.h"


void CGSerial::resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size) {
	double* g, * d, * A_prev_d;
	double step = 0, beta = 0, sum_new_g = 0, sum_prev_g = 0, sum_PR;
	int Iter = 0, i, j;
	float Accuracy = 0.01f; //шешім қателігі
	g = new double[Size];
	d = new double[Size];
	A_prev_d = new double[Size];
	// Бастапқы мәндерді енгізіп қою
	for (i = 0; i < Size; i++) {
		pResult[i] = 0;
		g[i] = pVector[i];
		d[i] = -pVector[i];
		sum_new_g += g[i] * g[i];
	}
	do {
		Iter++;
		sum_prev_g = sum_new_g;
		double ip = 0;

		// 1 этап вычисление step - қадам ұзындығы
		for (i = 0; i < Size; i++) {
			A_prev_d[i] = 0;
			for (j = 0; j < Size; j++) {
				A_prev_d[i] += pMatrix[i][j] * d[j];
			}
			ip += A_prev_d[i] * d[i];  //  бөлімі
		}
		step = sum_prev_g / ip;

		// 2 - 3 этап вычисление new_x - жаңа шешім, new_g - жаңа градиент
		sum_new_g = 0;
		sum_PR = 0;
		for (i = 0; i < Size; i++) {
			pResult[i] = pResult[i] - step * d[i];
			g[i] = g[i] + step * A_prev_d[i];
			sum_new_g += g[i] * g[i];
		}

		if (sqrt(sum_new_g) <= Accuracy) break;

		// 4 этап вычисление beta - параметр сопряженности
		beta = fmax(sum_new_g / sum_prev_g, 0);

		// 5 этап вычисление new_d - жаңа бағыт
		for (i = 0; i < Size; i++) {
			d[i] = -g[i] + beta * d[i];
		}
		iterationsCount = Iter;
	} while (sqrt(sum_new_g) > Accuracy);
}