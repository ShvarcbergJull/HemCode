#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <locale.h>
#include <cstdio>
#include "func.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	const char* inFileName = "out.txt";
	const char* outFileName = "in.txt";

	FILE* ifile = fopen(inFileName, "rb");
	FILE* ofile = fopen(outFileName, "wb");

	char act;
	cin >> act;

	fseek(ifile, 0, SEEK_END);
	int t = ftell(ifile);
	int count = 0, i = 0;
	int n = ceil(t / 5.);
	int m = t * 2;
	char* text = new char[m];
	char* start = new char[2];
	char* end = new char[2];
	text[0] = 's';
	prep(text, start, 0);
	text[0] = 'e';
	prep(text, end, 0);

	fseek(ifile, 0, SEEK_SET);
	if (act == 'c')
	{
		int k = 0;
		char a[1];
		while (count < t)
		{
			fread(a, 1, 1, ifile);
			prep(a, text, k);
			k += 2;
			count++;
		}

		count = 1;
		char* cope = new char[5];
		int variant = 0;
		//доделать вывод колличества блоков(перевод из инт в чар), старт и энд чтоб выводилось нормально + номер блока(и это только кодер)
		for (int j = 0; j < m; j++)
		{
			if (j % 5 == 0 && j != 0)
			{
				char* number = new char[len(count) * 2];
				char* num = new char[len(count)];
				num = trans(count, len(count));
				for (int i = 0; i < len(count) * 2; i += 2)
				{
					prep(num, number, i);
					num++;
				}
				fwrite(number, 1, len(count) * 2, ofile);
				fwrite(start, 1, 2, ofile);
				copier(cope, text, j);
				fwrite(cope, 1, 5, ofile);
				fwrite(end, 1, 2, ofile);
				count++;
				variant = j;
				delete[] number;
			}
		}
		if (count <= n)
		{
			char* number = new char[len(count) * 2];
			char* num = new char[len(count)];
			num = trans(count, len(count));
			for (int i = 0; i < len(count) * 2; i += 2)
			{
				prep(num, number, i);
				num++;
			}
			fwrite(start, 1, 2, ofile);
			fwrite(number, 1, len(count) * 2, ofile);
			for (int i = variant; i < m; i++)
			{
				number[0] = text[i];
				fwrite(number, 1, 1, ofile);
			}
			fwrite(end, 1, 2, ofile);
		}
	}
	else
	{
		char a[2];
		char answer[7] = { 0 };
		int k = 0;
		while (count < t)
		{
			fread(a, 1, 2, ifile);
			for (int j = 0; j < 2; j++)
			{
				a[j] = decoder(a[j]);
			}

			for (int i = 0; i < 4; i++)
				setBit(answer[k], i, getBit(a[0], i));
			for (int i = 4; i < 8; i++)
				setBit(answer[k], i, getBit(a[1], i - 4));
			
			if (answer[k] == 'e')
			{
				fwrite(answer, 1, k, ofile);
				for (int i = 0; i < 7; i++)
					answer[i] = 0;
				k = 0;
			}
			
			count += 2;
			
			if (answer[k] == 's' || answer[k] == 0)
			{
				answer[k] = 0;
				continue;
			}
			k++;
		}
	}


	fclose(ifile);
	fclose(ofile);

	system("pause");
	return 0;
}
