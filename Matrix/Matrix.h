//
//  Matrix.h
//  Matrix
//
//  Created by LI YANZHE on 30/11/2016.
//  Copyright © 2017 Yanzhe Lee. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h

#define UNIX
#define M_RAND_MIN 20
#define M_RAND_MAX 50
#define N_RAND_MIN 5
#define N_RAND_MAX 9
#define MAX_OPTIONS 18
//#define GET_CURRENT_PATH_MODE
#define CONFIG_FILE_NAME "config.json"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifdef UNIX
#include <unistd.h>
#endif
#include "cJSON.h"

#ifdef WINDOWS
#include <io.h>
#include <conio.h>
#endif

struct Characteristic_of_Matrix
{
	char* Matrix_Name;
	int m;
	int n;
};

typedef struct Config
{
	int getMODE;
	int getTestFlag;
	char extraOption;
	int getM_One;
	int getN_One;
	int getM_Two;
	int getN_Two;
	double *getElements_One;
	double *getElements_Two;
}sConfig;

typedef struct sMatrix
{
	int mode;
	int m;
	int n;
	double value;
	double** content;

	int sluExistFlag;
	int homogeneousFlag;
	int onlySluFlag;

}sMatrix;

//--------------------------------------------------------- MODE ------------------------------------------------------------
//MODE 1
double Determinant(double **Matrix, const int n);
//MODE 2
double** Adjoint_Matrix(double **Matrix, const int m, const int n);
//MODE 3
int Reverse_Matrix(double **Matrix, const int n);
//MODE 4
int Matrix_Multiplication(double **A, double **B, double **Result_Matrix, const int m_A, const int n_A, const int m_B, const int n_B);
//MODE 5
double Row_Echelon_Form(double **Matrix, const int m, const int n, const int DeterminantMODE);
//MODE 6
int Row_Canonical_Form(double **Matrix, const int m, const int n);
//MODE 7
void Build_Solution_Matrix(double **AB, double **Solution_Matrix, const int m, const int n, const int n_of_Solution_Matrix, const int rankOf_A);
//MODE 8
double** Schmidt_Orthogonalization(double **Matrix, const int m, const int n);
//--------------------------------------------------------- MODE ------------------------------------------------------------



//--------------------------------------------------------- Tools -----------------------------------------------------------
char* TextFile2Char(FILE *fp);
char* GetFileExactPath(const char* argvTemp, const char *fileName);
sConfig Read_Config(const char* programPath);
char* Result2JSON(const sMatrix rawResult, const unsigned formatFlag);
void Config_Fill_Matrix(double **Matrix, const sConfig configSource, const int TYPE);
void User_Input_Matrix(double **Matrix, const int m, const int n, const char *TYPE, const int inputMode);
void Test_Scanf(struct Characteristic_of_Matrix *Recive_mn_for_Test, const int structElementNumber, const int m_rand_min, const int mRandMax, const int nRandMin, const int nRandMax);
int Check_Echelon(double **Matrix, const int m, const int n);
//求矩阵的秩
int Find_Rank(double **Matrix, const int m, const int n);
//判断线性方程组的解是否存在
int Check_Linear_Equation_Solution_Existance(double **AB, const int m, const int n);
double** Transpose_Matrix(double **Matrix, const int m, const int n);
//找出余子矩阵，并返回指定元素的余子式的值
double Mirror(double **Matrix, const int row, const int column, const int m, const int n);
//两向量的标量积
double Scalar_Product(double **Vector1, double **Vector2, const int n);
//向量组的标准化
double** Vector_Normalization(double **Matrix, const int m, const int n);
//求矩阵和
double** Matrix_Sum(double **A, double **B, const int m, const int n, const int MODE);
char** CommandList();
int Check_No_Command(const int argc, const char** argv);
int Check_Known_Options(const int argc, const char** argv, int *invalidContinueFlag);
int Check_Option_Order(const int argc, const char** argv, const char *str1, const unsigned long limN1, const char *str2, const unsigned long limN2);
void strrpl(char* src, const char ch1, const char ch2, const unsigned long length);

void Matrix_Copy(double **dst, double **src, const int begin_r, const int begin_c, const int end_r, const int end_c);
inline void swap(int* m, int* n)
{
	int tp = *m;
	*m = *n;
	*n = tp;
}
//--------------------------------------------------------- Tools -----------------------------------------------------------



//-------------------------------------------------- Independent Functions --------------------------------------------------
//用于清空scanf缓冲区
void Safe_Flush(FILE *fp);
int Show_File_Text(FILE *fp);
double** Create_Matrix(const int m, const int n, const char *DISPLAY);
void Free_Matrix(double **Matrix, const int m);
//数乘运算 用k数乘第r行
void Scalar_Multiplication(const double k, double **Matrix, const int r, const int m, const int n);
//用于向屏幕打印整个矩阵
void Show_Matrix(double **Matrix, const int startRow, const int startColumn, const int endRow, const int endColumn, const int displayFlag);
//近似运算
void Approximate(double **Matrix, const int m, const int n, const int index);
//行交换，r1行与r2行交换
void Row_Exchange(double **Matrix, const int r1, const int r2, const int n);
//行加减运算
void Row_Add(double **Matrix, const int r1, const int r2, const int n, const int flag);

//返回指定列首个非零元的行号
int Find_No_Zero_Row(double **Matrix, const int column, const int m);
//返回指定行首个非零元的列号
int Find_Leading_Column(double **Matrix, const int row, const int n);
//判断是否是零矩阵
int Check_Zero_Matrix(double **Matrix, const int m, const int n);
//随机数填充给定矩阵
void Rand_Fill(double **Matrix, const int m, const int n, const int MIN, const int MAX, const int MODE);
//从矩阵中提取列向量
double*** Column_Vector_Extract(double **Matrix, const int m, const int n);
//将一系列列向量重新构成一个矩阵
void Column_Vector_Refill(double ***vector_System, double **Matrix, const int m, const int n);
void Show_Index_Page();
void Show_Menu_Page();
void Show_Help_Page();
/*
	判断一个数是否在数组中
	@return:
		-1:	 Not exist
		>=0: The position of expected number
 */
inline int exists(double* array, const int n, const double val)
{
	double *p = array;
	for (int i = 0; i < n; ++i) {
		if (*p == val)
			return i;
		else
			++p;
	}
	return -1;
}
int Show_Header_Source();
void Show_MODE_Band(const char MODE);
int Check_Option(const int argc, const char **argv, const char *option);

#ifdef UNIX
//---------------------------------- 针对UNIX系统，定义getch函数 ----------------------------------
#include <termios.h>
static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(const int echo);

/* Restore old terminal i/o settings */
void resetTermios(void);

/* Read 1 character - echo defines echo mode */
char getch_(const int echo);

/* Read 1 character without echo */

//char getch()
//{
//    return getch_(0);
//}

/* Read 1 character with echo */
char getche(void);
//---------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------- Independent Functions --------------------------------------------------
#endif /* Matrix_h */
