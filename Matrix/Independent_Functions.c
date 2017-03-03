//
//  Independent_Functions.c
//  Matrix
//
//  Created by LI YANZHE on 14/12/2016.
//  Copyright © 2016 Yanzhe Lee. All rights reserved.
//

#include "Matrix.h"

int Check_Option(int argc, const char **argv, char *option)
{
	int optionCheck;
	int flag = 0;
	for (optionCheck = 0; optionCheck < argc; ++optionCheck)
	{
		if (strcmp(argv[optionCheck], option) == 0)
		{
			flag = 1;
			break;
		}
	}
	return flag;
}

int Show_File_Text(FILE *fp)
{
	char C;
	//    FILE *fp = fopen(__FILE__, "r");
	if (fp == NULL)
	{
		perror("FILE Text open error");
		return 1;
	}
	do
	{
		C = fgetc(fp);
		putchar(C);
	} while (C != EOF);

	fclose(fp);
	return 0;
}

void Safe_Flush(FILE *fp)                                                                    //用于清空scanf缓冲区
{
	int ch;
	while ((ch = fgetc(fp)) != EOF && ch != '\n');
}

double** Create_Matrix(int m, int n, char *DISPLAY)
{
	int i;
	double **Matrix;
	if ((Matrix = (double**)calloc(m, sizeof(double*))) == NULL)
	{
		printf("Matrix%s Calloc Error\n", DISPLAY);
		perror("Error Info");
		return NULL;
	}

	for (i = 0; i < m; ++i)
	{
		Matrix[i] = (double*)calloc(n, sizeof(double));
	}
	return Matrix;
}

void Free_Matrix(double **Matrix, int m)
{
	int i;
	for (i = 0; i < m; ++i)
	{
		free(Matrix[i]);
	}
	free(Matrix);
}

void Scalar_Multiplication(double k, double **Matrix, int r, int m, int n)                           //数乘运算 用k数乘第r行
{
	int j;
	for (j = 0; j <= n - 1; ++j)
		Matrix[r][j] *= k;
}

void Show_Matrix(double **Matrix, int startRow, int startColumn, int endRow, int endColumn, int displayFlag)
{                                                                                                 //用于向屏幕打印整个矩阵
	int i, j;
	for (i = startRow - 1; i <= endRow - 1; ++i)
	{
		for (j = startColumn - 1; j <= endColumn - 1; ++j)
		{
			if (displayFlag == 1)
			{
				if (Matrix[i][j] == floor(Matrix[i][j]))
					printf("%9d", (int)floor(Matrix[i][j]));
				else if (Matrix[i][j] == ceil(Matrix[i][j]))
					printf("%9d", (int)ceil(Matrix[i][j]));
				else
					printf("%9.3lf", Matrix[i][j]);
			}
			else printf("%9.3lf", Matrix[i][j]);
		}
		puts("");
	}
}

void Approximate(double **Matrix, int m, int n, int index)
{
	int i, j;
	double p = pow(10, -index);
	for (i = 0; i <= m - 1; ++i)
	{
		for (j = 0; j <= n - 1; ++j)
		{
			if (Matrix[i][j] - floor(Matrix[i][j]) < p)
				Matrix[i][j] = floor(Matrix[i][j]);                                            //近似运算
			else if (ceil(Matrix[i][j]) - Matrix[i][j] < p)
				Matrix[i][j] = ceil(Matrix[i][j]);
		}
	}
}

void Row_Exchange(double **Matrix, int r1, int r2, int n)                                       //行交换，r1行与r2行交换
{
	double temp = 0;
	int j;
	for (j = 0; j <= n - 1; ++j)
	{
		temp = Matrix[r1][j];
		Matrix[r1][j] = Matrix[r2][j];
		Matrix[r2][j] = temp;
	}
}

void Row_Add(double **Matrix, int r1, int r2, int n, int flag)                                   //行加减运算
{
	int c;
	if (flag == 0)                                                                              //flag=0时进行r1行加r2行
		for (c = 0; c <= n - 1; c++)
		{
			Matrix[r1][c] += Matrix[r2][c];
		}
	if (flag == 1)                                                                              //flag=1时进行r1行减r2行
		for (c = 0; c <= n - 1; ++c)
		{
			Matrix[r1][c] -= Matrix[r2][c];
		}
}

int Find_No_Zero_Row(double **Matrix, int column, int m)                                       //从上到下找出第column列中最后一个非零元所在的行号
{
	int count = 0, row;
	for (row = 0; row < m; ++row)
	{
		if (fabs(Matrix[row][column]) != 0)count = row;
	}

	return count;
}

int Find_Leading_Column(double **Matrix, int row, int n)
{
	int count = 0, column;
	for (column = n - 1; column >= 0; column--)
	{
		if (fabs(Matrix[row][column]) != 0)count = column;
	}

	return count;
}

int Check_Zero_Matrix(double **Matrix, int m, int n)
{
	int zeroElementCount = 0, i, j;
	for (i = 0; i <= m - 1; ++i)                                                                   //数出增广矩阵中零元素的个数
	{
		for (j = 0; j <= n - 1; ++j)
		{
			if (Matrix[i][j] == 0)zeroElementCount++;
		}
	}
	if (zeroElementCount == (m)*(n))
	{
		//        printf("Zero Element = %d\n",zeroElementCount);
		return -1;                                                                           //如果用户输入的矩阵元素全为0，则不用化简，直接返回
	}
	else return zeroElementCount;
}

void Rand_Fill(double **Matrix, int m, int n, int MIN, int MAX, int MODE)
{
	int i, j;
//	srand((unsigned)time(NULL));
	for (i = 0; i <= m - 1; ++i)
	{
		for (j = 0; j <= n - 1; ++j)                                                               //测试需要 随机整数填充增广矩阵
		{
			if (MODE == 1)
				Matrix[i][j] = MIN + rand() % (MAX - MIN - 1) + (double)rand() / RAND_MAX;                 //随机小数填充
			else Matrix[i][j] = MIN + rand() % MAX;                                                //随机整数填充
		}
	}
}

double*** Column_Vector_Extract(double **Matrix, int m, int n)
{
	int i, r, s;
	double ***vector_System = (double***)calloc(n, sizeof(double**));
	if (vector_System == NULL)
	{
		perror("vector_System");
		exit(1);
	}
	for (i = 0; i < n; ++i)
		vector_System[i] = Create_Matrix(m, 1, "Vector System");

	for (s = 0; s < n; ++s)
	{
		for (r = 0; r < m; ++r)
		{
			vector_System[s][r][0] = Matrix[r][s];
		}
	}
	return vector_System;
}

void Column_Vector_Refill(double ***vector_System, double **Matrix, int m, int n)
{
	int r, s;
	for (s = 0; s < n; ++s)
	{
		for (r = 0; r < m; ++r)
		{
			Matrix[r][s] = vector_System[s][r][0];
		}
	}
}

void Show_Index_Page()
{
	puts("\n--------------------------------------------------------------------------------");
	puts("|                                                                              |");
	puts("|         Copyright (C) 2016 Yanzhe Lee. All rights reserved.                  |");
	puts("|                                                                              |");
	puts("|                       Harbin Institute of Technology                         |");
	puts("|                                                                              |");
	puts("|         License GPLv3+: GNU GPL version 3 or later                           |");
	puts("|                                                                              |");
	puts("|         This is free software: you are free to change and redistribute it    |");
	puts("|                                                                              |");
	puts("|         Email: lee.yanzhe@yanzhe.org           Version 1.2.0                 |");
	puts("|                                                                              |");
	puts("--------------------------------------------------------------------------------");
	puts("          Please maximize your window to get a better display effect            ");
}

void Show_Menu_Page()
{
	puts("--------------------------------------------------------------------------------");
	puts("-     1----    Determinant   ----    2----       Adjoint Matrix      ----     --");
	puts("-     3----  Inverse Matrix  ----    4----   Matrix Multiplication   ----     --");
	puts("-     5---- Row Echelon Form ----    6----     Row Canonical Form    ----     --");
	puts("-     7---- Linear Equations ----    8---- Schmidt Orthogonalization ----     --");
	puts("-     c----    Config Mode   ----    q----            Quit           ----     --");
	puts("--------------------------------------------------------------------------------");
}

void Show_Help_Page()
{
	puts("--------------------------------------------------------------------------------");
	puts("\nSyntax: Matrix [Commands] [options]\n");

	puts("Commands:");
	puts("  -h --help       Show this help page");
	puts("  -c --config     Read config file named 'config.json' from $CURRENT_DIR");
	puts("                  -------------------------------------------------------------");
	puts("                  | You MUST write 'config.json' file in standard JSON format |");
	puts("                  -------------------------------------------------------------");
	puts("     --menu       Show Mode Menu page");
	puts("     --mode-1     --mode-2      --mode-3");
	puts("     --mode-4     --mode-5      --mode-6");
	puts("     --mode-7     --mode-8\n");

	puts("Options:");
	puts("  -o --out        Output stdout to FILE and store in $CURRENT_DIR");
	puts("     --test       Enable random fill test mode");
	puts("     --mass-test  Enable Unstoppable test mode");
	puts("                  -------------------------------------------------------------");
	puts("                  | To use this mode, you MUST copy the 'Mass_Test' program   |");
	puts("                  |   to the same directory as the 'Matrix'                   |");
	puts("                  | This mode will create 'Mass Test.txt' in $CURRENT_DIR     |");
	puts("                  |   that store all the Command Line stdout                  |");
	puts("                  | You MUST close the Command Line window mannually when     |");
	puts("                  |   you need to stop this process                           |");
    puts("                  | After finishing the process, you should delete the file   |");
    puts("                  |  'RobotBegin' mannually in $CURRENT_DIR                   |");
	puts("                  -------------------------------------------------------------\n");;
	puts("Examples:");
	puts("  Matrix --help");
	puts("  Matrix -c");
	puts("  Matrix -c -o");
	puts("  Matrix --mode-8 --test");
	puts("  Matrix --mode-5 --mass-test");
}

void Show_MODE_Band(char MODE)
{
	switch (MODE)
	{
	case '1':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                        ---- MODE 1 Determinant ----                          |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '2':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                       ---- MODE 2 Adjoint Matrix ----                        |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '3':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                       ---- MODE 3 Inverse Matrix ----                        |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '4':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                    ---- MODE 4 Matrix Multiplication ----                    |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '5':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                       ---- MODE 5 Row Echelon Form ----                      |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '6':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                      ---- MODE 6 Row Canonical Form ----                     |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '7':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                       ---- MODE 7 Linear Equations ----                      |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	case '8':
	{
		puts("--------------------------------------------------------------------------------");
		puts("|                   ---- MODE 8 Schmidt Orthogonalization ----                 |");
		puts("--------------------------------------------------------------------------------");
		break;
	}
	}
}


int Show_Header_Source()
{
	FILE *fp = fopen(__FILE__, "r");
	return Show_File_Text(fp);
}

#ifdef UNIX
//-------------------------------------------- 针对UNIX系统，定义getch函数 --------------------------------------------
#include <termios.h>

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */

//char getch()
//{
//    return getch_(0);
//}

/* Read 1 character with echo */
char getche(void)
{
	return getch_(1);
}
//---------------------------------------------------------------------------------------------------------------------
#endif
