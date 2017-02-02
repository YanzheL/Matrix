//
//  main.c
//  Matrix
//
//  Created by LI YANZHE on 30/11/2016.
//  Copyright © 2016 Yanzhe Lee. All rights reserved.
//
#include "Matrix.h"

static char TEST_FLAG='0';
static char MODE='0';
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
int main(int argc, const char * argv[])
{
    
    //    Read_Config();
    int invalidOptionFlag=0;
    int massFlag=0;
    int configMode=0;
    int outputMode=0;
    char *outputFileName;
    //    configMode=1;
    sConfig receiveCfg={0,0,'n',0,0,0,0,NULL,NULL};//初始化
    if (Check_Option(argc, argv, "-c")!=0||Check_Option(argc, argv, "--config")!=0)configMode=1;
    if (Check_Option(argc, argv, "--mass-test")!=0)
    {
        massFlag=1;
        TEST_FLAG='1';
    }
    if (Check_Option(argc, argv, "-o")!=0||Check_Option(argc, argv, "--out")!=0)outputMode=1;
    if (Check_Option(argc, argv, "--test")!=0) TEST_FLAG='1';
    
    if (configMode==1)
    {
        invalidOptionFlag=1;
        receiveCfg=Read_Config();
        MODE=(char)(48+receiveCfg.getMODE);
        TEST_FLAG=(char)(48+receiveCfg.getTestFlag);
    }
    
    if (outputMode==1)
    {
        time_t timeRaw;
        time(&timeRaw);
        outputFileName=ctime(&timeRaw);
        char *outTemp=(char*)calloc(strlen("【MODE 0】OUTPUT")+strlen(outputFileName)+strlen(".txt"), sizeof(char));
        strcpy(outTemp, "【MODE  】");
        outTemp[strlen("【MODE ")]=MODE;
        strcat(outTemp, "OUTPUT ");
        strcat(outTemp, outputFileName);
        outTemp[strlen(outTemp)-1]=' ';
        strcat(outTemp, ".txt");
        outputFileName=outTemp;
        freopen(outputFileName, "w", stdout);
    }
    if (argc==2&&strcmp(argv[1], "--lord")==0)               //上帝模式，可以打印出程序自身源码
    {
        char passwd[15];
        int k,l;
        puts("So prove that you are the Lord");                   //需要输入密码
        for(k=0;k<14;k++)
        {
#ifdef OSX
            passwd[k]=getch_(0);
#endif
#ifdef WINDOWS
            passwd[k]=_getch();
#endif
            printf("*");
        }
        passwd[14]='\0';
        printf("\n");
        if (strcmp(passwd, "LYZ18679853316")==0)
        {
            for (l=1;l<=3;l++)
            {
                for (k=1;k<=230; k++)printf("#");
                puts("");
            }
            printf("Main Source __FILE__ PATH = %s\n",__FILE__);
            FILE *fSOURCE = fopen(__FILE__, "r");
            if(Show_File_Text(fSOURCE)!=0)return 1;
            puts("");
            for (l=1;l<=3;l++)
            {
                for (k=1;k<=230; k++)printf("#");
                puts("");
            }
            if(Show_Header_Source()!=0)return 1;
        }
        else puts("You are not my lord\n");
        return 0;
    }
    if ((argc>=2&&strcmp(argv[1], "--mode-1")==0)||MODE=='1')
    {
        MODE='1';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-2")==0)||MODE=='2')
    {
        MODE='2';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-3")==0)||MODE=='3')
    {
        MODE='3';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-4")==0)||MODE=='4')
    {
        MODE='4';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-5")==0)||MODE=='5')
    {
        MODE='5';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-6")==0)||MODE=='6')
    {
        MODE='6';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-7")==0)||MODE=='7')
    {
        MODE='7';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if ((argc>=2&&strcmp(argv[1], "--mode-8")==0)||MODE=='8')
    {
        MODE='8';
        Show_Index_Page();
        Show_MODE_Band(MODE);
    }
    else if (argc>=2&&strcmp(argv[1], "--menu")==0)
        Show_Menu_Page();
    else if(argc>=2&&(strcmp(argv[1], "--help")==0||strcmp(argv[1], "-h")==0))
    {
        Show_Index_Page();
        Show_Help_Page();
        invalidOptionFlag=1;
    }
    else if (((argc==1||(argc==2&&(TEST_FLAG!=0||outputMode==1))))&&configMode==0)
    {
        Show_Index_Page();
        Show_Menu_Page();
        printf("Please choose mode number: ");
        scanf("%c",&MODE);
        fflush(stdin);
        while ((MODE>'8'||MODE<'1')&&MODE!='c')
        {
            printf("Unavailable Choice, please choose again\r");
            //            Safe_Flush(stdin);
            scanf("%c",&MODE);
        }
    }
    else if(argc>=2&&configMode==0)
    {
        printf("invalid option '%s'; type '--help' for a list.\n",argv[argc-1]);
        puts("Syntax: Matrix [Commands] [options]");
        invalidOptionFlag=1;
    }
    
    if (MODE=='c')
    {
        configMode=1;
        invalidOptionFlag=1;
        receiveCfg=Read_Config();
        MODE=(char)(48+receiveCfg.getMODE);
        TEST_FLAG=(char)(48+receiveCfg.getTestFlag);
        Show_MODE_Band(MODE);
    }
    
    //    if (argc>=2&&strcmp(argv[argc-1], "--test")==0)TEST_FLAG='1';
    
    if (argc==1&&configMode==0)
    {
        printf("Press any key to test or press 0 to manually input\n");
        Safe_Flush(stdin);
        scanf("%c",&TEST_FLAG);
        //        printf("TEST_FLAG = %c\n",TEST_FLAG);
        fflush(stdin);
    }
    if (MODE=='1'||MODE=='2'||MODE=='3')
    {
        int n=1;
        if(TEST_FLAG!='0')
        {
            srand((unsigned)time(NULL));                                                    //测试需要 获取随机的m和n
            n=4+rand()%4;
        }
        else if(configMode==0)
        {
            printf("\nPlease input 'n' : ");
            scanf("%d",&n);
            puts("");
        }
        else if (configMode==1)
        {
            if (receiveCfg.getM_One!=receiveCfg.getN_One)
            {
                puts("Your input Matrix is not a Square Matrix");
                return 0;
            }
            else n=receiveCfg.getN_One;
        }
        
        double **Matrix=Create_Matrix(n,n,"MODE 1&2&3");
        if (TEST_FLAG=='0')
        {
            if (configMode==0)
                User_Input_Matrix(Matrix, n, n, "");
            else
                Config_Fill_Matrix(Matrix, receiveCfg,1);
        }
        else
            Rand_Fill(Matrix, n, n,-10,10,0);
        
        Approximate(Matrix, n, n, 6);
        puts("--------------------------------- Confirm Input --------------------------------\n");
        if(n>9)
            Show_Matrix(Matrix, 1,n-9,n, n,1);
        else
            Show_Matrix(Matrix, 1,1,n, n,1);
        puts("\n\n------------------------------------ Result ------------------------------------\n");
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
        if (MODE=='3')
        {
            if(Reverse_Matrix(Matrix, n)==0)
                printf("The Inverse Matrix doesn't Exist\n");
            else
            {
                if(n>9)Show_Matrix(Matrix, 1,n-9,n, n,1);
                else Show_Matrix(Matrix, 1,1,n, n,1);
            }
            Free_Matrix(Matrix, n);
        }
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
        else if (configMode==0)
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
        else if (configMode==1)
        {
            Matrix_Description[0].m=receiveCfg.getM_One;
            Matrix_Description[0].n=receiveCfg.getN_One;
            Matrix_Description[1].m=receiveCfg.getM_Two;
            Matrix_Description[1].n=receiveCfg.getN_Two;
        }
        
        int multi_times=1;
        int multiFlag=0;
        if (Matrix_Description[0].m==Matrix_Description[1].n)
        {
            multiFlag=1;
            if (configMode==0)
            {
                printf("\nIt seems like AB is a square matrix.\nSo input how many times you want to multipy in total: ");
                for (i=0;; i++)
                {
                    scanf("%d",&multi_times);
                    if (multi_times>=1) break;
                    else printf("Wrong answer. Please input again: ");
                }
            }
            else
            {
                multi_times=(int)(receiveCfg.extraOption)-48;
            }
            printf("Multi Times = %d\n",multi_times);
        }
        
        double **A=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n,Matrix_Description[0].Matrix_Name);
        double **B=Create_Matrix(Matrix_Description[1].m,Matrix_Description[1].n,Matrix_Description[1].Matrix_Name);
        double **Result_Matrix=Create_Matrix(Matrix_Description[0].m,Matrix_Description[1].n,"MODE 4 Result");
        
        if (TEST_FLAG=='0')
        {
            if (configMode==0)
            {
                puts("");
                User_Input_Matrix(A, Matrix_Description[0].m, Matrix_Description[0].n, " A");
                puts("");
                User_Input_Matrix(B, Matrix_Description[1].m, Matrix_Description[1].n, " B");
            }
            else
            {
                Config_Fill_Matrix(A, receiveCfg,1);
                Config_Fill_Matrix(B, receiveCfg,2);
            }
        }
        else
        {
            Rand_Fill(A, Matrix_Description[0].m, Matrix_Description[0].n,50,500,0);
            Rand_Fill(B, Matrix_Description[1].m, Matrix_Description[1].n,50,500,0);
        }
        
        Approximate(A, Matrix_Description[0].m, Matrix_Description[0].n, 6);
        
        puts("--------------------------------- Confirm Input --------------------------------\n");
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
        
        
        if (multiFlag==1)
        {
            double **Temp_Result=Create_Matrix(Matrix_Description[0].m, Matrix_Description[1].n, "");
            for (i=1;i<=multi_times-1;i++)
            {
                Matrix_Multiplication(Result_Matrix, Result_Matrix, Temp_Result, Matrix_Description[0].m, Matrix_Description[0].m, Matrix_Description[0].m, Matrix_Description[0].m);
                //            Matrix_Multiplication(<#double **A#>, <#double **B#>, <#double **Result_Matrix#>, <#int m_A#>, <#int n_A#>, <#int m_B#>, <#int n_B#>)
            }
            Result_Matrix=Temp_Result;
        }
        
        puts("\n\n------------------------------------ Result ------------------------------------\n");
        
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
    
    if (MODE=='5'||MODE=='6'||MODE=='8')
    {
        struct Characteristic_of_Matrix *Matrix_Description;
        Matrix_Description=(struct Characteristic_of_Matrix*)calloc(1,sizeof(struct Characteristic_of_Matrix));
        Matrix_Description[0].Matrix_Name="MODE 5&6&8 Input";
        
        if(TEST_FLAG!='0')
            Test_Scanf(Matrix_Description,1, M_RAND_MIN,M_RAND_MAX,N_RAND_MIN,N_RAND_MAX);
        else if(configMode==0)
        {
            printf("\nPlease input 'm' and 'n' : ");
            scanf("%d %d",&Matrix_Description[0].m,&Matrix_Description[0].n);
            puts("");
        }
        else if (configMode==1)
        {
            Matrix_Description[0].m=receiveCfg.getM_One;
            Matrix_Description[0].n=receiveCfg.getN_One;
        }
        
        double **Input_Matrix=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n,"MODE 5&6&8");
        
        if (TEST_FLAG=='0')
        {
            if (configMode==0)
                User_Input_Matrix(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, "");
            else
                Config_Fill_Matrix(Input_Matrix, receiveCfg,1);
        }
        else
            Rand_Fill(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n,-10,10,0);
        
        Approximate(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 6);
        
        if (MODE=='8')
        {
            //            printf("stdin = %d\n",(int)fgetc(stdin));
            if (configMode==0&&argc>=2&&TEST_FLAG=='0'&&massFlag==0)
                Safe_Flush(stdin);
            char normFlag='n';
            if (configMode==0)
            {
                puts("Do you want Normalization? Please press y or n. (default n)");
                if (massFlag==1)normFlag='y';
                if (argc>=2&&massFlag==0)scanf("%c",&normFlag);
            }
            else normFlag=receiveCfg.extraOption;
            
            if (normFlag=='\n') normFlag='n';
            while (normFlag!='y'&&normFlag!='n'&&normFlag!='\n')
            {
                printf("Unavailable Choice, please choose again: ");
                fflush(stdin);
                scanf("%c",&MODE);
            }
            
            puts("--------------------------------- Confirm Input --------------------------------\n");
            if(Matrix_Description[0].n>9)
                Show_Matrix(Input_Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            else
                Show_Matrix(Input_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            puts("\n\n------------------------------------ Result ------------------------------------\n");
            double **Result_Matrix=Schmidt_Orthogonalization(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n);
            Approximate(Result_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 5);
            
            if (normFlag=='y')
            {
                double **temp_Result=Vector_Normalization(Result_Matrix, Matrix_Description[0].m, Matrix_Description[0].n);
                Result_Matrix=temp_Result;
            }
            if(Matrix_Description[0].n>9)
                Show_Matrix(Result_Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            else
                Show_Matrix(Result_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            Free_Matrix(Result_Matrix, Matrix_Description[0].m);
        }
        
        else
        {
            puts("--------------------------------- Confirm Input --------------------------------\n");
            if(Matrix_Description[0].n>9)
                Show_Matrix(Input_Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            else
                Show_Matrix(Input_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            
            puts("\n\n------------------------------------ Result ------------------------------------\n");
            if(MODE=='5')
            {
                if(Row_Echelon_Form(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n,0)==0)
                    Show_Matrix(Input_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            }
            else if(MODE=='6')
            {
                if(Row_Canonical_Form(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n)==0)
                    Show_Matrix(Input_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,0);
            }
            Approximate(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, 5);
            if(Matrix_Description[0].n>9)
                Show_Matrix(Input_Matrix, 1,Matrix_Description[0].n-9,Matrix_Description[0].m, Matrix_Description[0].n,1);
            else
                Show_Matrix(Input_Matrix, 1,1,Matrix_Description[0].m, Matrix_Description[0].n,1);
            printf("Rank = %d\n",Find_Rank(Input_Matrix, Matrix_Description[0].m, Matrix_Description[0].n));
        }
        Free_Matrix(Input_Matrix, Matrix_Description[0].m);
    }
    
    if (MODE=='7')
    {
        TEST_FLAG='0';
        
        struct Characteristic_of_Matrix *Matrix_Description;
        Matrix_Description=(struct Characteristic_of_Matrix*)calloc(1,sizeof(struct Characteristic_of_Matrix));
        Matrix_Description[0].Matrix_Name="MODE 5 Input";
        
        if(TEST_FLAG!='0')
            Test_Scanf(Matrix_Description,1, M_RAND_MIN,M_RAND_MAX,N_RAND_MIN,N_RAND_MAX);
        else if(configMode==0)
        {
            printf("\nPlease input 'm' and 'n' : ");
            scanf("%d %d",&Matrix_Description[0].m,&Matrix_Description[0].n);
            puts("");
        }
        else if (configMode==1)
        {
            Matrix_Description[0].m=receiveCfg.getM_One;
            Matrix_Description[0].n=receiveCfg.getN_One-1;
        }
        
        double **AB=Create_Matrix(Matrix_Description[0].m,Matrix_Description[0].n+1,"MODE 4 AB");
        
        if (TEST_FLAG=='0')
        {
            if (configMode==0)
                User_Input_Matrix(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, "");
            else
                Config_Fill_Matrix(AB, receiveCfg,1);
        }
        else
            Rand_Fill(AB, Matrix_Description[0].m, Matrix_Description[0].n,-10,10,0);
        
        //        if (TEST_FLAG=='0')
        //            User_Input_Matrix(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, " Augmented");
        //        else
        //            Rand_Fill(AB, Matrix_Description[0].m, Matrix_Description[0].n+1,50,1000,0);
        
        Approximate(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, 6);
        
        puts("--------------------------------- Confirm Input --------------------------------\n");
        if(Matrix_Description[0].n>9)
            Show_Matrix(AB, 1,Matrix_Description[0].n+1-9,Matrix_Description[0].m, Matrix_Description[0].n+1,1);
        else
            Show_Matrix(AB, 1,1,Matrix_Description[0].m, Matrix_Description[0].n+1,1);
        
        puts("\n\n------------------------------------ Result ------------------------------------\n");
        if(Check_Linear_Equation_Solution_Existance(AB, Matrix_Description[0].m, Matrix_Description[0].n)==0)
        {
            printf("This Linear Equation System does not have a solution\n");
            return 0;
        }
        
        Row_Canonical_Form(AB, Matrix_Description[0].m, Matrix_Description[0].n+1);
        
        Approximate(AB, Matrix_Description[0].m, Matrix_Description[0].n+1, 6);
        
        int rank_of_A=Find_Rank(AB, Matrix_Description[0].m, Matrix_Description[0].n),n_of_Solution_Matrix=Matrix_Description[0].n-rank_of_A+2;
        double **Solution_Matrix=Create_Matrix(Matrix_Description[0].n,n_of_Solution_Matrix,"Solution Matrix");
        Build_Solution_Matrix(AB, Solution_Matrix, Matrix_Description[0].m, Matrix_Description[0].n, n_of_Solution_Matrix, rank_of_A);
        
        
        Approximate(Solution_Matrix, Matrix_Description[0].n, n_of_Solution_Matrix, 5);
        
        int Homogeneous_Flag=Find_No_Zero_Row(Solution_Matrix, n_of_Solution_Matrix-1, Matrix_Description[0].n);
        if (Homogeneous_Flag!=0)
        {
            puts("\n------------------------------ Particular Solution -----------------------------\n");
            Show_Matrix(Solution_Matrix, 1,n_of_Solution_Matrix,Matrix_Description[0].n, n_of_Solution_Matrix, 1);
            
        }
        if (Matrix_Description[0].n-rank_of_A<=0)
        {
            printf("\nThis Linear Equation Systems only exist one solution\n\n");
            if (Homogeneous_Flag==0)puts(" = ZERO\n");
        }
        else
        {
            puts("\n-------------------------- Fundamental Solution Systems ------------------------\n");
            Show_Matrix(Solution_Matrix, 1,2,Matrix_Description[0].n, n_of_Solution_Matrix-1, 1);
        }
        Free_Matrix(AB, Matrix_Description[0].m);
        Free_Matrix(Solution_Matrix, Matrix_Description[0].n);
    }
    
    if (configMode==1) free(receiveCfg.getElements_One);
    if(invalidOptionFlag==0)
    {
        if (massFlag==0) Safe_Flush(stdin);
        puts("\nPress any key to run again or press 0 to exit");
        char continueFlag='0';
        if (massFlag==0) scanf("%c",&continueFlag);
        if (massFlag==1)continueFlag='1';
        if(continueFlag!='0')
        {
            fflush(stdin);
            if ((argc>=2&&(TEST_FLAG!='0'||massFlag==1)))main(argc ,argv);
            else
            {
                int argcNext=1;
                const char** argvNext=(const char**)calloc(1, sizeof(char*));
                argvNext[0]=(char*)calloc(1, sizeof(char));
                argvNext[0]=argv[0];
                main(argcNext, argvNext);
            }
        }
    }
    
//    if (outputMode==1)
//    {
//        FILE *output=fopen(outputFileName, "rt");
//        Show_File_Text(output);
//        fclose(stdout);
//    }
    
    return 0;
}
