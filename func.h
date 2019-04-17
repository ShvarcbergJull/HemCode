#pragma once

char getBit(char b, int i)
{
	b = b & (1 << i);
	b = b != 0 ? 1 : 0;
	return b;
}

void setBit(char& b, int i, char bit)
{
	b = (bit == 1) ? b | (1 << i) : b & ~(1 << i);
}

void setBit2(int& b, int i, char bit)
{
	b = (bit == 1) ? b | (1 << i) : b & ~(1 << i);
}

char coder(char a)
{
	char e[8] = {0};
	char ans = 0;

	e[2] = getBit(a, 0);
	e[4] = getBit(a, 1);
	e[5] = getBit(a, 2);
	e[6] = getBit(a, 3);

	e[0] = e[2] ^ e[4] ^ e[6];
	e[1] = e[2] ^ e[5] ^ e[6];
	e[3] = e[4] ^ e[5] ^ e[6];

	for (int i = 0; i < 7; i++)
	{
		e[7] ^= e[i];
		setBit(ans, i, e[i]);
	}

	setBit(ans, 7, e[7]);

	return ans;
}

char decoder(char a) 
{
	char e[8] = { 0 };
	char sum[4] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		e[i] = getBit(a, i);
		if (i != 7)
			sum[3] ^= e[i];
	}

	int ind = 0;

	sum[0] = e[0] ^ e[2] ^ e[4] ^ e[6];
	sum[1] = e[1] ^ e[2] ^ e[5] ^ e[6];
	sum[2] = e[3] ^ e[4] ^ e[5] ^ e[6];

	for (int i = 0; i < 3; i++)
		setBit2(ind, i, sum[i]);

	if (ind != 0)
	{
		ind--;
		e[ind] = e[ind] ^ 1;
	}

	char ans = 0;
	setBit(ans, 0, e[2]);
	setBit(ans, 1, e[4]);
	setBit(ans, 2, e[5]);
	setBit(ans, 3, e[6]);

	return ans;
}

void prep(char* a, char* &text, int k) // кодировка символа
{
	int j = 0, i = 0;
	char b = 0;

	while (i < 2)
	{
		if (j == 4)
		{
			*a = *a >> j;
			text[k] = coder(b);
			k++;
			j = 0;
			b = 0;
			i++;
		}
		else
		{
			b = b | *a & (1 << j);
			j++;
		}
	}
}

int len(int n) // узнаём длину интового числа
{
	int i = 0;
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return i;
}

char* trans(int n) // перевод из int в char
{
	int m = len(n);
	char* ans = new char[m];
	int j = m - 1;
	while (n != 0)
	{
		int i = n % 10;
		ans[j] = i + '0';
		j--;
		n /= 10;
	}

	return ans;
}

void copier(char* &copy, char* orig, int k) // создание копии
{
	int j = 0;
	for (int i = k - 10; i < k; i++)
	{
		copy[j] = orig[i];
		j++;
	}
}

int readNum(int &number, FILE* file) // декодировка для количества блоков и номера блока
{
	number = 0;
	char a = '0';
	char b[2], c = 0;
	int k = 0;
	while (a >= '0' && a <= '9')
	{
		c = 0;
		fread(b, 1, 2, file);
		for (int j = 0; j < 2; j++)
		{
			b[j] = decoder(b[j]);
		}

		for (int i = 0; i < 4; i++)
			setBit(c, i, getBit(b[0], i));
		for (int i = 4; i < 8; i++)
			setBit(c, i, getBit(b[1], i - 4));

		if (c >= '0' && c <= '9')
		{
			number *= 10;
			number += c - '0';
			k++;
		}
		a = c;
	}
	return k;
}

void writeNumber(int n, char* &number)// кодировка интовых переменных
{
	char* num = new char[len(n)];
	num = trans(n);
	for (int i = 0; i < len(n) * 2; i += 2)
	{
		prep(num, number, i);
		num++;
	}
}
