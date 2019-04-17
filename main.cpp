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

	const char* inFileName = "out_noise.txt";
	const char* outFileName = "in.txt";

	FILE* ifile = fopen(inFileName, "rb");
	FILE* ofile = fopen(outFileName, "wb");

	cout << "Enter c(coder)/d(decoder): ";
	char act;
	cin >> act;

	fseek(ifile, 0, SEEK_END);// установить количество символов
	int t = ftell(ifile);
	int count = 0, i = 0;
	int n = ceil(t / 5.);// установление количества блоков

	fseek(ifile, 0, SEEK_SET);//возрат на начальную позицию в файле
	if (act == 'c')// кодируем
	{
		int m = t * 2;
		char* text = new char[m];// массив для закодированного текста
		char* start = new char[2];// храниться закодированный символ старта блока
		char* end = new char[2]; // храниться закодированный символ конца блока
		text[0] = 's'; // кодировка начла и конца блоков
		prep(text, start, 0);
		text[0] = 'e';
		prep(text, end, 0);

		int k = 0;
		char a[1];
		while (count < t)// считываем из файла текст и сразу же посимвольно кодируем
		{
			fread(a, 1, 1, ifile);
			prep(a, text, k);
			k += 2;
			count++;
		}

		count = 1;
		char* cope = new char[10]; // массив, для загрузки 10 символов в выходной файл
		int variant = 0;
		char* number = new char[len(n) * 2];
		writeNumber(n, number); // функция, преобразует int(n) в char*(number) и сразу же кодирует
		fwrite(number, 1, len(n) * 2, ofile); // записываем количесвто символов в выходной файл
		delete[] number;
		
		for (int j = 0; j <= m; j++) // теперь идём по полученному тексту
		{
			if (j % 10 == 0 && j != 0) // делим его на блоки по 5 символов(т.к. 1 символ кодируется 2-мя, делим по 10)
			{
				char* number = new char[len(count) * 2];
				writeNumber(count, number); // выясняем номер блока
				fwrite(start, 1, 2, ofile); // записываем начало блока
				fwrite(number, 1, len(count) * 2, ofile); // номер блока
				copier(cope, text, j); // берём из всего текста 10 символов
				fwrite(cope, 1, 10, ofile); // помещаем эти 10 символов в выходной файл
				fwrite(end, 1, 2, ofile); // записываем конец блока
				count++;
				variant = j; // если у нас количество символов не кратно 10, то остаток будет записываться с данной позиции
				delete[] number;
			}
		}

		if (count <= n) // если количество символов не кратно 10
		{
			char* number = new char[len(count) * 2];
			char* num = new char[len(count)];
			num = trans(count);
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
	else //декодер
	{
		char a[2];
		count = 1;
		char answer[18] = { 0 };
		int counter = 0, block_numer = 0, size = 0;
		int k = 0;
		readNum(counter, ifile); // считываем с файла, декодирование и помещение в int колличества блоков
		fseek(ifile, -2, SEEK_CUR);
		size += ftell(ifile); //так уже из файла взяли несколько символов
		while (size < t)
		{
			fread(a, 1, 2, ifile);// считываем 2 символа
			size += 2; 
			for (int j = 0; j < 2; j++) // деколируем оба символа
			{
				a[j] = decoder(a[j]);
			}

			for (int i = 0; i < 4; i++) // побитово совмещаем символы в один
				setBit(answer[k], i, getBit(a[0], i));
			for (int i = 4; i < 8; i++)
				setBit(answer[k], i, getBit(a[1], i - 4));

			if (answer[k] == 'e') // проверяем конец блока
			{
				fwrite(answer, 1, k, ofile); // записываем полученное колличество символов в блоке
				for (int i = 0; i < 7; i++) // обнуляем
					answer[i] = 0;
				k = 0;
				count++; // переходим к следующему блоку
				continue;
			}
			
			if (answer[k] == 's') // проверка на начало блока
			{
				size += readNum(block_numer, ifile) * 2; // считваем номер блока
				fseek(ifile, -2, SEEK_CUR);
				if (block_numer != count) // проверяем не пропущен ли какой-то блок
					while (block_numer != count && count <= n)
					{
						cout << counter << " блока нет" << endl;
						count++;
					}
				answer[k] = 0;// обнуляем данную переменную, так как в конечном выводе её не должно быть
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
