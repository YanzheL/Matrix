//
//  Independent_Functions.h
//  Matrix
//
//  Created by LI YANZHE on 14/12/2016.
//  Copyright © 2016 Yanzhe Lee. All rights reserved.
//

#ifndef Independent_Functions_h
#define Independent_Functions_h

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

//用于清空scanf缓冲区
void Safe_Flush(FILE *fp);
int Show_File_Text(FILE *fp);
double** Create_Matrix(int m,int n,char *DISPLAY);
void Free_Matrix(double **Matrix,int m);

//数乘运算 用k数乘第r行
void Scalar_Multiplication(double k, double **Matrix,int r,int m,int n);

//用于向屏幕打印整个矩阵
void Show_Matrix(double **Matrix,int startRow,int startColumn,int endRow,int endColumn,int displayFlag);

//近似运算
void Approximate(double **Matrix,int m,int n,int index);

//行交换，r1行与r2行交换
void Row_Exchange(double **Matrix,int r1,int r2,int n);

//行加减运算
void Row_Add(double **Matrix,int r1,int r2,int n,int flag);

int Find_No_Zero_Row(double **Matrix,int column,int m);
int Find_Leading_Column(double **Matrix,int row,int n);
int Check_Zero_Matrix(double **Matrix,int m,int n);
void Rand_Fill(double **Matrix,int m,int n,int MIN,int MAX,int MODE);
double*** Column_Vector_Extract(double **Matrix,int m,int n);
void Column_Vector_Refill(double ***vector_System,double **Matrix,int m,int n);
void Show_Index_Page();
void Show_Menu_Page();
void Show_Help_Page();
int Show_Header_Source();

//-------------------------------------------- 针对OSX系统，定义getch函数 --------------------------------------------
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
//---------------------------------------------------------------------------------------------------------------------

#endif /* Independent_Functions_h */
