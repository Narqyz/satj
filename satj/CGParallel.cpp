﻿#include "CGParallel.h"
#include "math.h"
#include <omp.h>


void CGParallel::resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size, int threads_count) {
	omp_set_num_threads(threads_count); // устанавливаем количество потоков в "параллельных" блоках 
	double *CurrentApproximation, *PreviousApproximation;
	double *CurrentGradient, *PreviousGradient;
	double *CurrentDirection, *PreviousDirection;
	double *Denom, *tempPointer;
	double Step;
	int Iter = 1, MaxIter = Size + 1;
	float Accuracy = 0.00001f; //шешім қателігі
	CurrentApproximation = new double[Size];
	PreviousApproximation = new double[Size];
	CurrentGradient = new double[Size];
	PreviousGradient = new double[Size];
	CurrentDirection = new double[Size];
	PreviousDirection = new double[Size];
	Denom = new double[Size];
	double CurrentGradient_sum = 0, PreviousGradient_sum = 0;
	// Бастапқы мәндерді енгізіп қою
	for (int i = 0; i < Size; i++) {
		PreviousApproximation[i] = 0;
		PreviousDirection[i] = 0;
		PreviousGradient[i] = -pVector[i];
	}
	do {
		if (Iter > 1) {
			tempPointer = PreviousApproximation;
			PreviousApproximation = CurrentApproximation;
			CurrentApproximation = tempPointer;
			tempPointer = PreviousGradient;
			PreviousGradient = CurrentGradient;
			CurrentGradient = tempPointer;
			tempPointer = PreviousDirection;
			PreviousDirection = CurrentDirection;
			CurrentDirection = tempPointer;
			PreviousGradient_sum = CurrentGradient_sum;
			CurrentGradient_sum = 0;
		}

		// Градиентті есептеу
#pragma omp parallel for reduction(+:CurrentGradient_sum)
		for (int i = 0; i < Size; i++) {
			CurrentGradient[i] = -pVector[i];
			for (int j = 0; j < Size; j++) {
				CurrentGradient[i] += pMatrix[i][j] * PreviousApproximation[j];
			}
			CurrentGradient_sum += CurrentGradient[i] * CurrentGradient[i];  // (new_g^T, new_g) алымы
		}

		// Бағыттауыш векторды есептеу
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			CurrentDirection[i] = -CurrentGradient[i] +	PreviousDirection[i] * CurrentGradient_sum / PreviousGradient_sum;
		}

		double ip1 = 0;
		// Бөлушектің алымы мен бөлімін алдын ала есептеп аламыз
#pragma omp parallel for reduction(+:ip1)
		for (int i = 0; i < Size; i++) {
			Denom[i] = 0;
			for (int j = 0; j < Size; j++)
				Denom[i] += pMatrix[i][j] * CurrentDirection[j];
			ip1 += CurrentDirection[i] * Denom[i];
		}
		// Қадам шамасын есептейміз
		Step = CurrentGradient_sum / ip1;
		// Жаңа шешімді есептейміз
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			CurrentApproximation[i] = PreviousApproximation[i] + Step * CurrentDirection[i];
		}
		Iter++;
	} while /* градиенттің үлкендігін тексереміз  
			* егер ол қателіктен Accuracy үлкен болса
			* және қайталау саны массив өлшемінен кіші болса
			* жаңа градиент, бағыт, қадам есептейміз
			*/
		(CurrentGradient_sum > Accuracy	&& Iter < MaxIter);
	for (int i = 0; i < Size; i++)
		pResult[i] = CurrentApproximation[i];
	iterationsCount = Iter;
}