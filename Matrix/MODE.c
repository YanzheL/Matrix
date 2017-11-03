//
//  MODE.c
//  Matrix
//
//  Created by LI YANZHE on 30/11/2016.
//  Copyright © 2017 Yanzhe Lee. All rights reserved.
//

/*
 This file implements core functions of each mode
 */

#include "Matrix.h"

 //计算行列式的值
double Determinant(double **Matrix, int n)
{
	double result = 1;
	//    Determinant_Row_Echelon_Form(Matrix, n);
	Row_Echelon_Form(Matrix, n, n, 1);
	//    Show_Matrix(Matrix, 1, 1, n, n, 1);
	for (int i = 0; i < n; ++i)
		result *= Matrix[i][i];
	return result;
}

//求伴随矩阵
double** Adjoint_Matrix(double **Matrix, int m, int n)
{
	double **Result_Matrix = Create_Matrix(m, n, "");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			Result_Matrix[i][j] = pow(-1, i + j)*Mirror(Matrix, i, j, m, n);
		}
	}
	return Transpose_Matrix(Result_Matrix, m, n);                                          //最后需要求转置矩阵才能得到最后的伴随矩阵
}

//行阶梯形变换
double Row_Echelon_Form(double **Matrix, int m, int n, int DeterminantMODE)
{
	int noZeroRowCount = 0;
	double coefficientOfEntireDeterminant = 1;
	if (Check_Zero_Matrix(Matrix, m, n) == -1)return 0;
	for (int column = 0; column < n && column < m; ++column) {
		//        printf("\n\n-------------------------- Column %d Begin ------------------------\n",column+1);
		// 将第column列从上到下化为1,1,...,1,...,0,0,...,0
		for (int rowToBeExchanged = column; rowToBeExchanged <= m - 2; ++rowToBeExchanged) {
			for (int j = rowToBeExchanged; j < m; ++j) {
				if (Matrix[rowToBeExchanged][column]) {
					double k = (double)1 / (Matrix[rowToBeExchanged][column]);                           // k等于第row_to_be_changed行第一个元素的倒数
					Scalar_Multiplication(k, Matrix, rowToBeExchanged, m, n);
					if (DeterminantMODE == 1)
						coefficientOfEntireDeterminant *= 1 / k;
				}
				// 从上到下找出首列元素不为零的行
				if (Matrix[j][column]) {
					double k = (double)1 / (Matrix[j][column]);                                             // k等于第j行第一个元素的倒数
					Scalar_Multiplication(k, Matrix, j, m, n);                                      // k数乘第j行，使第j行第一个元素化为1
					if (DeterminantMODE == 1)
						coefficientOfEntireDeterminant *= 1 / k;
					if (Matrix[rowToBeExchanged][column] == 0) {
						Row_Exchange(Matrix, j, rowToBeExchanged, n);                           // 将第j行与第row_to_be_changed行交换
						if (DeterminantMODE == 1)
							coefficientOfEntireDeterminant *= (-1);
					}
					//printf("------------------------- Row %d <--> Row %d -----------------------\n",j+1,row_to_be_exchanged+1);
					//Show_Matrix(Determinant, 1,1,n, n,1);
				}
			}
		}

		noZeroRowCount = Find_No_Zero_Row(Matrix, column, m) + 1;

		//printf("------------------ Column %d No Zero Row Count = %d ----------------\n",column+1,no_zero_row_count);
		//Show_Matrix(Determinant, 1,1,n, n,1);
		for (int r = column + 1; r < noZeroRowCount; ++r) {
			Row_Add(Matrix, r, column, n, 1);
		}

		//		puts("------------------------ Add Finish ------------------------------");
		//        Show_Matrix(Matrix, 1, 1, m, n, 1);

		noZeroRowCount = 0;

		//		printf("------------------------- Column %d Finish ------------------------\n\n",column+1);
	}
	//    printf("--------------------------------------- Fix Before ---------------------------------------\n\n");
	//    Show_Matrix(Matrix, 1, 1, m, n, 1);
	//    puts("--------------------------------------- Fix Begin ----------------------------------------");

	int problemColumn = 0;
	if (Check_Echelon(Matrix, m, n))
		problemColumn = Check_Echelon(Matrix, m, n);                    //检查是否已经化为行阶梯

	//    printf("Problem column = %d\n",problemColumn+1);
	if (problemColumn) {
		double last_2_LeadingCoefficient, last_1_LeadingCoefficient;
		int lastNoZeroRow = m - 1;
		for (int v = m - 1; v >= 0; --v) {
			if (Find_Leading_Column(Matrix, v, n)) {
				lastNoZeroRow = v;
				break;
			}
		}

		last_2_LeadingCoefficient = Matrix[lastNoZeroRow - 1][problemColumn + 1];
		last_1_LeadingCoefficient = Matrix[lastNoZeroRow][problemColumn + 1];
		Scalar_Multiplication(1 / last_2_LeadingCoefficient, Matrix, lastNoZeroRow - 1, m, n);                         //先把倒数第二行首个非零元化为1
		if (DeterminantMODE == 1)
			coefficientOfEntireDeterminant *= last_2_LeadingCoefficient;
		Scalar_Multiplication(1 / last_1_LeadingCoefficient, Matrix, lastNoZeroRow, m, n);                         //再把倒数第一行首个非零元化为1
		if (DeterminantMODE == 1)
			coefficientOfEntireDeterminant *= last_1_LeadingCoefficient;
		Row_Add(Matrix, lastNoZeroRow, lastNoZeroRow - 1, n, 1);                                                                 //倒数第一行减去倒数第二行，使成为阶梯
	}
	//    puts("-------------------------------------- Row Echelon Finish ---------------------------------------");
	if (DeterminantMODE == 1)
		Scalar_Multiplication(coefficientOfEntireDeterminant, Matrix, 0, m, n);
	Approximate(Matrix, m, n, 6);
	//    Show_Matrix(Matrix, 1, 1, m, n, 1);
	return coefficientOfEntireDeterminant;
}

//行最简形变换
int Row_Canonical_Form(double **Matrix, int m, int n)
{
	int lastNoZeroRow = m - 1;
	if (Row_Echelon_Form(Matrix, m, n, 0) == 0)
        return 0;
	for (int i = m - 1; i >= 0; --i) {
		int returnValueOf_Find_Leading_Column = Find_Leading_Column(Matrix, i, n);
		if (returnValueOf_Find_Leading_Column)
			lastNoZeroRow = i;
		
		double rowLastCoefficient = Matrix[lastNoZeroRow][Find_Leading_Column(Matrix, lastNoZeroRow, n)];
		if (rowLastCoefficient)
			Scalar_Multiplication(1 / rowLastCoefficient, Matrix, lastNoZeroRow, m, n);
	}
	for (int row = 0; row < m - 1; ++row) {
		for (int i = row + 1; i < m; ++i) {
			//            row_row_leading_column=Find_Leading_Column(Matrix, row, n);
			int row_i_LeadingColumn = Find_Leading_Column(Matrix, i, n);
			double row_row_SecondCoefficient = Matrix[row][row_i_LeadingColumn];
			if (row_row_SecondCoefficient != 0 && Matrix[i][row_i_LeadingColumn] != 0) {
				//printf("\n---------- Row %d ----> i %d Prepare---------------\n",row+1,i+1);
				//Show_Matrix(AB, m, n,1);
				Scalar_Multiplication(1 / row_row_SecondCoefficient, Matrix, row, m, n);
				//printf("---------- Row %d ----> i %d Scalared--------------\n",row+1,i+1);
				//Show_Matrix(AB, m, n,1);
				Row_Add(Matrix, row, i, n, 1);
				//printf("---------- Row %d ----> i %d Added ----------------\n\n",row+1,i+1);
				//Show_Matrix(AB, m, n,1);
			}
		}
		//}
		double row_row_LeadingCoefficient = Matrix[row][Find_Leading_Column(Matrix, row, n)];
		if (row_row_LeadingCoefficient)
			Scalar_Multiplication(1 / row_row_LeadingCoefficient, Matrix, row, m, n);
	}
	//puts("----------------------- Row Canonical Finish -----------------------");
	//Show_Matrix(AB, m, n,1);
	return 1;
}



void Build_Solution_Matrix(double **AB, double **Solution_Matrix, int m, int n, int n_of_Solution_Matrix, int rankOf_A)
{
	//	puts("---------------------------------------- Raw ------------------------------------------");
	//	Show_Matrix(AB, 1, 1, m, n+1, 1);

	for (int i = 0; i < n; ++i) {
		Solution_Matrix[i][0] = 1;
	}
	int basicColumnCount = rankOf_A;

	double **basic_column_array = Create_Matrix(1, basicColumnCount, "Basic Column Array");
	for (int i = 0; i < basicColumnCount; ++i) {
		basic_column_array[0][i] = Find_Leading_Column(AB, i, n);            //找出基础未知量所在的列号，存入数组
	}
	//	puts("---------------------------------------- Basic Array ------------------------------------------");
	//	Show_Matrix(basic_column_array, 1,1,1, basicColumnCount, 1);

	double **non_basic_column_array = Create_Matrix(1, n - rankOf_A, "Non Basic Column Array");
	double *pointer = non_basic_column_array[0];

	for (int i = 0; i < n; ++i) {
		if (exists(basic_column_array[0], basicColumnCount, i) == -1) {
			*pointer = i;
			pointer++;
		}
	}

	//	puts("----------------------------------------- Non Basic -------------------------------------------");
	//	Show_Matrix(non_basic_column_array, 1, 1,1,n-rankOf_A, 1);

	for (int i = 0; i < n - rankOf_A; ++i) {
		int xi = non_basic_column_array[0][i];
		Solution_Matrix[xi][i + 1] = 1;
	}

	for (int i = 0; i < rankOf_A; ++i) {
		int xi = basic_column_array[0][i];
		int correspondRow = 0;
		for (int row = 0; row < m; ++row) {
			if (xi == Find_Leading_Column(AB, row, n)) {
				correspondRow = row;
				break;
			}
		}
		double *p = Solution_Matrix[xi] + 1;
		for (int j = 0; j < n - rankOf_A; ++j) {
			int nxi = non_basic_column_array[0][j];
			if (nxi > xi) {
				*p = -AB[correspondRow][nxi];
			}
			++p;
		}
		*p = AB[correspondRow][n];
	}
	//	puts("------------------------------------- Solution Matrix ----------------------------------------");
	//	Show_Matrix(Solution_Matrix,1,1,n,n_of_Solution_Matrix, 1);
	Free_Matrix(basic_column_array, 1);
	Free_Matrix(non_basic_column_array, 1);
}

int Reverse_Matrix(double **Matrix, int n)
{
	if (Find_Rank(Matrix, n, n) != n)
		return 0;		//如果方阵不满秩，则行列式为0
	else {
		double **Combined_Matrix = Create_Matrix(n, 2 * n, "Combined Matrix");
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < 2 * n; ++j) {
				if (j < n)
					Combined_Matrix[i][j] = Matrix[i][j];
				else
					Combined_Matrix[i][j] = (i == (j - n)) ? 1 : 0;
			}
		}
		Row_Canonical_Form(Combined_Matrix, n, 2 * n);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				Matrix[i][j] = Combined_Matrix[i][j + n];
			}
		}
		Free_Matrix(Combined_Matrix, n);
	}
	return 1;
}

double** Schmidt_Orthogonalization(double **Matrix, int m, int n)
{
	//    double ***alpha=(double***)calloc(n, sizeof(double**));	//alpha[]中每一个元素都是一个列矩阵
	double ***alpha = Column_Vector_Extract(Matrix, m, n);
	//    for (i=0; i<=n-1; ++i)
	//        alpha[i]=Create_Matrix(m, 1, "Beta");
	double ***beta = (double***)calloc(n, sizeof(double**));		//beta[]中每一个元素都是一个列矩阵,用于存储上一次正交化得到的beta列向量
	for (int i = 0; i < n; ++i)
		beta[i] = Create_Matrix(m, 1, "Beta");

	for (int i = 0; i < m; ++i)
		beta[0][i][0] = alpha[0][i][0];								//赋初始值beta1 = alpha1

	for (int i = 1; i < n; ++i) {
		double **sum = Create_Matrix(m, 1, "");                     //sum得到正交化公式中后面项的和向量
		for (int x = 0; x < i; ++x) {
			double **temp = Create_Matrix(m, 1, "");

			for (int row = 0; row < m; ++row) {
				temp[row][0] = beta[i - x - 1][row][0];				//复制Beta[i-1]的列向量到temp中
			}

			//            printf("------------------------------- temp[%d] -------------------------------\n",i-1);
			//            Show_Matrix(temp, 1, 1,m, 1, 1);
			//            printf("------------------------------- Beta[%d] -------------------------------\n",i-x-1);
			//            Show_Matrix(beta[i-x-1], 1, 1, m, 1, 1);

			double numerator = Scalar_Product(alpha[i], beta[i - x - 1], m);   //可能没有free
			double dominator = Scalar_Product(beta[i - x - 1], beta[i - x - 1], m);

			double coefficient = numerator / dominator;
			double **transpose_temp = Transpose_Matrix(temp, m, 1);        //转置成行向量再数乘每个元素
			Scalar_Multiplication(coefficient, transpose_temp, 0, 1, m);
			temp = Transpose_Matrix(transpose_temp, 1, m);                 //恢复temp向量
			//            printf("------------------------------- temp-T[%d] -------------------------------\n",i-x-1);
			//            Show_Matrix(temp, 1, 1,m, 1, 1);
			sum = Matrix_Sum(sum, temp, m, 1, 0);
			//            printf("------------------------------- Sum[%d] -------------------------------\n",x);
			//            Show_Matrix(sum, 1, 1, m, 1, 1);
			Free_Matrix(temp, m);
			Free_Matrix(transpose_temp, 1);
		}
		beta[i] = Matrix_Sum(alpha[i], sum, m, 1, 1);
		Free_Matrix(sum, m);
	}

	double **Result_Matrix = Create_Matrix(m, n, "Schmidt Result");

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			Result_Matrix[i][j] = beta[j][i][0];
		}
	}

	for (int i = 0; i < n - 1; ++i)
		Free_Matrix(alpha[i], m);

	for (int i = 1; i < n - 1; ++i)
		Free_Matrix(beta[i], m);

	return Result_Matrix;
}

int Matrix_Multiplication(double **A, double **B, double **Result_Matrix, int m_A, int n_A, int m_B, int n_B)
{
	double **Temp = Create_Matrix(m_A, n_B, "Multi Temp");
	for (int i = 0; i < m_A; ++i) {
		for (int j = 0; j < n_B; ++j) {
			for (int r = 0; r < n_A; ++r) {
				Temp[i][j] += A[i][r] * B[r][j];
			}
			Result_Matrix[i][j] = Temp[i][j];
		}
	}
	Free_Matrix(Temp, m_A);
	return 1;
}
