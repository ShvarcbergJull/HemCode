#pragma once
#include <fstream>

using namespace std;

class HemCode
{
protected:
	const char* inFileName = "out_noise.txt";
	const char* outFileName = "in2.txt";

	FILE* infile;
	FILE* outfile;

	int countSymbols, countBlocks;
	int count = 0, counter = 0;

	virtual void write_in_file() = 0;

public:
	HemCode();

	~HemCode();
};

class HemCoder : public HemCode
{
private:
	int countCoderSymbols = countSymbols * 2;
	char* text = new char[countCoderSymbols];
	char* start = new char[2];
	char* end = new char[2];
	char* countCharBlocks;
	char* copyTextForWrite = new char[10];

	void write_in_file();

public:
	HemCoder();

	void h_coder();

	~HemCoder()
	{
		delete[] text;
		delete[] start;
		delete[] end;
		delete[] countCharBlocks;
		delete[] copyTextForWrite;
	}
};


class HemDecoder : public HemCode
{
private:
	char cask[2];
	char answer[18] = { 0 };
	int blockNumer = 0, symbolNumer = 0, bitNumer = 0;

	void write_in_file();

public:
	HemDecoder() : HemCode()
	{
		countBlocks = ceil(countSymbols / 10.);// установление количества блоков
		count = 1;
	}

	void h_decoder();
};
