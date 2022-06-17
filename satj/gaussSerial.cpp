#include "gaussSerial.h"
#include "math.h"
#include "stdio.h"
#include "matrixHelpers.h"
#include <iostream>

/*
Класс конструкторы. 
Есептеу орындалу барысында екі ақпаратты, 
әр жолдың жетекші болғаны және қай итерацияда 
қай жол жетекші болғанын сақтайтын екі массив қажет. 
pSerialPivotIter массиві тура жүріс кезінде алдыңғы 
итерацияларда жетекші болған жолдарды арналған. 
Басында бұл массивтің барлық элементі -1 тең болады. 
pSerialPivotPos массиві кері жүріс кезінде теңдеулерді шешу 
ретін анықтауға қажет. Бұл әдістің коды келесідей болады:*/
gaussSerial::gaussSerial(int size) {
	mSize = size;
	pSerialPivotIter = new int[size];             // хранить в каком цикле стал главным определенная строка, нужен для прямого хода
	pSerialPivotPos = new int[size];                    // хранить порядок строк по итерации, нужен для обратного хода	
	for (int i = 0; i < size; i++) {
		pSerialPivotIter[i] = -1;                    //Бұл жолдарға әлі кірмегенімізді көрсету үшін -1 толтырамыз
	}
}

/*
Көрсетілген әдістерді ретімен орындау әдісі. 
Бұл әдіс – есептеуді бастаушы әдіс. 
Мұнда әдістерді орындау реті көрсетіледі. 
Бағандарды кезегімен жою үшін итерациямен орындаймыз. 
Бұл әдістің коды келесідей болады:
*/
int gaussSerial::resultCalculation(double** pMatrix, double* pVector, double* pResult) {
	serialGaussianElimination(pMatrix, pVector);	// Тура жүріс, айнымалыларды Гаусс бойынша жою
	serialBackSubstitution(pMatrix, pVector, pResult); // Кері жүріс, айнымалыларды есептеу	
	return 0;
}


// Тура жүріс, айнымалыларды Гаусс бойынша жою
void gaussSerial::serialGaussianElimination(double** pMatrix, double* pVector) {

	// Ағымдағы айналмалы жолдың саны
	for (int Iter = 0; Iter < mSize; Iter++) {
		int PivotRow = findPivotRow(pMatrix, Iter); //жетекші жолды максимумы бойынша анықтау жолды табу
		pSerialPivotPos[Iter] = PivotRow; // хранить порядок строк по итерации, нужен для обратного хода
		pSerialPivotIter[PivotRow] = Iter; // хранить в каком цикле стал главным определенная строка, нужен для прямого хода

		serialColumnElimination(pMatrix, pVector, PivotRow, Iter);
	}
}

// Iter бағаны үшін макс элементті жолды таңдау жолды табу
/*Максимумды іздеу әдісі. 
Бұл әдісте біз баған бойынша барлық жолды циклмен өтіп модулі бойынша 
үлкен және pSerialPivotIter массивіне жол индексі -1 – ге тең жолдарды іздейді. 
Әдіс ішінде екі айнымалы қолданылады. MaxValue – цикл бағандағы модулі бойынша үлкен 
элементтің мәні, PivotRow – цикл бағандағы модулі бойынша үлкен элемент орналасқан
жолдың индексі. Бұл әдістің коды келесідей болады:*/
int gaussSerial::findPivotRow(double** pMatrix, int Iter) {
	double MaxValue = 0; // бағандағы макс мәні
	int PivotRow = -1; // бағандағы макс мән жолдың индексі
	// жолдар бойынша макс іздеу және ол жол алдын қолданылмаған болу керек
	for (int i = 0; i < mSize; i++) {
		if ((pSerialPivotIter[i] == -1) && (fabs(pMatrix[i][Iter]) >= MaxValue)) {
			PivotRow = i;
			MaxValue = fabs(pMatrix[i][Iter]);
		}

	}
	return PivotRow;
}

// Бағанның басқа элементтерін жою
/*
* Баған басқа элементтерін жою әдісі. 
Бұл әдісте біз алдыңғы итерацияларда жетекші 
болмаған жолдардың баған элементін жоямыз, нөлге айнылдырамыз. 
Сондай – ақ, жойылатын бағаннан кейінгі тұрған элементтерге де 
және бос мүшелер векторына да арифметикалық түрлендіруді орындаймыз. 
PivotValue – баған бойынша үлкен мәні, PivotFactor алгебралық түрлендіру 
үшін көбейту мәні. Жоюды орындау кезінде, алдыңғы жоюлар орындалған кездегі 
жетекші болған жолдарға алгебралық түрлендірулер орындалмауы керек. 
Оны қадағалау үшін pSerialPivotIter массивіндегі сәйкес жолдың мәні -1 – ге 
тең болмауы керек. Бұл әдістің коды келесідей болады:
*/
void gaussSerial::serialColumnElimination(double** pMatrix, double* pVector, int PivotRow, int Iter) {
	double PivotValue = pMatrix[PivotRow][Iter];
	for (int i = 0; i < mSize; i++) {
		if (pSerialPivotIter[i] == -1) {
			double PivotFactor = pMatrix[i][Iter] / PivotValue;
			for (int j = Iter; j < mSize; j++) {
				pMatrix[i][j] -= PivotFactor * pMatrix[PivotRow][j];
			}
			pVector[i] -= PivotFactor * pVector[PivotRow];
		}
	}
}

// Кері жүріс
/*
* Кері жүріс – айнымалыларды есептеу әдісі. 
Бұл әдісте жетекші болу ретімен жазылған pSerialPivotPos 
массивіне соңынан басына қарай жүре отырып, айнымалыларды 
есептеп pResult массивіне жазып отырамыз. Бұл әдістің коды келесідей болады:
*/
void gaussSerial::serialBackSubstitution(double** pMatrix, double* pVector, double* pResult) {
	for (int i = mSize - 1; i >= 0; i--) {
		int RowIndex = pSerialPivotPos[i];
		pResult[i] = pVector[RowIndex] / pMatrix[RowIndex][i];
		for (int j = i + 1; j < mSize; j++) {
			pResult[i] -= pMatrix[RowIndex][j] * pResult[j] / pMatrix[RowIndex][i];
		}
	}
}