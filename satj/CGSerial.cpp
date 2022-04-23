#include "CGSerial.h"
#include "math.h"
#include "stdio.h"


void CGSerial::resultCalculation(double** pMatrix, double* pVector, double* pResult, int Size) {
	double* CurrentApproximation, * PreviousApproximation;
	double* CurrentGradient, * PreviousGradient;
	double* CurrentDirection, * PreviousDirection;
	double* Denom, * tempPointer;
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
		CurrentApproximation[i] = 0;
		CurrentDirection[i] = 0;
		CurrentGradient[i] = -pVector[i];
		CurrentGradient_sum += CurrentGradient[i] * CurrentGradient[i];
	}
	do {
		PreviousApproximation = CurrentApproximation;
		PreviousGradient = CurrentGradient;
		PreviousDirection = CurrentDirection;
		PreviousGradient_sum = CurrentGradient_sum;
		CurrentGradient_sum = 0;
		// Градиентті есептеу

		// Бөлушектің алымы мен бөлімін алдын ала есептеп аламыз
		for (int i = 0; i < Size; i++) {
			CurrentGradient[i] = -pVector[i];
			for (int j = 0; j < Size; j++) {
				CurrentGradient[i] += pMatrix[i][j] * PreviousApproximation[j];
			}
			CurrentGradient_sum += CurrentGradient[i] * CurrentGradient[i];  // (new_g^T, new_g) алымы
		}
		// Бағыттауыш векторды есептеу
		for (int i = 0; i < Size; i++) {
			CurrentDirection[i] = -CurrentGradient[i] + PreviousDirection[i] * CurrentGradient_sum / PreviousGradient_sum;
		}

		double ip1 = 0;
		// Скаляр қадам бөлушектің бөлімін алдын ала есептеп аламыз
		for (int i = 0; i < Size; i++) {
			Denom[i] = 0;
			for (int j = 0; j < Size; j++)
				Denom[i] += pMatrix[j][i] * CurrentDirection[j];
			ip1 += CurrentDirection[i] * Denom[i];
		}
		// Қадам шамасын есептейміз
		Step = CurrentGradient_sum / ip1;
		// Жаңа шешімді есептейміз
		for (int i = 0; i < Size; i++) {
			CurrentApproximation[i] = PreviousApproximation[i] + Step * CurrentDirection[i];
		}
		Iter++;
	} while /* градиенттің үлкендігін тексереміз
			* егер ол қателіктен Accuracy үлкен болса
			* және қайталау саны массив өлшемінен кіші болса
			* жаңа градиент, бағыт, қадам есептейміз
			*/
		(CurrentGradient_sum >= Accuracy && Iter < MaxIter);
	for (int i = 0; i < Size; i++)
		pResult[i] = CurrentApproximation[i];
	iterationsCount = Iter;
}