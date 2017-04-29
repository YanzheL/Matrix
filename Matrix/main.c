//
//  main.c
//  Matrix
//
//  Created by LI YANZHE on 30/11/2016.
//  Copyright © 2016 Yanzhe Lee. All rights reserved.
//
#include "Matrix.h"
static char TEST_FLAG = '0';
static char MODE = '0';

unsigned CONTINUE_FLAG=0;

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
int main(int argc, const char * argv[])
{
	do
	{
		CONTINUE_FLAG=0;
		struct timespec acuTime;
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &acuTime);                                                       //提供ns级的随机数种子
		srand((unsigned)acuTime.tv_nsec);
		
		int invalidOptionFlag = 0;
		//    int helpFlag = 0;
		unsigned massFlag = 0;
		unsigned configMode = 0;
		unsigned outputMode = 0;
		int invalidContinueFlag = 0;
		unsigned serverMode=0;
		char *outputFileName;
		
		invalidOptionFlag = Check_Known_Options(argc, argv, &invalidContinueFlag);
		if (invalidOptionFlag)return 0;
		
		sConfig receiveCfg = { 0,0,'n',0,0,0,0,NULL,NULL };														//结构体初始化
		if (Check_Option(argc, argv, "-c") != 0 || Check_Option(argc, argv, "--config") != 0)
		{
			configMode = 1;
			if (Check_Option(argc, argv, "--server") != 0 )
				serverMode = 1;
		}
		if (Check_Option(argc, argv, "--mass-test"))
		{
			massFlag = 1;
			TEST_FLAG = '1';
		}
		if (invalidOptionFlag == 0 && massFlag == 0)
		{
#ifdef UNIX
			switch (system("clear"))
			{
				case 127:
					perror("System('clear') return 127");
					exit(1);
					break;
				case -1:
					perror("System('clear') return -1");
					exit(1);
					break;
				default:
					break;
			}
#endif
#ifdef WINDOWS
			switch (system("cls"))
			{
				case 127:
					perror("System('cls') return 127");
					exit(1);
					break;
				case -1:
					perror("System('cls') return -1");
					exit(1);
					break;
				default:
					break;
			}
#endif
		}
		
		if (Check_Option(argc, argv, "-o") != 0 || Check_Option(argc, argv, "--out") != 0)
		{
			if (configMode == 0)
			{
				printf("\nOutput Mode is only available in Config Mode. Bypassing...\n\n");
				return 0;
			}
			else
			{
#ifdef UNIX
				outputMode = 1;
				puts("\nOutput success\n");
#else
				switch (system("cls"))
				{
					case 127:
						perror("System('cls') return 127");
						exit(1);
						break;
					case -1:
						perror("System('cls') return -1");
						exit(1);
						break;
					default:
						break;
				}
				puts("\nOutput mode is only available on UNIX platforms. Bypassing...");
				return 0;
#endif // UNIX
			}
		}
		if (Check_Option(argc, argv, "--test")) TEST_FLAG = '1';
		
		if (configMode == 1)
		{
			invalidContinueFlag = 1;
			receiveCfg = Read_Config(argv[0]);
			MODE = (char)(48 + receiveCfg.getMODE);
			TEST_FLAG = (char)(48 + receiveCfg.getTestFlag);
		}
		
		if (outputMode == 1)
		{
			time_t timeRaw;
			time(&timeRaw);
			outputFileName = ctime(&timeRaw);
			char *outTemp = (char*)calloc(strlen("[MODE 0]OUTPUT") + strlen(outputFileName) + strlen(".txt"), sizeof(char));
			strcpy(outTemp, "[MODE  ]");
			outTemp[strlen("[MODE ")] = MODE;
			strcat(outTemp, "OUTPUT ");
			strcat(outTemp, outputFileName);
			outTemp[strlen(outTemp) - 1] = ' ';
			strcat(outTemp, ".txt");
			outputFileName = outTemp;
			strrpl(outputFileName, ' ', '_', strlen(outputFileName));
			if (configMode == 1)
			{
				if (freopen(outputFileName, "w", stdout) == NULL)
				{
					perror("freopen error");
					exit(1);
				}
			}
		}
		
		if (argc >= 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
		{
			//        helpFlag = 1;
			Show_Index_Page();
			Show_Help_Page();
			invalidContinueFlag = 1;
		}
		if (argc >= 2 && strcmp(argv[1], "--menu") == 0)
		{
			invalidContinueFlag = 1;
			Show_Menu_Page();
		}
		
		if ((argc >= 2 && strcmp(argv[1], "--mode-1") == 0) || MODE == '1')
		{
			MODE = '1';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-2") == 0) || MODE == '2')
		{
			MODE = '2';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-3") == 0) || MODE == '3')
		{
			MODE = '3';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-4") == 0) || MODE == '4')
		{
			MODE = '4';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-5") == 0) || MODE == '5')
		{
			MODE = '5';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-6") == 0) || MODE == '6')
		{
			MODE = '6';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-7") == 0) || MODE == '7')
		{
			MODE = '7';
			if (serverMode==0) Show_Index_Page();
		}
		else if ((argc >= 2 && strcmp(argv[1], "--mode-8") == 0) || MODE == '8')
		{
			MODE = '8';
			if (serverMode==0) Show_Index_Page();
		}
		if ((argc == 1||((argc == 2)&&(strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "--mass-test") == 0))) && configMode == 0)
		{
			Show_Index_Page();
			Show_Menu_Page();
			printf("Please choose mode number: ");
			if (scanf("%c", &MODE) != 1)
			{
				puts("Input error");
				exit(1);
			}
			fflush(stdin);
			while ((MODE > '8' || MODE < '1') && MODE != 'c'&&MODE != 'q')
			{
				printf("\r");
				printf("Unavailable Choice, please choose again:");
				fflush(stdin);
				if (scanf("%c", &MODE) != 1)
				{
					puts("Input error");
					exit(1);
				}
			}
		}
		
		if (MODE == 'q')return 0;
		
		if (MODE == 'c')
		{
			configMode = 1;
			invalidOptionFlag = 1;
			receiveCfg = Read_Config(argv[0]);
			MODE = (char)(48 + receiveCfg.getMODE);
			TEST_FLAG = (char)(48 + receiveCfg.getTestFlag);
		}
		
		if (serverMode==0) Show_MODE_Band(MODE);
		
		//    if (argc>=2&&strcmp(argv[argc-1], "--test")==0)TEST_FLAG='1';
		
		if (argc == 1 && configMode == 0)
		{
			printf("Press any key to test or press 0 to manually input\n");
			Safe_Flush(stdin);
			if (scanf("%c", &TEST_FLAG) != 1)
			{
				puts("Input error");
				exit(1);
			}
			//        printf("TEST_FLAG = %c\n",TEST_FLAG);
			fflush(stdin);
		}
		
		
		//-------------------------------------- 进入运算 --------------------------------------
		
		sMatrix Calculate_Result={0,0,0,0,NULL,1,0,0};
		Calculate_Result.mode=(int)(MODE-'0');
		
		if (MODE == '1' || MODE == '2' || MODE == '3')
		{
			int n = 1;
			if (TEST_FLAG != '0')
			{
				//			srand((unsigned)time(NULL));                                                    //测试需要 获取随机的m和n
				n = 4 + rand() % 4;
			}
			else if (configMode == 0)
			{
				printf("\nPlease input 'n' : ");
				if (scanf("%d", &n) != 1)
				{
					puts("Input error");
					exit(1);
				}
				puts("");
			}
			else if (configMode == 1)
			{
				if (receiveCfg.getM_One != receiveCfg.getN_One)
				{
					puts("Your input Matrix is not a Square Matrix");
					exit(1);
				}
				else n = receiveCfg.getN_One;
			}
			
			double **Matrix = Create_Matrix(n, n, "MODE 1&2&3");
			if (TEST_FLAG == '0')
			{
				if (configMode == 0)
					User_Input_Matrix(Matrix, n, n, "",0);
				else
					Config_Fill_Matrix(Matrix, receiveCfg, 1);
			}
			else
				Rand_Fill(Matrix, n, n, -10, 10, 0);
			
			Approximate(Matrix, n, n, 6);
			
			if (serverMode==0)
			{
				puts("\n--------------------------------- Confirm Input --------------------------------\n");
				if (n > 9)
					Show_Matrix(Matrix, 1, n - 9, n, n, 1);
				else
					Show_Matrix(Matrix, 1, 1, n, n, 1);
			}
			
			Calculate_Result.m=n;
			Calculate_Result.n=n;
			
			if (MODE == '1')
			{
				Calculate_Result.content=Matrix;
				Calculate_Result.value=Determinant(Calculate_Result.content, n);
			}
			if (MODE == '2')
			{
				Calculate_Result.content = Adjoint_Matrix(Matrix, n, n);
				Approximate(Calculate_Result.content, n, n, 5);
				Free_Matrix(Matrix, n);
			}
			if (MODE == '3')
			{
				Calculate_Result.content=Matrix;
				if (Reverse_Matrix(Calculate_Result.content, n) == 0)
					printf("The Inverse Matrix doesn't Exist\n");
			}
		}
		
		if (MODE == '4')
		{
			int i;
			
			struct Characteristic_of_Matrix *Matrix_Description;
			Matrix_Description = (struct Characteristic_of_Matrix*)calloc(2, sizeof(struct Characteristic_of_Matrix));
			if (Matrix_Description == NULL)
			{
				perror("Matrix_Description MODE 4");
				exit(1);
			}
			Matrix_Description[0].Matrix_Name = "MODE 4 A";
			Matrix_Description[1].Matrix_Name = "MODE 4 B";
			
			if (TEST_FLAG != '0')
			{
				Test_Scanf(Matrix_Description, 1, M_RAND_MIN, M_RAND_MAX + rand() % 500, N_RAND_MIN, N_RAND_MAX + rand() % 500);
				Test_Scanf(Matrix_Description, 2, M_RAND_MIN, M_RAND_MAX, 3 + rand() % 3, N_RAND_MAX + rand() % 500);
				Matrix_Description[1].m = Matrix_Description[0].n;
			}
			else if (configMode == 0)
			{
				for (i = 0;; ++i)
				{
					printf("\nPlease input 'm' and 'n' of A Matrix : ");
					if (scanf("%d %d", &Matrix_Description[0].m, &Matrix_Description[0].n) != 2)
					{
						puts("Input error");
						exit(1);
					}
					printf("Please input 'm' and 'n' of B Matrix : ");
					if (scanf("%d %d", &Matrix_Description[1].m, &Matrix_Description[1].n) != 2)
					{
						puts("Input error");
						exit(1);
					}
					if (Matrix_Description[0].n == Matrix_Description[1].m) break;
					else printf("A and B cannot be multiplied, Please input again: \n");
				}
			}
			else if (configMode == 1)
			{
				Matrix_Description[0].m = receiveCfg.getM_One;
				Matrix_Description[0].n = receiveCfg.getN_One;
				Matrix_Description[1].m = receiveCfg.getM_Two;
				Matrix_Description[1].n = receiveCfg.getN_Two;
			}
			
			int multi_times = 1;
			int multiFlag = 0;
			if (Matrix_Description[0].m == Matrix_Description[1].n)
			{
				multiFlag = 1;
				if (configMode == 0)
				{
					printf("\nIt seems like AB is a square matrix.\nSo input how many times you want to multipy in total: ");
					for (i = 0;; ++i)
					{
						if (scanf("%d", &multi_times) != 1)
						{
							puts("Input error");
							exit(1);
						}
						if (multi_times >= 1) break;
						else printf("Wrong answer. Please input again: ");
					}
				}
				else
				{
					multi_times = (int)(receiveCfg.extraOption) - 48;
				}
			}
			
			double **A = Create_Matrix(Matrix_Description[0].m, Matrix_Description[0].n, Matrix_Description[0].Matrix_Name);
			double **B = Create_Matrix(Matrix_Description[1].m, Matrix_Description[1].n, Matrix_Description[1].Matrix_Name);
			Calculate_Result.m=Matrix_Description[0].m;
			Calculate_Result.n=Matrix_Description[1].n;
			Calculate_Result.content = Create_Matrix(Calculate_Result.m, Calculate_Result.n, "MODE 4 Result");
			
			if (TEST_FLAG == '0')
			{
				if (configMode == 0)
				{
					puts("");
					User_Input_Matrix(A, Matrix_Description[0].m, Matrix_Description[0].n, " A",0);
					puts("");
					User_Input_Matrix(B, Matrix_Description[1].m, Matrix_Description[1].n, " B",0);
				}
				else
				{
					Config_Fill_Matrix(A, receiveCfg, 1);
					Config_Fill_Matrix(B, receiveCfg, 2);
				}
			}
			else
			{
				Rand_Fill(A, Matrix_Description[0].m, Matrix_Description[0].n, -250, 500, 0);
				Rand_Fill(B, Matrix_Description[1].m, Matrix_Description[1].n, -250, 500, 0);
			}
			
			
			Approximate(A, Matrix_Description[0].m, Matrix_Description[0].n, 6);
			
			if (serverMode==0)
			{
				puts("\n--------------------------------- Confirm Input --------------------------------\n");
				if (Matrix_Description[0].n > 8 || Matrix_Description[1].n > 8)
				{
					printf(" --------------------------------- A %d X %d --------------------------------\n", Matrix_Description[0].m, Matrix_Description[0].n);
					if (Matrix_Description[0].n > 8)
					{
						printf("  ------------------------------- Last 8 Columns ---------------------------- \n");
						Show_Matrix(A, 1, Matrix_Description[0].n - 7, Matrix_Description[0].m, Matrix_Description[0].n, 1);
					}
					else Show_Matrix(A, 1, 1, Matrix_Description[0].m, Matrix_Description[0].n, 1);
					printf(" --------------------------------- B %d X %d --------------------------------\n", Matrix_Description[1].m, Matrix_Description[1].n);
					if (Matrix_Description[1].n > 8)
					{
						printf("  ------------------------------- Last 8 Columns ---------------------------- \n");
						Show_Matrix(B, 1, Matrix_Description[1].n - 7, Matrix_Description[1].m, Matrix_Description[1].n, 1);
					}
					else Show_Matrix(B, 1, 1, Matrix_Description[1].m, Matrix_Description[1].n, 1);
				}
				else
				{
					printf(" --------------------------------- A %d X %d --------------------------------\n", Matrix_Description[0].m, Matrix_Description[0].n);
					Show_Matrix(A, 1, 1, Matrix_Description[0].m, Matrix_Description[0].n, 1);
					printf(" --------------------------------- B %d X %d --------------------------------\n", Matrix_Description[1].m, Matrix_Description[1].n);
					Show_Matrix(B, 1, 1, Matrix_Description[1].m, Matrix_Description[1].n, 1);
				}
			}
			
			if (Matrix_Multiplication(A, B, Calculate_Result.content, Matrix_Description[0].m, Matrix_Description[0].n, Matrix_Description[1].m, Matrix_Description[1].n) == 0&&serverMode==0)
				Show_Matrix(Calculate_Result.content, 1, 1, Calculate_Result.m, Calculate_Result.n, 0);
			
			
			if (multiFlag == 1)
			{
				if (serverMode==0)
				{
					printf("\nMulti Times = %d", multi_times);
				}
				for (i = 1; i <= multi_times - 1; ++i)
				{
					double **Temp_Result = Create_Matrix(Matrix_Description[0].m, Matrix_Description[1].n, "");
					Matrix_Multiplication(Calculate_Result.content, Calculate_Result.content, Temp_Result, Matrix_Description[0].m, Matrix_Description[0].m, Matrix_Description[0].m, Matrix_Description[0].m);
					
					Free_Matrix(Calculate_Result.content, Calculate_Result.m);
					
					Calculate_Result.content = Temp_Result;
				}
			}
			
			Approximate(Calculate_Result.content, Matrix_Description[0].m, Matrix_Description[1].n, 5);
			
			
			Free_Matrix(A, Matrix_Description[0].m);
			Free_Matrix(B, Matrix_Description[1].m);
		}
		
		if (MODE == '5' || MODE == '6' || MODE == '8')
		{
			struct Characteristic_of_Matrix *Matrix_Description;
			Matrix_Description = (struct Characteristic_of_Matrix*)calloc(1, sizeof(struct Characteristic_of_Matrix));
			if (Matrix_Description == NULL)
			{
				perror("Matrix_Description MODE 5&6&8");
				exit(1);
			}
			Matrix_Description[0].Matrix_Name = "MODE 5&6&8 Input";
			
			if (TEST_FLAG != '0')
			{
				Test_Scanf(Matrix_Description, 1, M_RAND_MIN, M_RAND_MAX, N_RAND_MIN, N_RAND_MAX);
				
			}
			else if (configMode == 0)
			{
				if (MODE=='8')
					printf("\nPlease input the Dimension of a Vector and the Number of Vectors : ");
				else printf("\nPlease input 'm' and 'n' : ");
				if (scanf("%d %d", &Matrix_Description[0].m, &Matrix_Description[0].n) != 2)
				{
					puts("Input error");
					exit(1);
				}
				puts("");
			}
			else if (configMode == 1)
			{
				Matrix_Description[0].m = receiveCfg.getM_One;
				Matrix_Description[0].n = receiveCfg.getN_One;
			}
			sMatrix Input_Matrix={(int)(MODE-'0'),Matrix_Description[0].m,Matrix_Description[0].n,0,NULL,1,0,0};
			
			
			Input_Matrix.content = Create_Matrix(Matrix_Description[0].m, Matrix_Description[0].n, "MODE 5&6&8");
			
			if (TEST_FLAG == '0')
			{
				if (configMode == 0)
				{
					if (MODE=='8') {
						User_Input_Matrix(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n, "",1);
					}
					else User_Input_Matrix(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n, "",0);
				}
				else
				{
					Config_Fill_Matrix(Input_Matrix.content, receiveCfg, 1);
					double **tp2free=Input_Matrix.content;
					Input_Matrix.content=Transpose_Matrix(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n);
					Free_Matrix(tp2free, Input_Matrix.m);
					swap(&Input_Matrix.m,&Input_Matrix.n);
				}
			}
			else Rand_Fill(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n, -10, 10, 0);
			
			Approximate(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n, 6);
			
			if (MODE == '8')
			{
				//            printf("stdin = %d\n",(int)fgetc(stdin));
				if (configMode == 0 && argc >= 2 && TEST_FLAG == '0'&&massFlag == 0)
					Safe_Flush(stdin);
				char normFlag = 'n';
				if (configMode == 0)
				{
					puts("Do you want Normalization? Please press y or n. (default n)");
					if (massFlag == 1)normFlag = 'y';
					if ((argc >= 2 && massFlag == 0) || argc == 1)
					{
						Safe_Flush(stdin);
						if (scanf("%c", &normFlag) != 1)
						{
							puts("Input error");
							exit(1);
						}
					}
				}
				else normFlag = receiveCfg.extraOption;
				
				if (normFlag == '\n') normFlag = 'n';
				while ((normFlag != 'y'&&normFlag != 'n') || normFlag == '\n')
				{
					printf("\r");
					printf("Unavailable Choice, please choose again: ");
					fflush(stdin);
					if (scanf("%c", &normFlag) != 1)
					{
						puts("Input error");
						exit(1);
					}
				}
				
				
				
				if (serverMode==0)
				{
					puts("\n--------------------------------- Confirm Input --------------------------------\n");
					if (Input_Matrix.n > 9)
						Show_Matrix(Input_Matrix.content, 1, Input_Matrix.n - 9, Input_Matrix.m, Input_Matrix.n, 1);
					else
						Show_Matrix(Input_Matrix.content, 1, 1, Input_Matrix.m, Input_Matrix.n, 1);
					printf("\nNeed Normalization = %c", normFlag);
				}
				
				//--------------------------------- Result --------------------------------
				Calculate_Result.m=Input_Matrix.m;
				Calculate_Result.n=Input_Matrix.n;
				Calculate_Result.content = Schmidt_Orthogonalization(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n);
				Approximate(Calculate_Result.content, Calculate_Result.m, Calculate_Result.n, 5);
				
				if (normFlag == 'y')
				{
					double **temp_Result = Vector_Normalization(Calculate_Result.content, Calculate_Result.m, Calculate_Result.n);
					Free_Matrix(Calculate_Result.content, Calculate_Result.m);
					Calculate_Result.content = temp_Result;
					Approximate(Calculate_Result.content, Calculate_Result.m, Calculate_Result.n, 5);
				}
				
				if (TEST_FLAG=='0') {
					double **tp_C=Calculate_Result.content;
					Calculate_Result.content=Transpose_Matrix(Calculate_Result.content, Calculate_Result.m, Calculate_Result.n);
					Free_Matrix(tp_C, Calculate_Result.m);		//把之前存的地址free
					int tp=Calculate_Result.m;
					Calculate_Result.m=Calculate_Result.n;
					Calculate_Result.n=tp;
				}
			}
			
			else
			{
				
				if (serverMode==0)
				{
					puts("\n--------------------------------- Confirm Input --------------------------------\n");
					if (Input_Matrix.n > 9)
						Show_Matrix(Input_Matrix.content, 1, Input_Matrix.n - 9, Input_Matrix.m, Input_Matrix.n, 1);
					else
						Show_Matrix(Input_Matrix.content, 1, 1, Input_Matrix.m, Input_Matrix.n, 1);
					puts("\n\n------------------------------------ Result ------------------------------------\n");
				}
				if (MODE == '5')
				{
					Row_Echelon_Form(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n, 0);
				}
				else if (MODE == '6')
				{
					Row_Canonical_Form(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n);
				}
				Approximate(Input_Matrix.content, Input_Matrix.m, Input_Matrix.n, 5);
				
				Calculate_Result.m=Input_Matrix.m;
				Calculate_Result.n=Input_Matrix.n;
				Calculate_Result.content=Create_Matrix(Calculate_Result.m, Calculate_Result.n, "");
				Matrix_Copy(Calculate_Result.content, Input_Matrix.content, 1,1,Input_Matrix.m, Input_Matrix.n);
				
				
			}
			Free_Matrix(Input_Matrix.content, Input_Matrix.m);
		}
		
		if (MODE == '7')
		{
			TEST_FLAG = '0';                //系统随机数填充的增广矩阵永远满秩的，绝大多数情况下方程组无解，因此Test模式无意义，故去除
			
			struct Characteristic_of_Matrix *Matrix_Description;
			Matrix_Description = (struct Characteristic_of_Matrix*)calloc(1, sizeof(struct Characteristic_of_Matrix));
			if (Matrix_Description == NULL)
			{
				perror("Matrix_Description MODE 7");
				exit(1);
			}
			Matrix_Description[0].Matrix_Name = "MODE 5 Input";
			
			if (configMode == 0)
			{
				puts("--------------------------------------------------------------------------------");
				puts("| Test mode is deprecated, you should input the Augmented Matrix manually.     |");
				puts("| The 'm' & 'n' represent row & column number of the Coefficient Matrix of     |");
				puts("| a linear equation system.                                                    |");
				puts("--------------------------------------------------------------------------------");
				printf("\nPlease input 'm' and 'n' : ");
				if (scanf("%d %d", &Matrix_Description[0].m, &Matrix_Description[0].n) != 2)
				{
					puts("Input error");
					exit(1);
				}
				puts("");
			}
			else if (configMode == 1)
			{
				Matrix_Description[0].m = receiveCfg.getM_One;
				Matrix_Description[0].n = receiveCfg.getN_One - 1;
			}
			
			double **AB = Create_Matrix(Matrix_Description[0].m, Matrix_Description[0].n + 1, "MODE 4 AB");
			
			if (configMode == 0)
				User_Input_Matrix(AB, Matrix_Description[0].m, Matrix_Description[0].n + 1, "",0);
			else
				Config_Fill_Matrix(AB, receiveCfg, 1);
			
			Approximate(AB, Matrix_Description[0].m, Matrix_Description[0].n + 1, 6);
			
			if (serverMode==0)
			{
				puts("\n--------------------------------- Confirm Input --------------------------------\n");
				if (Matrix_Description[0].n > 9)
					Show_Matrix(AB, 1, Matrix_Description[0].n + 1 - 9, Matrix_Description[0].m, Matrix_Description[0].n + 1, 1);
				else
					Show_Matrix(AB, 1, 1, Matrix_Description[0].m, Matrix_Description[0].n + 1, 1);
			}
			
			if (Check_Linear_Equation_Solution_Existance(AB, Matrix_Description[0].m, Matrix_Description[0].n) == 0)
			{
				Calculate_Result.sluExistFlag=0;
			}
			
			if (Calculate_Result.sluExistFlag!=0)
			{
				Row_Canonical_Form(AB, Matrix_Description[0].m, Matrix_Description[0].n + 1);
				
				Approximate(AB, Matrix_Description[0].m, Matrix_Description[0].n + 1, 6);
				
				int rank_of_A = Find_Rank(AB, Matrix_Description[0].m, Matrix_Description[0].n);
				int n_of_Solution_Matrix = Matrix_Description[0].n - rank_of_A + 2;
				double **Solution_Matrix = Create_Matrix(Matrix_Description[0].n, n_of_Solution_Matrix, "Solution Matrix");
				Build_Solution_Matrix(AB, Solution_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, n_of_Solution_Matrix, rank_of_A);
				
				Approximate(Solution_Matrix, Matrix_Description[0].n, n_of_Solution_Matrix, 5);
				Calculate_Result.m=Matrix_Description[0].n;
				Calculate_Result.n=n_of_Solution_Matrix;
				Calculate_Result.content=Solution_Matrix;
				
				
				if(Find_No_Zero_Row(Calculate_Result.content, Calculate_Result.n - 1, Calculate_Result.m)==0)
					Calculate_Result.homogeneousFlag = 1;
				if (Calculate_Result.m - rank_of_A <= 0)
					Calculate_Result.onlySluFlag=1;
			}
			
			Free_Matrix(AB, Matrix_Description[0].m);
			//			Free_Matrix(Solution_Matrix, Matrix_Description[0].n);
		}
		
		
		//------------------------------ 将运算结果输出 ------------------------------
		
		//		printf("SERVERMODE = %d",serverMode);
		
		if (serverMode==0)
		{
			puts("\n\n------------------------------------ Result ------------------------------------\n");
			switch (Calculate_Result.mode)
			{
				case 1:
					printf("Determinant Value = %lf\n", Calculate_Result.value);
					break;
					
				case 4:
					printf(" --------------------------------- A B %d X %d ------------------------------\n", Calculate_Result.m, Calculate_Result.n);
					break;
				case 7:
				{
					if (Calculate_Result.sluExistFlag==0)
					{
						printf("This Linear Equation System does not have a solution\n\n");
					}
					else
					{
						if (Calculate_Result.homogeneousFlag==0)
						{
							puts("\n------------------------------ Particular Solution -----------------------------\n");
							Show_Matrix(Calculate_Result.content, 1, Calculate_Result.n, Calculate_Result.m, Calculate_Result.n, 1);
							
						}
						if (Calculate_Result.onlySluFlag==1)
						{
							printf("\nThis Linear Equation Systems only exist one solution");
							if (Calculate_Result.homogeneousFlag == 1)puts(" = ZERO\n\n");
							else printf("\n\n");
						}
						else
						{
							puts("\n-------------------------- Fundamental Solution Systems ------------------------\n");
							Show_Matrix(Calculate_Result.content, 1, 2, Calculate_Result.m, Calculate_Result.n - 1, 1);
						}
					}
				}
				default:break;
			}
			
			if (Calculate_Result.mode!=1&&Calculate_Result.mode!=7)
			{
				if (Calculate_Result.n > 9)
				{
					Show_Matrix(Calculate_Result.content, 1, Calculate_Result.n - 9, Calculate_Result.m, Calculate_Result.n, 1);
				}
				else
					Show_Matrix(Calculate_Result.content, 1, 1, Calculate_Result.m, Calculate_Result.n, 1);
			}
		}
		else if(serverMode==1)
		{
			printf("%s",Result2JSON(Calculate_Result,0));
		}
		Free_Matrix(Calculate_Result.content, Calculate_Result.m);
		//-----------------------------------------------------------------------------
		
		if (configMode == 1) free(receiveCfg.getElements_One);
		if (invalidContinueFlag == 1) puts("");
		
		int robotFlag = 1;
#ifdef UNIX
		if (access("robotBegin", 0) == -1)robotFlag = 0;
#endif
#ifdef WINDOWS
		if (_access("robotBegin", 0) == -1)robotFlag = 0;
#endif
		if (invalidContinueFlag == 0)
		{
			if (massFlag == 0) Safe_Flush(stdin);
			puts("\nPress any key to run again or press 0 to exit");
			char continueFlag_ch = '0';
			if (massFlag == 0)
			{
				if (scanf("%c", &continueFlag_ch) != 1)
				{
					puts("Input error");
					exit(1);
				}
			}
			else if (massFlag == 1 && robotFlag == 0)continueFlag_ch = '1';
			if (continueFlag_ch!='0')CONTINUE_FLAG=1;
		}
	}while(CONTINUE_FLAG!=0);
	return 0;
}
