//
//  Tools.c
//  Matrix
//
//  Created by LI YANZHE on 30/11/2016.
//  Copyright © 2017 Yanzhe Lee. All rights reserved.
//
#include "Matrix.h"

char* TextFile2Char(FILE *fp)
{
	long int length;
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	//        printf("length = %ld\n",length);

	char *fstr = (char*)calloc(length, sizeof(char));
	if (!fstr) {
		perror("TextFile2Char char*fstr");
		exit(0);
	}
	char C;
	for (int i = 0;; ++i) {
		C = fgetc(fp);
		if (C == EOF)break;
		fstr[i] = C;
	}
	return fstr;
}

char* GetFileExactPath(const char* argvTemp, const char *fileName)
{
	char *filePath = (char*)calloc(strlen(argvTemp) - strlen("Matrix") + strlen(fileName), sizeof(char));
	if (filePath) {
		filePath = strncpy(filePath, argvTemp, strlen(argvTemp) - strlen("Matrix"));
		strcat(filePath, fileName);
	}
	else
		exit(0);
	return filePath;
}

sConfig Read_Config(const char* programPath)
{
	sConfig readResult;

#ifdef GET_CURRENT_PATH_MODE
	char *configPath = GetFileExactPath(argv[0], CONFIG_FILE_NAME);
	FILE *fp = fopen(configPath, "rt");
#else
	FILE *fp = fopen(CONFIG_FILE_NAME, "rt");
#endif

	if (!fp) {
		perror("Config open error");
		exit(0);
	}
	else {
		int i;
		char *fstr = TextFile2Char(fp);
		//        printf("%s\n",fstr);

		cJSON * root = cJSON_Parse(fstr);
		cJSON *Elements_One = cJSON_GetObjectItem(root, "Matrix_One");
		cJSON *Elements_Two = cJSON_GetObjectItem(root, "Matrix_Two");

		readResult.getMODE = cJSON_GetObjectItem(root, "MODE")->valueint;
		readResult.getTestFlag = cJSON_GetObjectItem(root, "TEST_FLAG")->valueint;
		readResult.getM_One = cJSON_GetObjectItem(root, "m_One")->valueint;
		readResult.getN_One = cJSON_GetObjectItem(root, "n_One")->valueint;
		if (Elements_Two) {
			readResult.getM_Two = cJSON_GetObjectItem(root, "m_Two")->valueint;
			readResult.getN_Two = cJSON_GetObjectItem(root, "n_Two")->valueint;
		}
		readResult.extraOption = cJSON_GetObjectItem(root, "Extra_Option")->valuestring[0];
		readResult.getElements_One = (double*)calloc(readResult.getM_One*readResult.getN_One, sizeof(double));
		readResult.getElements_Two = (double*)calloc(readResult.getM_One*readResult.getN_One, sizeof(double));
		i = 0;
		for (int row = 0; row < readResult.getM_One; ++row) {
			for (int column = 0; column < readResult.getN_One; ++column, ++i) {
				readResult.getElements_One[i] = cJSON_GetArrayItem(cJSON_GetArrayItem(Elements_One, row), column)->valuedouble;
			}
		}

		i = 0;
		if (Elements_Two != NULL)
			for (int row = 0; row < readResult.getM_Two; ++row) {
				for (int column = 0; column < readResult.getN_Two; ++column, ++i) {
					readResult.getElements_Two[i] = cJSON_GetArrayItem(cJSON_GetArrayItem(Elements_Two, row), column)->valuedouble;
				}
			}
		free(fstr);
	}
	fclose(fp);
	return readResult;
}

char* Result2JSON(const sMatrix rawResult, const unsigned formatFlag)
{

	char* jsonData = "";
	cJSON *root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "MODE", cJSON_CreateNumber(rawResult.mode));
	if (rawResult.mode != 7 || (rawResult.mode == 7 && rawResult.sluExistFlag != 0)) {
		cJSON_AddItemToObject(root, "Row_Number", cJSON_CreateNumber(rawResult.m));
		cJSON_AddItemToObject(root, "Column_Number", cJSON_CreateNumber(rawResult.n));
	}
	switch (rawResult.mode)
	{
	case 1:
		cJSON_AddItemToObject(root, "Determinant_Value", cJSON_CreateNumber(rawResult.value));
		break;
	case 7:
		if (rawResult.sluExistFlag) {
			if (!rawResult.homogeneousFlag) {
				double** ptcl_slv_arr = Create_Matrix(1, rawResult.m, "");
				//					Matrix_Copy(<#double **dst#>, <#double **src#>, <#int begin_r#>, <#int begin_c#>, <#int end_r#>, <#int end_c#>)

				Matrix_Copy(ptcl_slv_arr, Transpose_Matrix(rawResult.content, rawResult.m, rawResult.n), rawResult.n, 1, rawResult.n, rawResult.m);

				cJSON_AddItemToObject(root, "Particular_Solution", cJSON_CreateDoubleArray(ptcl_slv_arr[0], rawResult.m));

				Free_Matrix(ptcl_slv_arr, 1);
			}
			else {
				if (rawResult.onlySluFlag) {
					cJSON_AddItemToObject(root, "Particular_Solution", cJSON_CreateString("ZERO"));
				}
				cJSON_AddItemToObject(root, "Homogeneous_Flag", cJSON_CreateBool(1));
			}
			cJSON *Fundmtl_Slu_Sys = cJSON_CreateArray();
			for (int i = 0; i < rawResult.n - 1; ++i) {
				cJSON_AddItemToArray(Fundmtl_Slu_Sys, cJSON_CreateDoubleArray(Transpose_Matrix(rawResult.content, rawResult.m, rawResult.n)[i], rawResult.m));
			}

			cJSON_AddItemToObject(root, "Fundamental_Solution_Sys", Fundmtl_Slu_Sys);
		}
		else
			cJSON_AddItemToObject(root, "Solution_Exist_Flag", cJSON_CreateBool(0));
		break;
	default:
		break;
	}

	if (rawResult.mode != 1)
	{
		cJSON *matrixContent = cJSON_CreateArray();

		for (int i = 0; i < rawResult.m; ++i) {
			cJSON_AddItemToArray(matrixContent, cJSON_CreateDoubleArray(rawResult.content[i], rawResult.n));
		}

		if (rawResult.mode != 7) {
			cJSON_AddItemToObject(root, "Matrix_Content", matrixContent);
		}
		else if (rawResult.sluExistFlag != 0)cJSON_AddItemToObject(root, "Solution_Matrix", matrixContent);
	}



	switch (formatFlag) {
	case 0:jsonData = cJSON_PrintUnformatted(root); break;
	case 1:jsonData = cJSON_Print(root); break;
	}

	return jsonData;

}

void Config_Fill_Matrix(double **Matrix, const sConfig configSource, const int TYPE)
{
	int num = 0;
	switch (TYPE) {
	case 1:
		for (int i = 0; i < configSource.getM_One; ++i) {
			for (int j = 0; j < configSource.getN_One; ++j, ++num) {
				Matrix[i][j] = configSource.getElements_One[num];
			}
		}
		break;

	case 2:
		for (int i = 0; i < configSource.getM_Two; ++i) {
			for (int j = 0; j < configSource.getN_Two; ++j, ++num) {
				Matrix[i][j] = configSource.getElements_Two[num];
			}
		}
		break;
	}
}

void User_Input_Matrix(double **Matrix, const int m, const int n, const char *TYPE, const int inputMode)
{
	Safe_Flush(stdin);
	switch (inputMode)
	{
	case 0:
		for (int i = 0; i <= m - 1; ++i) {
			printf("Please input row %d elements of%s Matrix : ", i + 1, TYPE);
			for (int j = 0; j <= n - 1; ++j) {
				if (scanf("%lf", &Matrix[i][j]) != 1) {
					puts("Input error");
					exit(1);
				}
				printf("\b");
			}
		}
		break;
	case 1:
		for (int j = 0; j < n; ++j) {
			printf("Please input the %d Vector Element : ", j + 1);
			for (int i = 0; i < m; ++i) {
				if (scanf("%lf", &Matrix[i][j]) != 1) {
					puts("Input error");
					exit(1);
				}
				printf("\b");
			}
		}
		break;

	default:
		break;
	}
}

void Test_Scanf(struct Characteristic_of_Matrix *Recive_mn_for_Test, const int structElementNumber, const int mRandMin, const int mRandMax, const int nRandMin, const int nRandMax)
{
	//	srand((unsigned)time(NULL));
	Recive_mn_for_Test[structElementNumber - 1].m = mRandMin + rand() % (mRandMax - mRandMin);       //测试需要
	Recive_mn_for_Test[structElementNumber - 1].n = nRandMin + rand() % (nRandMax - nRandMin);
}

int Check_Echelon(double **Matrix, const int m, const int n)                         //用于检查是否已化为行阶梯形
{
	for (int i = 0; i < n - 1; ++i) {
		int formerColumnNoZeroCount = Find_No_Zero_Row(Matrix, i, m) + 1;
		int nextColumnNoZeroCount = Find_No_Zero_Row(Matrix, i + 1, m) + 1;
		nextColumnNoZeroCount = (formerColumnNoZeroCount > nextColumnNoZeroCount) ? formerColumnNoZeroCount : nextColumnNoZeroCount;
		if (nextColumnNoZeroCount - formerColumnNoZeroCount > 1 && (nextColumnNoZeroCount != formerColumnNoZeroCount)) {                                                               //如果发现后一列的非零元个数减去前一列的非零元个数的差大于1，则没有化简完
			return i;
		}
		if (Find_No_Zero_Row(Matrix, i, m) == m - 2) break;             //如果发现第i列最后一个非零元正好在最后一行，则停止判断
	}
	return 0;
}

int Find_Rank(double **Matrix, const int m, const int n)
{
	double **Copy_Of_Matrix = Create_Matrix(m, n, "Copy of Matrix");
	int rank = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			Copy_Of_Matrix[i][j] = Matrix[i][j];                        //把传来的矩阵元素复制到新的Copy矩阵里，防止影响原矩阵
	}
	if (Row_Echelon_Form(Copy_Of_Matrix, m, n, 0) == 0)
		return 0;
	else {
		for (int i = m - 1; i >= 0; --i) {
			if (Find_Leading_Column(Copy_Of_Matrix, i, n)) {
				rank = i + 1;
				break;
			}
		}
	}
	Free_Matrix(Copy_Of_Matrix, m);
	return rank;
}

int Check_Linear_Equation_Solution_Existance(double **AB, const int m, const int n)
{
	int rank_A = Find_Rank(AB, m, n), rank_AB = Find_Rank(AB, m, n + 1);
	//	printf("Rank A = %d\nRank AB = %d\n", rank_A, rank_AB);
	if ((rank_A != rank_AB) || rank_A == 0)
		return 0;
	else
		return 1;
}

double** Transpose_Matrix(double **Matrix, const int m, const int n)
{
	double **Transpose_Matrix;
	Transpose_Matrix = Create_Matrix(n, m, "Transpose");
	for (int i = 0; i <= n - 1; ++i) {
		for (int j = 0; j <= m - 1; ++j)
			Transpose_Matrix[i][j] = Matrix[j][i];
	}
	return Transpose_Matrix;
}

double** Matrix_Sum(double **A, double **B, const int m, const int n, const int MODE)
{
	double **Result_Matrix = Create_Matrix(m, n, "Matrix Sum");
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			switch (MODE)
			{
			case 0:
				Result_Matrix[i][j] = A[i][j] + B[i][j];
				break;
			case 1:
				Result_Matrix[i][j] = A[i][j] - B[i][j];
				break;
			default:break;
			}
		}
	}
	return Result_Matrix;
}

//找出余子矩阵，并返回余子式的值
double Mirror(double **Matrix, const int row, const int column, const int m, const int n)
{
	double **Mirror_Matrix = Create_Matrix(m - 1, n - 1, "");
	double result;
	for (int i = 0; i < m - 1; ++i) {
		for (int j = 0; j < n - 1; ++j) {
			if (i < row&&j < column)                                      //通过跳过指定的行、列来创建余子矩阵
				Mirror_Matrix[i][j] = Matrix[i][j];
			else if (i < row&&j >= column)
				Mirror_Matrix[i][j] = Matrix[i][j + 1];
			else if (i >= row&&j < column)
				Mirror_Matrix[i][j] = Matrix[i + 1][j];
			else if (i >= row&&j >= column)
				Mirror_Matrix[i][j] = Matrix[i + 1][j + 1];
		}
	}
	result = Determinant(Mirror_Matrix, n - 1);
	Free_Matrix(Mirror_Matrix, m - 1);
	return result;
}

double Scalar_Product(double **Vector1, double **Vector2, const int n)
{
	double **Product_Matrix = Create_Matrix(1, 1, "Product Matrix");
	double **Vector1_Transpose = Transpose_Matrix(Vector1, n, 1);
	Matrix_Multiplication(Vector1_Transpose, Vector2, Product_Matrix, 1, n, n, 1);

	double product = Product_Matrix[0][0];
	Free_Matrix(Product_Matrix, 1);
	Free_Matrix(Vector1_Transpose, 1);
	return product;
}

double** Vector_Normalization(double **Matrix, const int m, const int n)
{
	double *product = (double*)calloc(n, sizeof(double));
	if (!product) {
		perror("Vector_Normalization product]");
		exit(1);
	}
	double **Result_Matrix = Create_Matrix(m, n, "");
	double ***vector_System = Column_Vector_Extract(Matrix, m, n);

	for (int i = 0; i < n; ++i) {
		product[i] = sqrt(Scalar_Product(vector_System[i], vector_System[i], m));
		if (product[i]) {
			double **temp_Tranpose = Transpose_Matrix(vector_System[i], m, 1);
			Scalar_Multiplication(1 / product[i], temp_Tranpose, 0, 1, m);
			vector_System[i] = Transpose_Matrix(temp_Tranpose, 1, m);
			Free_Matrix(temp_Tranpose, 1);
		}
	}

	Column_Vector_Refill(vector_System, Result_Matrix, m, n);

	free(product);
	for (int i = 0; i < n; ++i)
		Free_Matrix(vector_System[i], m);
	return Result_Matrix;
}

int Check_Option_Order(const int argc, const char** argv, const char *str1, const unsigned long limN1, const char *str2, const unsigned long limN2)
{
	int addr1 = 0, addr2 = 0;
	for (int i = 0; i < argc; ++i) {
		if (strncmp(argv[i], str1, limN1) == 0)
			addr1 = i;
		if (strncmp(argv[i], str2, limN2) == 0)
			addr2 = i;
	}
	if (addr1 == 0 || addr2 == 0) return 9;
	return (addr1 <= addr2) ? 0 : 1;
	//return 0为 顺序 str1 str2
	//return 1为 逆序 str2 str1
	//return 9为 其中之一不存在
}

char** CommandList()
{
	char **allOptions = (char**)calloc(MAX_OPTIONS, sizeof(char*));
	if (!allOptions) {
		perror("allOptions");
		exit(1);
	}
	allOptions[0] = "-c";
	for (int oNum = 1; oNum <= 8; ++oNum) {
		allOptions[oNum] = (char*)calloc(9, sizeof(char));
		if (allOptions[oNum] == NULL) {
			perror("allOptions[oNum]");
			exit(1);
		}
		strcpy(allOptions[oNum], "--mode-");
		char n[2] = "0";
		n[0] = (char)('0' + oNum);
		strcat(allOptions[oNum], n);

	}
	allOptions[9] = "--config";
	allOptions[10] = "-h";
	allOptions[11] = "--help";
	allOptions[12] = "--menu";
	allOptions[13] = "--server";
	//---------- Options ----------
	allOptions[14] = "-o";
	allOptions[15] = "--out";
	allOptions[16] = "--test";
	allOptions[17] = "--mass-test";

	return allOptions;
}

int Check_No_Command(const int argc, const char** argv)
{
	char **allOptions = CommandList();
	int existFlag = 0, commandNum = 0;
	for (int i = 1; i < argc; ++i) {
		//确定传入参数是否在已知列表中
		for (int j = 0; j < MAX_OPTIONS; ++j) {
			if (strcmp(argv[i], allOptions[j]) == 0) {
				existFlag = 1;
				break;
			}
		}

		if (existFlag) {
			for (int j = 0; j <= 13; ++j) {
				if (strcmp(argv[i], allOptions[j]) == 0) {
					commandNum++;
					break;
				}
			}
		}
		else
			return 9;
		existFlag = 0;
	}

	switch (commandNum)
	{
	case 0:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

int Check_Known_Options(const int argc, const char** argv, int *invalidContinueFlag)
{
	int invalidOptionFlag = 0;
	char **knownOptions = CommandList();
	int wrongOrderFlag = 0, problemOption = 0;
	if (argc >= 2) {
		invalidOptionFlag = argc - 1;
		for (int i = 1; i < argc; ++i) {
			for (int j = 14; j < MAX_OPTIONS; ++j) {
				if (Check_Option_Order(argc, argv, knownOptions[j], strlen(knownOptions[j]), "--mode-", 7) == 0) {
					wrongOrderFlag = 1;
					break;
				}
				for (int k = 10; k <= 13; ++k) {
					if (Check_Option_Order(argc, argv, knownOptions[j], strlen(knownOptions[j]), knownOptions[k], strlen(knownOptions[k])) == 0) {
						wrongOrderFlag = 1;
						break;
					}
				}
			}
			if (wrongOrderFlag == 0) {
				int ini = invalidOptionFlag;
				for (int oNum = 0; oNum < MAX_OPTIONS; ++oNum) {
					//                printf("argc = %d\nargv[%d] = %s\nknownOptions[%d] = %s\n",argc,i,argv[i],oNum,knownOptions[oNum]);
					if (strcmp(argv[i], knownOptions[oNum]) == 0) {
						//                    printf("Bingo\n");
						invalidOptionFlag--;
						break;
					}
				}
				if (invalidOptionFlag == ini) problemOption = i;
			}
		}

		int noCommandFlag = 0;

		if (argc > 2 && Check_No_Command(argc, argv) == 1) {
			printf("No command found; type '--help' for a list.\n");
			puts("Syntax: Matrix [Commands] [Options]");
			invalidOptionFlag = 1;
			noCommandFlag = 1;
		}

		if (invalidOptionFlag != 0 && noCommandFlag == 0) {
			*invalidContinueFlag = 1;
			if (wrongOrderFlag == 0)
				printf("Invalid option '%s'; type '--help' for a list.\n", argv[problemOption]);
			else
				printf("Invalid command order; type '--help' for a list.\n");
			puts("Syntax: Matrix [Commands] [Options]");
		}
	}
	return invalidOptionFlag;
}

void strrpl(char* src, const char ch1, const char ch2, const unsigned long length)		//用于替换字符，遇到ch1就换为ch2
{
	for (int i = 0; i < length - 1; ++i) {
		if (src[i] == ch1) src[i] = ch2;
	}
}

void Matrix_Copy(double **dst, double **src, const int begin_r, const int begin_c, const int end_r, const int end_c)
{
	for (int i = begin_r - 1; i < end_r; ++i) {
		for (int j = begin_c - 1; j < end_c; ++j) {
			dst[i - (begin_r - 1)][j - (begin_c - 1)] = src[i][j];
		}
	}
}
