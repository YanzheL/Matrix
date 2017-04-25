//
//  Matrix.h
//  Matrix
//
//  Created by LI YANZHE on 02/02/2017.
//  Copyright © 2017 Yanzhe Lee. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h

#define WINDOWS
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

//--------------------------------------------------------- MODE ------------------------------------------------------------
//MODE 1
double Determinant(double **Matrix, int n);
//MODE 2
double** Adjoint_Matrix(double **Matrix, int m, int n);
//MODE 3
int Reverse_Matrix(double **Matrix, int n);
//MODE 4
int Matrix_Multiplication(double **A, double **B, double **Result_Matrix, int m_A, int n_A, int m_B, int n_B);
//MODE 5
double Row_Echelon_Form(double **Matrix, int m, int n, int DeterminantMODE);
//MODE 6
int Row_Canonical_Form(double **Matrix, int m, int n);
//MODE 7
void Build_Solution_Matrix(double **AB, double **Solution_Matrix, int m, int n, int n_of_Solution_Matrix, int rankOf_A);
//MODE 8
double** Schmidt_Orthogonalization(double **Matrix, int m, int n);
//--------------------------------------------------------- MODE ------------------------------------------------------------



//--------------------------------------------------------- Tools -----------------------------------------------------------
char* TextFile2Char(FILE *fp);
char* GetFileExactPath(const char* argvTemp, char *fileName);
sConfig Read_Config(const char* programPath);
void Config_Fill_Matrix(double **Matrix, sConfig configSource, int TYPE);
void User_Input_Matrix(double **Matrix, int m, int n, char *TYPE);
void Test_Scanf(struct Characteristic_of_Matrix *Recive_mn_for_Test, int structElementNumber, int m_rand_min, int mRandMax, int nRandMin, int nRandMax);
int Check_Echelon(double **Matrix, int m, int n);
int Find_Rank(double **Matrix, int m, int n);
int Check_Linear_Equation_Solution_Existance(double **AB, int m, int n);
double** Transpose_Matrix(double **Matrix, int m, int n);
double Mirror(double **Matrix, int row, int column, int m, int n);
double Scalar_Product(double **Vector1, double **Vector2, int n);
double** Vector_Normalization(double **Matrix, int m, int n);
double** Matrix_Sum(double **A, double **B, int m, int n, int MODE);
char** CommandList();
int Check_No_Command(int argc, const char** argv);
int Check_Known_Options(int argc, const char** argv, int *invalidContinueFlag);
int Check_Option_Order(int argc, const char** argv, char *str1, unsigned long limN1, char *str2, unsigned long limN2);
void strrpl(char* src, char ch1, char ch2, unsigned long length);
//--------------------------------------------------------- Tools -----------------------------------------------------------



//-------------------------------------------------- Independent Functions --------------------------------------------------
//用于清空scanf缓冲区
void Safe_Flush(FILE *fp);
int Show_File_Text(FILE *fp);
double** Create_Matrix(int m, int n, char *DISPLAY);
void Free_Matrix(double **Matrix, int m);
//数乘运算 用k数乘第r行
void Scalar_Multiplication(double k, double **Matrix, int r, int m, int n);
//用于向屏幕打印整个矩阵
void Show_Matrix(double **Matrix, int startRow, int startColumn, int endRow, int endColumn, int displayFlag);
//近似运算
void Approximate(double **Matrix, int m, int n, int index);
//行交换，r1行与r2行交换
void Row_Exchange(double **Matrix, int r1, int r2, int n);
//行加减运算
void Row_Add(double **Matrix, int r1, int r2, int n, int flag);

int Find_No_Zero_Row(double **Matrix, int column, int m);
int Find_Leading_Column(double **Matrix, int row, int n);
int Check_Zero_Matrix(double **Matrix, int m, int n);
void Rand_Fill(double **Matrix, int m, int n, int MIN, int MAX, int MODE);
double*** Column_Vector_Extract(double **Matrix, int m, int n);
void Column_Vector_Refill(double ***vector_System, double **Matrix, int m, int n);
void Show_Index_Page();
void Show_Menu_Page();
void Show_Help_Page();
int Show_Header_Source();
void Show_MODE_Band(char MODE);
int Check_Option(int argc, const char **argv, char *option);

#ifdef UNIX
//---------------------------------- 针对UNIX系统，定义getch函数 ----------------------------------
#include <termios.h>
static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo);

/* Restore old terminal i/o settings */
void resetTermios(void);

/* Read 1 character - echo defines echo mode */
char getch_(int echo);

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
