#include <iostream>
#include <locale.h>
#include <cstdio>
#include "file.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	
	cout << "Enter c(coder)/d(decoder): ";
	char act;
	cin >> act;

	if (act == 'c')// кодируем
	{
		HemCoder text;
		text.h_coder();
	}
	else //декодер
	{
		HemDecoder text;
		text.h_decoder();
	}

	system("pause");
	return 0;
}
