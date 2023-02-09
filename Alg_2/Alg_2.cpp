#include <iostream>
#include <fstream>
#include <cstring>

#define MAX_SIZE 100000
#define MAX_INT_DIGITS 9

#define SUCCESS_FLG 0
#define FAIL_FLG 1

int start(const char* path, const char* path2, int sort_flg);
void write_in_file(const char* path);
void write_in_file(const char* path, int *array, int n);
void str_parser(const char* string, const char* path, int sort_flg);

int update_gap(int gap);
void comb_sort(int arr[], int n);

int main(int argc, char** argv)
{
	if (argc < 3 || argc > 4)
		return FAIL_FLG;

	const char* path = argv[1];
	const char* path2 = argv[2];

	int sort_flg = 1;
	if (argc == 4 && !strcmp(argv[3], "-t"))
		sort_flg = 0;


	start(path, path2, sort_flg);
	
	return 0;
}

int start(const char* path, const char* path2, int sort_flg) {
	std::ifstream in_ptr(path);
	std::ofstream out_ptr(path2);
	out_ptr.close();

	if (!in_ptr.is_open()) {
		return FAIL_FLG;
	}

	while (!in_ptr.eof()) {
		char* str = new char[2 * MAX_SIZE * MAX_INT_DIGITS];
		in_ptr.getline(str, 2 * MAX_SIZE * MAX_INT_DIGITS);

		str_parser(str, path2, sort_flg);
		delete[] str;
	}

	in_ptr.close();

	return SUCCESS_FLG;
}

void write_in_file(const char* path, int* array, int n) {
	std::ofstream out_ptr(path, std::ios::app);

	for (int i = 0; i < n; i++)
		out_ptr << array[i] << " ";

	out_ptr << std::endl;

	out_ptr.close();
}

void write_in_file(const char* path) {
	std::ofstream out_ptr(path, std::ios::app);

	out_ptr << std::endl;
	
	out_ptr.close();
}

void str_parser(const char* string, const char* path, int sort_flg) {
	int i = 0;
	while (string[i] == ' ')
		i++;
	
	if (!string[i]) {
		write_in_file(path);
		return;
	}

	int* cur_array = new int[MAX_SIZE];
	int ind = 0;
	int size = 0;

	while (string[i]) {
		char number[MAX_INT_DIGITS];
		int num_flg = 0;
		int j = 0;

		while (string[i] && string[i] != ' ') {
			if ((string[i] < '0' || string[i] > '9') && string[i] != '-')
				num_flg = 1;

			number[j++] = string[i];
			i++;
		}

		if (num_flg == 0) {
			cur_array[ind++] = atoi(number);
			size++;
		}

		while (string[i] && string[i] == ' ')
			i++;
	}

	if (size == 0) {
		delete[] cur_array;
		write_in_file(path);
		return;
	}

	int* array = new int[size];
	for (int i = 0; i < size; i++)		
		array[i] = cur_array[i];
	
	delete[] cur_array;

	if (sort_flg == 0)
		comb_sort(array, size);

	write_in_file(path, array, size);

	delete[] array;
}

int update_gap(int gap)
{
	gap = (gap * 10) / 13;
	if (gap < 1)
		return 1;
	else
		return gap;
}

void comb_sort(int arr[], int n)
{ 
	int gap = n;
	bool swapped = true;
	while (gap > 1 || swapped == true)
	{
		gap = update_gap(gap);
		swapped = false;
		for (int i = 0; i < (n - gap); i++)
		{
			int temp;
			if (arr[i] > arr[i + gap])
			{
				temp = arr[i];
				arr[i] = arr[i + gap];
				arr[i + gap] = temp;
				swapped = true;
			}
		}
	}
}