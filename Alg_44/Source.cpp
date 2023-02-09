#include <iostream>
#include "LittleAlgorithm.h"

using namespace std;

void main()
{
	try {
		Algorithm* method = new Algorithm();
		method = new LittleAlgorithm();
		method->LoadData(_strdup("input.txt"));
		method->Run();
	}
	catch (char* err) {
		cout << "Exception: \n" << err << endl;
	}
}

