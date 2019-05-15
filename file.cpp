#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <locale.h>
#include <cstdio>
#include "func.h"
#include "file.h"

using namespace std;

HemCode::HemCode()
{
	infile = fopen(inFileName, "rb");
	outfile = fopen(outFileName, "wb");

	fseek(infile, 0, SEEK_END);// установить количество символов
	countSymbols = ftell(infile);
	fseek(infile, 0, SEEK_SET);
}

HemCode::~HemCode()
{
	fclose(infile);
	fclose(outfile);
}

HemCoder::HemCoder() : HemCode()
{
	countBlocks = ceil(countSymbols / 5.);// установление количества блоков
	countCharBlocks = new char[len(countBlocks) * 2];
	text[0] = 's';
	prep(text, start, 0);
	text[0] = 'e';
	prep(text, end, 0);
	writeNumber(countBlocks, countCharBlocks);
}

void HemCoder::write_in_file()
{
	int var = 0;
	count = 1;
	for (int i = 0; i < countCoderSymbols; i++)
	{
		if (i % 10 == 0 && i != 0) // делим его на блоки по 5 символов(т.к. 1 символ кодируется 2-мя, делим по 10)
		{
			char* number = new char[len(count) * 2];
			writeNumber(count, number); // выясняем номер блока
			copier(copyTextForWrite, text, i); // берём из всего текста 10 символов

			fwrite(start, 1, 2, outfile); // записываем начало блока
			fwrite(number, 1, len(count) * 2, outfile); // номер блока
			fwrite(countCharBlocks, 1, len(countBlocks) * 2, outfile);
			fwrite(copyTextForWrite, 1, 10, outfile); // помещаем эти 10 символов в выходной файл
			fwrite(end, 1, 2, outfile); // записываем конец блока

			count++;
			var = i; // если у нас количество символов не кратно 10, то остаток будет записываться с данной позиции
			delete[] number;
		}
	}

	if (count <= countBlocks)
	{
		char* number = new char[len(count) * 2];
		writeNumber(count, number);
		fwrite(start, 1, 2, outfile);
		fwrite(number, 1, len(count) * 2, outfile);
		fwrite(countCharBlocks, 1, len(countBlocks) * 2, outfile);
		for (int i = var; i < countCoderSymbols; i++)
		{
			number[0] = text[i];
			fwrite(number, 1, 1, outfile);
		}
		fwrite(end, 1, 2, outfile);
	}
}

void HemCoder::h_coder()
{
	char a[1];
	while (count < countSymbols)// считываем из файла текст и сразу же посимвольно кодируем
	{
		fread(a, 1, 1, infile);
		prep(a, text, counter);
		counter += 2;
		count++;
	}

	write_in_file();
}

void HemDecoder::write_in_file()
{
	if (answer[bitNumer] == 'e') // проверяем конец блока
	{
		fwrite(answer, 1, bitNumer, outfile); // записываем полученное колличество символов в блоке
		for (int i = 0; i < 18; i++) // обнуляем
			answer[i] = 0;
		bitNumer = 0;
		count++; // переходим к следующему блоку
		return;
	}

	if (answer[bitNumer] == 's') // проверка на начало блока
	{
		symbolNumer += readNum(blockNumer, infile) * 2; // считваем номер блока
		fseek(infile, -2, SEEK_CUR);
		symbolNumer += readNum(counter, infile) * 2; // считваем номер блока
		fseek(infile, -2, SEEK_CUR);
		if (blockNumer != count && counter == countBlocks) // проверяем не пропущен ли какой-то блок
			while (blockNumer != count && count <= countBlocks)
			{
				cout << count << " блока нет" << endl;
				count++;
			}
		answer[bitNumer] = 0;// обнуляем данную переменную, так как в конечном выводе её не должно быть
	}
}

void HemDecoder::h_decoder()
{
	while (symbolNumer < countSymbols)
	{
		fread(cask, 1, 2, infile);// считываем 2 символа
		symbolNumer += 2;
		for (int j = 0; j < 2; j++) // деколируем оба символа
		{
			cask[j] = decoder(cask[j]);
		}

		for (int i = 0; i < 4; i++) // побитово совмещаем символы в один
			setBit(answer[bitNumer], i, getBit(cask[0], i));
		for (int i = 4; i < 8; i++)
			setBit(answer[bitNumer], i, getBit(cask[1], i - 4));

		if (answer[bitNumer] == 'e' || answer[bitNumer] == 's') // проверяем конец блока
			write_in_file();
		else
			bitNumer++;
	}
}
