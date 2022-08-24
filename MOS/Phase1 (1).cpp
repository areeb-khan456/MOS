#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

char Memory[100][5], IR[5], R[5], c, buffer[10][5];
int b, ic, si, e, read_check;

FILE *Fread, *Fwrite;

void INIT();
void LOAD();
void STARTEXECUTION();
void EXECUTEPROGRAM();
void MOS(int);
void WRITE();
void TERMINATE();
void READ(int);

int main()
{
	int i, j;
	Fread = fopen("input.txt", "r");
	Fwrite = fopen("output.txt", "w");

	if (!Fread)
		cout << "File does not exit" << endl;
	else
	{
		LOAD();
	}

	cout << "Operation complete" << endl;
}

void INIT()
{
	int i, j;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Memory[i][j] = '\0';
		}
	}
	b = 0;
	e = b + 10;
	read_check = 0;
}

void LOAD()
{
	int i, j, k;
	char ch, temp[5];
	ch = fgetc(Fread);
	INIT();
	do
	{
		i = 0;
		while (i < 4 && ch != '\n')
		{
			temp[i] = ch;
			ch = fgetc(Fread);
			i++;
		}
		temp[i] = '\0';

		if ((strcmp(temp, "$AMJ")) == 0)
		{
			while (ch != '\n')
				ch = getc(Fread);
		}
		else if ((strcmp(temp, "$DTA")) == 0)
		{
			STARTEXECUTION();
		}
		else if ((strcmp(temp, "$DTA")) != 0)
		{
			i = 0;
			if (b == e)
			{
				e += 10;
			}
			for (k = 0; k < 4; k++)
			{
				Memory[b][k] = temp[i];
				i++;
			}
			b++;
		}
		if (ch == '\n')
			ch = fgetc(Fread);
	}
	while (ch != EOF);
}

void STARTEXECUTION()
{
	ic = 0;
	EXECUTEPROGRAM();
}

void EXECUTEPROGRAM()
{
	int k, i, j, location, x, y;
	while (ic < b)
	{
		for (k = 0; k < 4; k++)
		{
			IR[k] = Memory[ic][k];
		}

		x = atoi(&IR[2]);
		location = x;
		if (IR[0] == 'L' && IR[1] == 'R')
		{
			for (i = 0; i < 4; i++)
				R[i] = '\0';
			for (j = 0; j < 4; j++)
			{
				if (Memory[location][j] != '\0' && Memory[location][j] != '\n')
					R[j] = Memory[location][j];
				else
					break;
			}
		}
		else if (IR[0] == 'S' && IR[1] == 'R')
		{
			for (j = 0; j < 4 && R[j] != '\0'; j++)
			{
				Memory[location][j] = R[j];
			}
		}
		else if (IR[0] == 'C' && IR[1] == 'R')
		{
			for (i = 0; i < 4; i++)
			{
				if (R[i] == Memory[location][i])
					c = 't';
				else
				{
					c = 'f';
					break;
				}
			}
		}
		else if (IR[0] == 'B' && IR[1] == 'T')
		{
			if (c == 't')
			{
				ic = location;
				ic--;
			}
		}
		else if (IR[0] == 'G' && IR[1] == 'D')
		{
			si = 1;
			MOS(location);
		}
		else if (IR[0] == 'P' && IR[1] == 'D')
		{
			si = 2;
			MOS(location);
		}
		else if (IR[0] == 'H')
		{
			si = 3;
			MOS(location);
		}
		ic++;
	}
}

void MOS(int location)
{
	if(si ==  1)
	{
		READ(location);
		si = 0;
	}

	else if( si ==  2)
	{
		WRITE();
		si = 0;
	}
	else
	{
		TERMINATE();
		si = 0;
	}
}

void READ(int location)
{
	int i, j, k;
	char ch, temp[20];
	if (read_check == 0)
	{
		k = location - 1;
		e = location + 10;
		ch = fgetc(Fread);
		while (1)
		{
			i = 0;
			while (i < 4 && ch != '\n')
			{
				temp[i] = ch;
				i++;
				ch = fgetc(Fread);
			}
			temp[i] = '\0';
			if ((strcmp(temp, "$END")) == 0)
			{
				while (ch != '\n' && ch != EOF)
					ch = fgetc(Fread);
				break;
			}
			else
			{
				if (ch == '\n')
				{
					k++;
					i = 0;
					while (i < 4)
					{
						Memory[k][i] = temp[i];
						i++;
					}
					k = e - 1;
					e = e + 10;
				}
				else {
					k++;
					i = 0;
					while (i < 4)
					{
						Memory[k][i] = temp[i];
						i++;
					}
				}
			}
			if (ch == '\n')
				ch = fgetc(Fread);
		}
		read_check = 1;
	}
}

void WRITE()
{
	int i, j, k, location, x;
	char ch1;
	IR[3] = 0;
	x = atoi(&IR[2]);
	location = x * 10;
	i = location;
	while (i <= location + 9)
	{
		for (k = 0; k < 4; k++)
		{
			if (Memory[i][k] != '\0')
			{
				fputc(Memory[i][k], Fwrite);
				ch1 = Memory[i][k];
			}
			else
				break;
		}
		i++;
	}
	if (ch1 != '\n')
		fputc('\n', Fwrite);
}

void TERMINATE()
{
	fputc('\n', Fwrite);
	fputc('\n', Fwrite);
	LOAD();
}