//
//  Tools.c
//  Matrix
//
//  Created by LI YANZHE on 02/02/2017.
//  Copyright © 2017 Yanzhe Lee. All rights reserved.
//

#include "Matrix.h"

sConfig Read_Config()
{
    sConfig readResult;
    char CurrentPath_Temp[1000];
    getcwd(CurrentPath_Temp, 1000);
    unsigned long PathLength=strlen(CurrentPath_Temp);
    //    char *CurrentPath=(char*)calloc(PathLength, sizeof(char));
    
    char *configPath=(char*)calloc(PathLength+strlen("/config.json"), sizeof(char));
    strcpy(configPath, CurrentPath_Temp);
    strcat(configPath, "/config.json");
    
    //    printf("Length = %lu\nPath = %s\n",strlen(configPath),configPath);
    FILE *fp=fopen(configPath,"rt");
    if (fp==NULL)
    perror("Config open error");
    else
    {
        int i;
        long int length;
        fseek(fp,0L,SEEK_END);
        length=ftell(fp);
        fseek(fp,0L,SEEK_SET);
        
        //        printf("length = %ld\n",length);
        
        char *fstr=(char*)calloc(length, sizeof(char));
        char C;
        for (i=0;;i++)
        {
            C=fgetc(fp);
            if (C==EOF)break;
            fstr[i]=C;
        }
        
        //        printf("%s\n",fstr);
        
        cJSON * root = cJSON_Parse(fstr);
        cJSON *Elements_One=cJSON_GetObjectItem(root, "Matrix_One");
        cJSON *Elements_Two=cJSON_GetObjectItem(root, "Matrix_Two");
        
        readResult.getMODE=cJSON_GetObjectItem(root, "MODE")->valueint;
        readResult.getTestFlag=cJSON_GetObjectItem(root, "TEST_FLAG")->valueint;
        readResult.getM_One=cJSON_GetObjectItem(root, "m_One")->valueint;
        readResult.getN_One=cJSON_GetObjectItem(root, "n_One")->valueint;
        if (Elements_Two!=NULL)
        {
            readResult.getM_Two=cJSON_GetObjectItem(root, "m_Two")->valueint;
            readResult.getN_Two=cJSON_GetObjectItem(root, "n_Two")->valueint;
        }
        readResult.extraOption=cJSON_GetObjectItem(root, "Extra_Option")->valuestring[0];
        
        readResult.getElements_One=(double*)calloc(readResult.getM_One*readResult.getN_One, sizeof(double));
        readResult.getElements_Two=(double*)calloc(readResult.getM_One*readResult.getN_One, sizeof(double));
        
        
        int row,column;
        i=0;
        for (row=0; row<readResult.getM_One; row++)
        {
            for (column=0; column<readResult.getN_One; column++,i++)
            {
                readResult.getElements_One[i]=cJSON_GetArrayItem(cJSON_GetArrayItem(Elements_One, row), column)->valuedouble;
            }
        }
        row=0;
        column=0;
        i=0;
        if (Elements_Two!=NULL)
        for (row=0; row<readResult.getM_Two; row++)
        {
            for (column=0; column<readResult.getN_Two; column++,i++)
            {
                readResult.getElements_Two[i]=cJSON_GetArrayItem(cJSON_GetArrayItem(Elements_Two, row), column)->valuedouble;
            }
        }
        free(fstr);
        //        printf("getMODE = %d\n",readResult.getMODE);
        //        printf("getTestFlag = %d\n",readResult.getTestFlag);
        //        printf("getM = %d\n",readResult.getM);
        //        printf("getN = %d\n",readResult.getN);
        //        puts("Elements");
        //        for (i=0; i<readResult.getM*readResult.getN; i++)
        //        {
        //            printf("%lf ",readResult.getElements[i]);
        //        }
    }
    
    fclose(fp);
    return readResult;
}

void Config_Fill_Matrix(double **Matrix,sConfig configSource,int TYPE)
{
    int i,j;
    int num=0;
    if (TYPE==1)
    for (i=0; i<configSource.getM_One; i++)
    {
        for (j=0; j<configSource.getN_One; j++,num++)
        {
            Matrix[i][j]=configSource.getElements_One[num];
        }
    }
    if (TYPE==2)
    for (i=0; i<configSource.getM_Two; i++)
    {
        for (j=0; j<configSource.getN_Two; j++,num++)
        {
            Matrix[i][j]=configSource.getElements_Two[num];
        }
    }
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

void Test_Scanf(struct Characteristic_of_Matrix *Recive_mn_for_Test,int structElementNumber,int m_rand_min,int mRandMax,int nRandMin,int nRandMax)
{
    srand((unsigned)time(NULL));
    //srand((unsigned)20);
    Recive_mn_for_Test[structElementNumber-1].m=m_rand_min+rand()%(mRandMax-m_rand_min);       //测试需要
    Recive_mn_for_Test[structElementNumber-1].n=nRandMin+rand()%(nRandMax-nRandMin);
}

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

int Find_Rank(double **Matrix,int m,int n)
{
    double **Copy_Of_Matrix=Create_Matrix(m,n,"Copy of Matrix");
    int i,j,rank=0;
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)
        Copy_Of_Matrix[i][j]=Matrix[i][j];                        //把传来的矩阵元素复制到新的Copy矩阵里，防止影响原矩阵
    }
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

double** Vector_Normalization(double **Matrix,int m,int n)
{
    double *product=(double*)calloc(n, sizeof(double));
    double **Result_Matrix=Create_Matrix(m, n, "");
    int i;
    double ***vector_System=Column_Vector_Extract(Matrix, m, n);
    
    for (i=0; i<n; i++)
    {
        product[i]=sqrt(Scalar_Product(vector_System[i], vector_System[i], m));
        if (product[i]!=0)
        {
            double **temp_Tranpose=Transpose_Matrix(vector_System[i], m, 1);
            Scalar_Multiplication(1/product[i], temp_Tranpose, 0, 1, m);
            vector_System[i]=Transpose_Matrix(temp_Tranpose, 1, m);
            Free_Matrix(temp_Tranpose, 1);
        }
    }
    
    Column_Vector_Refill(vector_System, Result_Matrix, m, n);
    
    free(product);
    for (i=0; i<n; i++)
    Free_Matrix(vector_System[i], m);
    return Result_Matrix;
}
