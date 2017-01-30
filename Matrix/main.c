//
//  main.c
//  Matrix
//
//  Created by LI YANZHE on 30/11/2016.
//  Copyright © 2016 Yanzhe Lee. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Independent_Functions.h"

#define M_RAND_MIN 20
#define M_RAND_MAX 50
#define N_RAND_MIN 5
#define N_RAND_MAX 9

int Check_Echelon(double **Matrix,int m,int n)                                                   //用于检查是否已化为行阶梯形
{
    int formerColumnNoZeroCount,nextColumnNoZeroCount;
    int i;
    for (i=0; i<=n-2; i++)
    {
        formerColumnNoZeroCount=Find_No_Zero_Row(Matrix, i, m)+1;
        nextColumnNoZeroCount=Find_No_Zero_Row(Matrix, i+1, m)+1;
        nextColumnNoZeroCount=(formerColumnNoZeroCount>nextColumnNoZeroCount)?formerColumnNoZeroCount:nextColumnNoZeroCount;
        if(nextColumnNoZeroCount-formerColumnNoZeroCount>1&&(nextColumnNoZeroCount!=formerColumnNoZeroCount))
        {                                                                  //如果发现后一列的非零元个数减去前一列的非零元个数的差大于1，则没有化简完
            return i;
        }
        if (Find_No_Zero_Row(Matrix, i, m)==m-2) break;                    //如果发现第i列最后一个非零元正好在最后一行，则停止判断
    }
    return 0;
}

void User_Input_Matrix(double **Matrix,int m,int n,char *TYPE)
{
    int i,j;
    Safe_Flush(stdin);
    for (i=0; i<=m-1; i++)                                                                       //用户输入矩阵
    {
        printf("Please input row %d elements of%s Matrix : ",i+1,TYPE);
        for (j=0; j<=n-1; j++)
        {
            scanf("%lf",&Matrix[i][j]);
            printf("\b");
        }
    }
}

int Row_Echelon_Form(double **Matrix,int m, int n,int DeterminantMODE)
{
    int r,j,column,rowToBeExchanged,noZeroRowCount=0;
    double k,coefficientOfEntireDeterminant=1;
    
    if (Check_Zero_Matrix(Matrix, m, n)==-1)return 0;
    for(column=0;column<=n-1&&column<=m-1;column++)
    {
        //        printf("\n\n-------------------------- Column %d Begin ------------------------\n",column+1);
        for (rowToBeExchanged=column;rowToBeExchanged<=m-2 ; rowToBeExchanged++)        // 将第column列从上到下化为1,1,...,1,...,0,0,...,0
        {
            
            for(j=rowToBeExchanged;j<=m-1;j++)
            {
                if(Matrix[rowToBeExchanged][column]!=0)
                {
                    k=(double)1/(Matrix[rowToBeExchanged][column]);                           // k等于第row_to_be_changed行第一个元素的倒数
                    Scalar_Multiplication(k, Matrix,rowToBeExchanged,m,n);
                    if (DeterminantMODE==1)
                        coefficientOfEntireDeterminant*=1/k;
                }
                if(Matrix[j][column]!=0)                                                         // 从上到下找出首列元素不为零的行
                {
                    k=(double)1/(Matrix[j][column]);                                             // k等于第j行第一个元素的倒数
                    Scalar_Multiplication(k, Matrix,j,m,n);                                      // k数乘第j行，使第j行第一个元素化为1
                    if (DeterminantMODE==1)
                        coefficientOfEntireDeterminant*=1/k;
                    if(Matrix[rowToBeExchanged][column]==0)
                    {
                        Row_Exchange(Matrix, j,rowToBeExchanged,n);                           // 将第j行与第row_to_be_changed行交换
                        if (DeterminantMODE==1)
                            coefficientOfEntireDeterminant*=(-1);
                    }
                    //printf("------------------------- Row %d <--> Row %d -----------------------\n",j+1,row_to_be_exchanged+1);
                    //Show_Matrix(Determinant, 1,1,n, n,1);
                }
            }
        }
        
        noZeroRowCount=Find_No_Zero_Row(Matrix, column, m)+1;
        
        //printf("------------------ Column %d No Zero Row Count = %d ----------------\n",column+1,no_zero_row_count);
        //Show_Matrix(Determinant, 1,1,n, n,1);
        for (r=column+1; r<=noZeroRowCount-1; r++)
        {
            Row_Add(Matrix, r,column,n,1);
        }
        
        //puts("------------------------ Add Finish ------------------------------");
        //Show_Matrix(AB, m, n,1);
        noZeroRowCount=0;
        
        //printf("------------------------- Column %d Finish ------------------------\n\n",column+1);
    }
    //    printf("--------------------------------------- Fix Before ---------------------------------------\n\n");
    //    if(n>9)Show_Matrix(Matrix, 1,n-9,n, n,1);
    //    else Show_Matrix(Matrix, 1,1,n, n,1);
    //    puts("--------------------------------------- Fix Begin ----------------------------------------");
    //
    int problemColumn=0;
    if(Check_Echelon(Matrix, m, n)!=0)problemColumn=Check_Echelon(Matrix, m, n);                    //检查是否已经化为行阶梯
    
    //    printf("Problem column = %d\n",problem_column+1);
    if(problemColumn!=0)
    {
        double last_2_LeadingCoefficient,last_1_LeadingCoefficient;
        last_2_LeadingCoefficient=Matrix[m-2][problemColumn+1];
        last_1_LeadingCoefficient=Matrix[m-1][problemColumn+1];
        Scalar_Multiplication(1/last_2_LeadingCoefficient, Matrix,m-2,m,n);                         //先把倒数第二行首个非零元化为1
        if (DeterminantMODE==1)
            coefficientOfEntireDeterminant*=last_2_LeadingCoefficient;
        Scalar_Multiplication(1/last_1_LeadingCoefficient, Matrix,m-1,m,n);                         //再把倒数第一行首个非零元化为1
        if (DeterminantMODE==1)
            coefficientOfEntireDeterminant*=last_1_LeadingCoefficient;
        Row_Add(Matrix,m-1,m-2,n,1);                                                                 //倒数第一行减去倒数第二行，使成为阶梯
    }
    //    puts("-------------------------------------- Row Echelon Finish ---------------------------------------");
    if (DeterminantMODE==1)
        Scalar_Multiplication(coefficientOfEntireDeterminant, Matrix, 0, m, n);
    Approximate(Matrix, m, n, 6);
    //    if(n>9)Show_Matrix(Matrix, 1,n-9,n, n,1);
    //    else Show_Matrix(Matrix, 1,1,n, n,1);
    
    
    return coefficientOfEntireDeterminant;
}


int Row_Canonical_Form(double **Matrix,int m, int n)
{
    int i,row,row_i_LeadingColumn,lastNoZeroRow=m-1,returnValueOf_Find_Leading_Column;
    //    int row_row_leading_column;
    double row_row_SecondCoefficient,row_row_LeadingCoefficient,rowLastCoefficient;
    if(Row_Echelon_Form(Matrix,m,n,0)==0)return 0;
    
    for (i=m-1; i>=0; i--)
    {
        returnValueOf_Find_Leading_Column=Find_Leading_Column(Matrix, i, n);
        if (returnValueOf_Find_Leading_Column!=0)
        {
            lastNoZeroRow=i;
        }
        rowLastCoefficient=Matrix[lastNoZeroRow][Find_Leading_Column(Matrix, lastNoZeroRow, n)];
        if (rowLastCoefficient!=0)Scalar_Multiplication(1/rowLastCoefficient, Matrix, lastNoZeroRow, m, n);
    }
    
    for (row=0; row<=m-2; row++)
    {
        for (i=row+1; i<=m-1; i++)
        {
            //            row_row_leading_column=Find_Leading_Column(Matrix, row, n);
            row_i_LeadingColumn=Find_Leading_Column(Matrix,i, n);
            row_row_SecondCoefficient=Matrix[row][row_i_LeadingColumn];
            if (row_row_SecondCoefficient!=0&&Matrix[i][row_i_LeadingColumn]!=0)
            {
                //printf("\n---------- Row %d ----> i %d Prepare---------------\n",row+1,i+1);
                //Show_Matrix(AB, m, n,1);
                Scalar_Multiplication(1/row_row_SecondCoefficient, Matrix, row, m, n);
                //printf("---------- Row %d ----> i %d Scalared--------------\n",row+1,i+1);
                //Show_Matrix(AB, m, n,1);
                Row_Add(Matrix, row, i, n, 1);
                //printf("---------- Row %d ----> i %d Added ----------------\n\n",row+1,i+1);
                //Show_Matrix(AB, m, n,1);
            }
        }
        //}
        row_row_LeadingCoefficient=Matrix[row][Find_Leading_Column(Matrix, row, n)];
        if(row_row_LeadingCoefficient!=0)Scalar_Multiplication(1/row_row_LeadingCoefficient, Matrix, row, m, n);
    }
    
    //puts("----------------------- Row Canonical Finish -----------------------");
    //Show_Matrix(AB, m, n,1);
    return 1;
}

int Matrix_Multiplication(double **A,double **B,double **Result_Matrix,int m_A,int n_A,int m_B,int n_B)
{
    int i,j,r;
    double **Temp=Create_Matrix(m_A, n_B, "Multi Temp");
    for (i=0; i<=m_A-1; i++)
    {
        for (j=0; j<=n_B-1; j++)
        {
            for (r=0; r<=n_A-1; r++)
            {
                Temp[i][j]+=A[i][r]*B[r][j];
            }
            Result_Matrix[i][j]=Temp[i][j];
        }
    }
    Free_Matrix(Temp, m_A);
    return 1;
}

int Find_Rank(double **Matrix,int m,int n)
{
    double **Copy_Of_Matrix=Create_Matrix(m,n,"Copy of Matrix");
    int i,j,rank=0;
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)
            Copy_Of_Matrix[i][j]=Matrix[i][j];                        //把传来的矩阵元素复制到新的Copy矩阵里，防止影响原矩阵
    }
    
    //    puts("Copy-------------------------------------------");
    //    Show_Matrix(Copy_Of_Matrix, 1, 1, m, n, 1);
    if(Row_Echelon_Form(Copy_Of_Matrix, m, n, 0)==0)return 0;
    else
    {
        for (i=m-1; i>=0; i--)
        {
            if (Find_Leading_Column(Copy_Of_Matrix, i, n)!=0)
            {
                rank=i+1;
                break;
            }
        }
    }
    Free_Matrix(Copy_Of_Matrix, m);
    return rank;
}

int Check_Linear_Equation_Solution_Existance(double **AB,int m,int n)
{
    int rank_A=Find_Rank(AB, m, n),rank_AB=Find_Rank(AB, m, n+1);
    printf("Rank A = %d\nRank AB = %d\n",rank_A,rank_AB);
    if((rank_A!=rank_AB)||rank_A==0)
        return 0;
    else return 1;
}

void Build_Solution_Matrix(double **AB,double **Solution_Matrix,int m,int n,int n_of_Solution_Matrix,int rankOf_A)
{
    int i,j,row_i_LeadingColumn=0;
    for (i=0; i<=n-1; i++)
    {
        Solution_Matrix[i][0]=1;
        if(i<=m-1)
            Solution_Matrix[i][n_of_Solution_Matrix-1]=AB[i][n];
        else
            Solution_Matrix[i][0]=1;
    }
    int basicColumnCount=rankOf_A;
    
    double **basic_column_array=Create_Matrix(1, basicColumnCount, "Basic Column Array");
    for (i=0; i<=basicColumnCount-1; i++)
    {
        basic_column_array[0][i]=Find_Leading_Column(AB, i, n);            //找出基础未知量所在的列号，存入数组
    }
    //    puts("---------------------------------------- Basic Array ------------------------------------------");
    //    Show_Matrix(basic_column_array, 1, basic_column_count, 1);
    //    Show_Matrix(basic_column_array, 1, 1,1,basic_column_count, 1);
    
    double **non_basic_column_array=Create_Matrix(1, n-rankOf_A, "Non Basic Column Array");
    double *pointer=&non_basic_column_array[0][0];
    
    for (i=0; i<=n-1; i++)
    {
        if (i!=basic_column_array[0][i])                                   //找出自由未知量所在的列号，存入数组
        {
            *pointer=i;
            pointer++;
        }
    }
    
    //    puts("----------------------------------------- Non Basic -------------------------------------------");
    //    Show_Matrix(non_basic_column_array, 1, n-rank_of_A, 1);
    //    Show_Matrix(non_basic_column_array, 1, 1,1,n-rank_of_A, 1);
    
    for (i=0; i<=n-1; i++)
    {
        if(i<=m-1)
            row_i_LeadingColumn=Find_Leading_Column(AB, i, n);
        
        if (i!=row_i_LeadingColumn)
        {
            for (j=0; j<=n-rankOf_A-1; j++)
            {
                if (i==non_basic_column_array[0][j])
                {
                    break;
                }
            }
            Solution_Matrix[i][j+1]=1;
        }
        else
        {
            j=0;
            if (i<=m-1)
            {
                for (j=0; j<=n-rankOf_A-1; j++)
                {
                    Solution_Matrix[i][j+1]=-AB[i][(int)non_basic_column_array[0][j]];
                }
            }
            else
            {
                Solution_Matrix[i][j+1]=1;
            }
            
        }
    }
    //    puts("------------------------------------------- Struct ----------------------------------------------");
    //    Show_Matrix(Solution_Matrix,1,1,n,n_of_Solution_Matrix, 1);
    Free_Matrix(basic_column_array, 1);
    Free_Matrix(non_basic_column_array, 1);
}

int Reverse_Matrix(double **Matrix,int n)
{
    int i,j;
    if (Find_Rank(Matrix,n,n)!=n)return 0;
    else
    {
        double **Combined_Matrix=Create_Matrix(n, 2*n, "Combined Matrix");
        for (i=0; i<=n-1; i++)
        {
            for (j=0; j<=2*n-1; j++)
            {
                if (j<=n-1)
                {
                    Combined_Matrix[i][j]=Matrix[i][j];
                }
                else
                {
                    Combined_Matrix[i][j]=(i==(j-n))?1:0;
                }
            }
        }
        Row_Canonical_Form(Combined_Matrix, n, 2*n);
        for (i=0; i<=n-1; i++)
        {
            for (j=0; j<=n-1; j++)
            {
                Matrix[i][j]=Combined_Matrix[i][j+n];
            }
        }
        
        Free_Matrix(Combined_Matrix, n);
    }
    return 1;
}

double Determinant(double **Matrix,int n)
{
    int i;
    double result=1;
    //    Determinant_Row_Echelon_Form(Matrix, n);
    Row_Echelon_Form(Matrix, n, n, 1);
    //    Show_Matrix(Matrix, 1, 1, n, n, 1);
    for (i=0; i<=n-1; i++)
        result*=Matrix[i][i];
    return result;
}

struct Characteristic_of_Matrix
{
    char* Matrix_Name;
    int m;
    int n;
};

void Test_Scanf(struct Characteristic_of_Matrix *Recive_mn_for_Test,int structElementNumber,int m_rand_min,int mRandMax,int nRandMin,int nRandMax)
{
    srand((unsigned)time(NULL));
    //srand((unsigned)20);
    Recive_mn_for_Test[structElementNumber-1].m=m_rand_min+rand()%(mRandMax-m_rand_min);       //测试需要
    Recive_mn_for_Test[structElementNumber-1].n=nRandMin+rand()%(nRandMax-nRandMin);
}

double** Transpose_Matrix(double **Matrix,int m,int n)
{
    int i=0,j=0;
    double **Transpose_Matrix;
    Transpose_Matrix=Create_Matrix(n, m, "Transpose");
    for (i=0; i<=n-1; i++)
    {
        for (j=0; j<=m-1; j++)
            Transpose_Matrix[i][j]=Matrix[j][i];
    }
    return Transpose_Matrix;
}

double** Matrix_Sum(double **A,double **B,int m,int n,int MODE)
{
    int i,j;
    double **Result_Matrix=Create_Matrix(m, n, "Matrix Sum");
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)
        {
            switch (MODE)
            {
                case 0:
                {
                    Result_Matrix[i][j]=A[i][j]+B[i][j];
                    break;
                }
                case 1:
                {
                    Result_Matrix[i][j]=A[i][j]-B[i][j];
                    break;
                }
                default:break;
            }
            
        }
    }
    return Result_Matrix;
}

double Scalar_Product(double **Vector1,double **Vector2,int n)
{
    double **Product_Matrix=Create_Matrix(1, 1, "Product Matrix");
    double **Vector1_Transpose=Transpose_Matrix(Vector1, n, 1);
    Matrix_Multiplication(Vector1_Transpose, Vector2, Product_Matrix, 1, n, n, 1);
    
    double product=Product_Matrix[0][0];
    Free_Matrix(Product_Matrix, 1);
    Free_Matrix(Vector1_Transpose, 1);
    return product;
}

double** Schmidt_Orthogonalization(double **Matrix,int m,int n)
{
    int i,j;
    double *bets_scalar_product=(double*)calloc(n-1, sizeof(double));
    
    double ***alpha=(double***)calloc(n, sizeof(double**));              //alpha[]中每一个元素都是一个列矩阵
    for (i=0; i<=n-1; i++)
        alpha[i]=Create_Matrix(m, 1, "Beta");
    double ***beta=(double***)calloc(n, sizeof(double**));               //beta[]中每一个元素都是一个列矩阵,用于存储上一次正交化得到的beta列向量
    for (i=0; i<=n-1; i++)
        beta[i]=Create_Matrix(m, 1, "Beta");
    
    int r,s;
    
    for (s=0; s<n; s++)
    {
        for (r=0; r<m; r++)
        {
            alpha[s][r][0]=Matrix[r][s];                                 //提取Matrix中的列向量到alpha[]，准备正交化
        }
        //        printf("------------------------------- alpha_s[%d] -------------------------------\n",s);
        //        Show_Matrix(alpha[s], 1, 1, m, 1, 1);
    }
    
    for (i=0; i<m; i++)
    {
        beta[0][i][0]=alpha[0][i][0];                                    //赋初始值beta1 = alpha1
    }
    
    int row=0,x=0;
    for (i=1; i<=n-1; i++)
    {
        double **sum=Create_Matrix(m, 1, "");                            //sum得到正交化公式中后面项的和向量
        for (x=0; x<i; x++)
        {
            double **temp=Create_Matrix(m, 1, "");
            
            for (row=0; row<m; row++)
                temp[row][0]=beta[i-x-1][row][0];                          //复制Beta[i-1]的列向量到temp中
            
            //            printf("------------------------------- temp[%d] -------------------------------\n",i-1);
            //            Show_Matrix(temp, 1, 1,m, 1, 1);
            //            printf("------------------------------- Beta[%d] -------------------------------\n",i-x-1);
            //            Show_Matrix(beta[i-x-1], 1, 1, m, 1, 1);
            
            double numerator=Scalar_Product(alpha[i], beta[i-x-1], m);   //可能没有free
            double dominator=Scalar_Product(beta[i-x-1], beta[i-x-1], m);
            
            double coefficient=numerator/dominator;
            double **transpose_temp=Transpose_Matrix(temp, m, 1);        //转置成行向量再数乘每个元素
            Scalar_Multiplication(coefficient,transpose_temp, 0, 1, m);
            temp=Transpose_Matrix(transpose_temp, 1, m);                 //恢复temp向量
            //            printf("------------------------------- temp-T[%d] -------------------------------\n",i-x-1);
            //            Show_Matrix(temp, 1, 1,m, 1, 1);
            
            sum=Matrix_Sum(sum, temp, m, 1, 0);
            
            //            printf("------------------------------- Sum[%d] -------------------------------\n",x);
            //            Show_Matrix(sum, 1, 1, m, 1, 1);
            
            Free_Matrix(temp, m);
            Free_Matrix(transpose_temp, 1);
        }
        beta[i]=Matrix_Sum(alpha[i], sum, m, 1, 1);
        Free_Matrix(sum, m);
    }
    
    double **Result_Matrix=Create_Matrix(m, n, "Schmidt Result");
    
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)
        {
            Result_Matrix[i][j]=beta[j][i][0];
        }
    }
    
    free(bets_scalar_product);
    for (i=0; i<n-1; i++)
        Free_Matrix(alpha[i], m);
    
    for (i=1; i<n-1; i++)
        Free_Matrix(beta[i], m);
    
    return Result_Matrix;
}

double Mirror(double **Matrix, int row, int column, int m,int n)                            //找出余子矩阵，并返回余子式的值
{
    double **Mirror_Matrix=Create_Matrix(m-1, n-1, "");
    int i,j;
    double result;
    for (i=0; i<=m-2; i++)
    {
        for (j=0; j<=n-2; j++)
        {
            if (i<row&&j<column)                                                            //通过跳过指定的行、列来创建余子矩阵
                Mirror_Matrix[i][j]=Matrix[i][j];
            else if(i<row&&j>=column)
                Mirror_Matrix[i][j]=Matrix[i][j+1];
            else if(i>=row&&j<column)
                Mirror_Matrix[i][j]=Matrix[i+1][j];
            else if(i>=row&&j>=column)
                Mirror_Matrix[i][j]=Matrix[i+1][j+1];
        }
    }
    result=Determinant(Mirror_Matrix, n-1);
    Free_Matrix(Mirror_Matrix, m-1);
    return result;
}

double** Adjoint_Matrix(double **Matrix,int m, int n)
{
    double **Result_Matrix=Create_Matrix(m, n, "");
    int i,j;
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)
        {
            Result_Matrix[i][j]=pow(-1, i+j)*Mirror(Matrix, i, j, m, n);
        }
    }
    return Transpose_Matrix(Result_Matrix, m, n);                                          //最后需要求转置矩阵才能得到最后的伴随矩阵
}

char TEST_FLAG='0';



//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

int main(int argc, const char * argv[])
{
    char MODE='0';
    int invalidOptionFlag=0;
    if (argc>=2&&strcmp(argv[1], "--mode-1")==0)
    {
        MODE='1';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                        ---- MODE 1 Determinant ----                          |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-2")==0)
    {
        MODE='2';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                       ---- MODE 2 Adjoint Matrix ----                        |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-3")==0)
    {
        MODE='3';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                       ---- MODE 3 Inverse Matrix ----                        |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-4")==0)
    {
        MODE='4';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                    ---- MODE 4 Matrix Multiplication ----                    |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-5")==0)
    {
        MODE='5';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                       ---- MODE 5 Row Echelon Form ----                      |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-6")==0)
    {
        MODE='6';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                      ---- MODE 6 Row Canonical Form ----                     |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-7")==0)
    {
        MODE='7';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                       ---- MODE 7 Linear Equations ----                      |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--mode-8")==0)
    {
        MODE='8';
        Show_Index_Page();
        puts("--------------------------------------------------------------------------------");
        puts("|                   ---- MODE 8 Schmidt Orthogonalization ----                 |");
        puts("--------------------------------------------------------------------------------");
    }
    else if (argc>=2&&strcmp(argv[1], "--menu")==0)
        Show_Menu_Page();
    else if(argc>=2&&strcmp(argv[1], "--help")==0)
    {
        Show_Index_Page();
        Show_Help_Page();
        invalidOptionFlag=1;
    }
    else if (argc==1||(argc==2&&strcmp(argv[1], "--test")==0))
    {
        Show_Index_Page();
        Show_Menu_Page();
        printf("Please choose mode number: ");
        scanf("%c",&MODE);
        while (MODE>'8'||MODE<'1')
        {
            printf("Unavailable Choice, please choose again: ");
            //        Safe_Flush(stdin);
            scanf("%c",&MODE);
        }
    }
    else if(argc>=2)
    {
        printf("invalid option '%s'; type '--help' for a list.\n",argv[argc-1]);
        puts("Syntax: Matrix [Commands] [options]");
        invalidOptionFlag=1;
    }
    
    if (argc>=2&&strcmp(argv[argc-1], "--test")==0)TEST_FLAG='1';
    
    if (argc==1)
    {
        printf("Press any key to test or press 0 to manually input\n");
        Safe_Flush(stdin);
        scanf("%c",&TEST_FLAG);
    }
    if (MODE=='1'||MODE=='2')
    {
        int n;
        if(TEST_FLAG!='0')
        {
            srand((unsigned)time(NULL));                                                    //测试需要 获取随机的m和n
            n=4+rand()%4;
        }
        else
        {
            printf("\nPlease input 'n' : ");
            scanf("%d",&n);
            puts("");
        }
        
        double **Matrix=Create_Matrix(n,n,"MODE 1");
        
        if (TEST_FLAG=='0')
            User_Input_Matrix(Matrix, n, n, "");
        else
            Rand_Fill(Matrix, n, n,-10,10,0);
        
        Approximate(Matrix, n, n, 6);
        puts("---------------------------------- Confirm Input -------------------------------");
        if(n>9)
            Show_Matrix(Matrix, 1,n-9,n, n,1);
        else
            Show_Matrix(Matrix, 1,1,n, n,1);
        puts("\n\n\n ---------------------------------- Result ------------------------------------");
        if(MODE=='1')
        {
            printf("Determinant Value = %lf\n",Determinant(Matrix, n));
            Free_Matrix(Matrix, n);
        }
        if (MODE=='2')
        {
            double **Adjoint=Adjoint_Matrix(Matrix, n, n);
            Approximate(Adjoint, n, n, 5);
            Show_Matrix(Adjoint, 1, 1, n, n, 1);
            Free_Matrix(Adjoint, n);
            Free_Matrix(Matrix, n);
        }
    }
    
    if (MODE=='5'||MODE=='6')
    {
        struct Characteristic_of_Matrix *Matrix_Description;
        Matrix_Description=(struct Characteristic_of_Matrix*)calloc(1,sizeof(struct Characteristic_of_Matrix));
        Matrix_Description[0].Matrix_Name="MODE 5 Input";
        
        if(TEST_FLAG!='0')
            Test_Scanf(Matrix_Description,1, M_RAND_MIN,M_RAND_MAX,N_RAND_MIN,N_RAND_MAX);
        else
        {
            printf("\nPlease input 'm' and 'n' : ");
            scanf("%d %d",&Matrix_Description[0].m,&Matrix_Description[0].n);
            puts("");
        }
        
        double **Matrix=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n,"MODE 2");
        
        if (TEST_FLAG=='0')
            User_Input_Matrix(Matrix, Matrix_Description[0].m, Matrix_Description[0].n, "");
        else
            Rand_Fill(Matrix, Matrix_Description[0].m, Matrix_Description[0].n,-10,10,0);
        
        Approximate(Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 6);
        
        puts("---------------------------------- Confirm Input -------------------------------");
        //Show_Matrix(AB, m, n+1,1);
        if(Matrix_Description[0].n>9)
            Show_Matrix(Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
        else
            Show_Matrix(Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
        
        puts("\n\n\n------------------------------------ Result ------------------------------------\n");
        if(MODE=='5')
        {
            if(Row_Echelon_Form(Matrix, Matrix_Description[0].m, Matrix_Description[0].n,0)==0)
                Show_Matrix(Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            Approximate(Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 5);
            if(Matrix_Description[0].n>9)
                Show_Matrix(Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            else
                Show_Matrix(Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            
            printf("Rank = %d\n",Find_Rank(Matrix, Matrix_Description[0].m, Matrix_Description[0].n));
            
            Free_Matrix(Matrix, Matrix_Description[0].m);
        }
        else if(MODE=='6')
        {
            if(Row_Canonical_Form(Matrix, Matrix_Description[0].m, Matrix_Description[0].n)==0)
                Show_Matrix(Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,0);
            Approximate(Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 5);
            if(Matrix_Description[0].n>9)
                Show_Matrix(Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            else
                Show_Matrix(Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            printf("Rank = %d\n",Find_Rank(Matrix, Matrix_Description[0].m, Matrix_Description[0].n));
            Free_Matrix(Matrix, Matrix_Description[0].m);
        }
    }
    
    if (MODE=='3')
    {
        int n;
        
        if(TEST_FLAG!='0')
        {
            srand((unsigned)time(NULL));                            //测试需要 获取随机的m和n
            n=4+rand()%4;
        }
        else
        {
            printf("\nPlease input 'n' : ");
            scanf("%d",&n);
            puts("");
        }
        
        double **Matrix=Create_Matrix(n,n,"MODE 5");
        
        if (TEST_FLAG=='0')
            User_Input_Matrix(Matrix, n, n, "");
        else
            Rand_Fill(Matrix, n, n,-10,50,0);
        
        Approximate(Matrix, n, n, 6);
        
        puts("---------------------------------- Confirm Input -------------------------------");
        if(n>9)
            Show_Matrix(Matrix, 1,n-9,n, n,1);
        else
            Show_Matrix(Matrix, 1,1,n, n,1);
        
        if(Reverse_Matrix(Matrix, n)==0)
        {
            printf("The Inverse Matrix doesn't Exist\n");
        }
        else
        {
            puts("\n\n\n------------------------------------ Result ------------------------------------");
            if(n>9)Show_Matrix(Matrix, 1,n-9,n, n,1);
            else Show_Matrix(Matrix, 1,1,n, n,1);
        }
        Free_Matrix(Matrix, n);
    }
    
    if (MODE=='4')
    {
        
        int i;
        
        struct Characteristic_of_Matrix *Matrix_Description;
        Matrix_Description=(struct Characteristic_of_Matrix*)calloc(2,sizeof(struct Characteristic_of_Matrix));
        Matrix_Description[0].Matrix_Name="MODE 4 A";
        Matrix_Description[1].Matrix_Name="MODE 4 B";
        
        
        if(TEST_FLAG!='0')
        {
            Test_Scanf(Matrix_Description,1, M_RAND_MIN,M_RAND_MAX,N_RAND_MIN,N_RAND_MAX);
            Test_Scanf(Matrix_Description,2, M_RAND_MIN,M_RAND_MAX,N_RAND_MIN,N_RAND_MAX);
            Matrix_Description[1].m=Matrix_Description[0].n;
        }
        else
        {
            for (i=0;; i++)
            {
                printf("\nPlease input 'm' and 'n' of A Matrix : ");
                scanf("%d %d",&Matrix_Description[0].m,&Matrix_Description[0].n);
                printf("Please input 'm' and 'n' of B Matrix : ");
                scanf("%d %d",&Matrix_Description[1].m,&Matrix_Description[1].n);
                if (Matrix_Description[0].n==Matrix_Description[1].m) break;
                else printf("A and B cannot be multiplied, Please input again: \n");
            }
        }
        
        int multi_times=1;
        if (Matrix_Description[0].m==Matrix_Description[1].n)
        {
            printf("\nIt seems like AB is a square matrix.\nSo input how many times you want to multipy in total: ");
            for (i=0;; i++)
            {
                scanf("%d",&multi_times);
                if (multi_times>=1) break;
                else printf("Wrong answer. Please input again: ");
            }
        }
        
        double **A=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n,Matrix_Description[0].Matrix_Name);
        double **B=Create_Matrix(Matrix_Description[1].m,Matrix_Description[1].n,Matrix_Description[1].Matrix_Name);
        double **Result_Matrix=Create_Matrix(Matrix_Description[0].m,Matrix_Description[1].n,"MODE 4 Result");
        
        if (TEST_FLAG=='0')
        {
            puts("");
            User_Input_Matrix(A, Matrix_Description[0].m, Matrix_Description[0].n, " A");
            puts("");
            User_Input_Matrix(B, Matrix_Description[1].m, Matrix_Description[1].n, " B");
        }
        else
        {
            Rand_Fill(A, Matrix_Description[0].m, Matrix_Description[0].n,50,500,0);
            Rand_Fill(B, Matrix_Description[1].m, Matrix_Description[1].n,50,500,0);
        }
        
        Approximate(A, Matrix_Description[0].m, Matrix_Description[0].n, 6);
        
        puts("---------------------------------- Confirm Input -------------------------------");
        if(Matrix_Description[1].n>14)
        {
            printf(" ----------------------------------- A %d X %d ---------------------------------\n",Matrix_Description[0].m,Matrix_Description[0].n);
            Show_Matrix(A, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            printf(" ----------------------------------- B %d X %d ---------------------------------\n",Matrix_Description[1].m,Matrix_Description[1].n);
            Show_Matrix(B, 1,Matrix_Description[1].n-9,Matrix_Description[1].m, Matrix_Description[1].n,1);
        }
        else
        {
            printf(" ----------------------------------- A %d X %d ---------------------------------\n",Matrix_Description[0].m,Matrix_Description[0].n);
            Show_Matrix(A, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            printf(" ----------------------------------- B %d X %d ---------------------------------\n",Matrix_Description[1].m,Matrix_Description[1].n);
            Show_Matrix(B, 1,1,Matrix_Description[1].m, Matrix_Description[1].n,1);
        }
        
        if(Matrix_Multiplication(A, B, Result_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, Matrix_Description[1].m, Matrix_Description[1].n)==0)
            Show_Matrix(Result_Matrix,1,1, Matrix_Description[0].m, Matrix_Description[1].n,0);
        
        for (i=1;i<=multi_times-1;i++)
        {
            Matrix_Multiplication(Result_Matrix, Result_Matrix, Result_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, Matrix_Description[1].m, Matrix_Description[1].n);
        }
        
        puts("\n\n\n------------------------------------ Result ------------------------------------\n");
        
        Approximate(Result_Matrix, Matrix_Description[0].m, Matrix_Description[1].n, 5);
        
        if(Matrix_Description[1].n>14)
        {
            printf(" ----------------------------------- A %d X %d ---------------------------------\n",Matrix_Description[0].m,Matrix_Description[0].n);
            Show_Matrix(A, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            printf(" ----------------------------------- B %d X %d ---------------------------------\n",Matrix_Description[1].m,Matrix_Description[1].n);
            Show_Matrix(B, 1,Matrix_Description[1].n-9,Matrix_Description[1].m, Matrix_Description[1].n,1);
        }
        else
        {
            printf(" ----------------------------------- A %d X %d ---------------------------------\n",Matrix_Description[0].m,Matrix_Description[0].n);
            Show_Matrix(A, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            printf(" ----------------------------------- B %d X %d ---------------------------------\n",Matrix_Description[1].m,Matrix_Description[1].n);
            Show_Matrix(B, 1,1,Matrix_Description[1].m, Matrix_Description[1].n,1);
            printf(" ----------------------------------- A B %d X %d -------------------------------\n",Matrix_Description[0].m,Matrix_Description[1].n);
            Show_Matrix(Result_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[1].n,1);
        }
        
        Free_Matrix(A, Matrix_Description[0].m);
        Free_Matrix(B, Matrix_Description[1].m);
        Free_Matrix(Result_Matrix, Matrix_Description[0].m);
    }
    
    if (MODE=='7')
    {
        TEST_FLAG='0';
        
        struct Characteristic_of_Matrix *Matrix_Description;
        Matrix_Description=(struct Characteristic_of_Matrix*)calloc(1,sizeof(struct Characteristic_of_Matrix));
        Matrix_Description[0].Matrix_Name="MODE 5 Input";
        
        if(TEST_FLAG!='0')
        {
            Test_Scanf(Matrix_Description,1, M_RAND_MIN,M_RAND_MAX,N_RAND_MIN,N_RAND_MAX);
        }
        else
        {
            printf("\nPlease input 'm' and 'n' : ");
            scanf("%d %d",&Matrix_Description[0].m,&Matrix_Description[0].n);
            puts("");
        }
        
        double **AB=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n+1,"MODE 4 AB");
        
        if (TEST_FLAG=='0')
            User_Input_Matrix(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, " Augmented");
        else
            Rand_Fill(AB, Matrix_Description[0].m, Matrix_Description[0].n+1,50,1000,0);
        
        Approximate(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, 6);
        
        puts("---------------------------------- Confirm Input -------------------------------");
        //Show_Matrix(AB, m, n+1,1);
        if(Matrix_Description[0].n>9)
            Show_Matrix(AB, 1,Matrix_Description[0].n+1-9,Matrix_Description[0].m, Matrix_Description[0].n+1,1);
        else
            Show_Matrix(AB, 1,1,Matrix_Description[0].m, Matrix_Description[0].n+1,1);
        
        if(Check_Linear_Equation_Solution_Existance(AB, Matrix_Description[0].m, Matrix_Description[0].n)==0)
        {
            printf("This Linear Equation System does not have a solution\n");
            return 0;
        }
        
        //if(Row_Echelon_Form(AB,m,n+1)==0)Show_Matrix(AB, m, n+1,0);
        if(Row_Canonical_Form(AB, Matrix_Description[0].m, Matrix_Description[0].n+1)==0)
            Show_Matrix(AB, 1,1,Matrix_Description[0].m, Matrix_Description[0].n+1,0);
        
        Approximate(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, 6);
        
        int rank_of_A=Find_Rank(AB, Matrix_Description[0].m, Matrix_Description[0].n),n_of_Solution_Matrix=Matrix_Description[0].n-rank_of_A+2;
        double **Solution_Matrix=Create_Matrix(Matrix_Description[0].n,n_of_Solution_Matrix,"Solution Matrix");
        Build_Solution_Matrix(AB, Solution_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, n_of_Solution_Matrix, rank_of_A);
        
        puts("\n\n------------------------------------ Result ------------------------------------");
        
        Approximate(Solution_Matrix, Matrix_Description[0].n, n_of_Solution_Matrix, 5);
        
        int Homogeneous_Flag=Find_No_Zero_Row(Solution_Matrix, n_of_Solution_Matrix-1, Matrix_Description[0].n);
        
        if (Homogeneous_Flag!=0)
        {
            puts("\n\n\n------------------------------ Particular Solution -----------------------------");
            Show_Matrix(Solution_Matrix, 1,n_of_Solution_Matrix,Matrix_Description[0].n, n_of_Solution_Matrix, 1);
            
        }
        if (Matrix_Description[0].n-rank_of_A<=0)
        {
            printf("This Linear Equation Systems only exist one solution");
            if (Homogeneous_Flag==0)
                puts(" = ZERO\n");
        }
        else
        {
            puts("\n\n\n-------------------------- Fundamental Solution Systems ------------------------");
            Show_Matrix(Solution_Matrix, 1,2,Matrix_Description[0].n, n_of_Solution_Matrix-1, 1);
        }
        Free_Matrix(AB, Matrix_Description[0].m);
        Free_Matrix(Solution_Matrix, Matrix_Description[0].n);
    }
    
    if (MODE=='8')
    {
        struct Characteristic_of_Matrix *Matrix_Description;
        Matrix_Description=(struct Characteristic_of_Matrix*)calloc(1,sizeof(struct Characteristic_of_Matrix));
        Matrix_Description[0].Matrix_Name="MODE 5 Input";
        
        if(TEST_FLAG!='0')
            Test_Scanf(Matrix_Description,1, 4,8,3,9);
        else
        {
            printf("\nPlease input 'm' and 'n' : ");
            scanf("%d %d",&Matrix_Description[0].m,&Matrix_Description[0].n);
            puts("");
        }
        
        double **Matrix=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n,"MODE 8");
        
        if (TEST_FLAG=='0')
            User_Input_Matrix(Matrix, Matrix_Description[0].m, Matrix_Description[0].n, "");
        else
            Rand_Fill(Matrix, Matrix_Description[0].m, Matrix_Description[0].n,-10,20,0);
        
        Approximate(Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 6);
        
        puts("---------------------------------- Confirm Input -------------------------------");
        if(Matrix_Description[0].n>9)
            Show_Matrix(Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
        else
            Show_Matrix(Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
        
        puts("\n\n\n------------------------------------ Result ------------------------------------");
        double **Result_Matrix=Schmidt_Orthogonalization(Matrix, Matrix_Description[0].m, Matrix_Description[0].n);
        Approximate(Result_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 5);
        
        if(Matrix_Description[0].n>9)
            Show_Matrix(Result_Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
        else
            Show_Matrix(Result_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
    }
    
    if(invalidOptionFlag==0)
    {
        Safe_Flush(stdin);
        puts("\nDo you want to run again? (Press 0 to exit)");
        char flag;
        scanf("%c",&flag);
        if(flag!='0')
        {
            Safe_Flush(stdin);
            if ((argc>=2&&strcmp(argv[argc-1], "--test")==0))main(argc ,argv);
            else main(1, argv);
        }
    }
    
    return 0;
}
