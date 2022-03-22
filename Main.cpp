#include "CircuitAC.h"

#include <iostream>

using namespace std;


int main()
{
	setlocale(LC_ALL, "ru");
	cout.precision(4);
	cout.setf(ios::fixed);

	CircuitAC circuit("circuit_ac_data/circuit_ac_data_2.txt");
	MatrC IR = calculateCircuit(circuit);

	cout << IR << endl;

	return 0;
}
