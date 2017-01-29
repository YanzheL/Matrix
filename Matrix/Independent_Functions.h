//
//  Independent_Functions.h
//  Matrix
//
//  Created by LI YANZHE on 14/12/2016.
//  Copyright © 2016 Yanzhe Lee. All rights reserved.
//

#ifndef Independent_Functions_h
#define Independent_Functions_h

double** Create_Matrix(int m,int n,char *DISPLAY)
{
    int i;
    double **Matrix;
    if((Matrix=(double**)calloc(m,sizeof(double*)))==NULL)
    {
        printf("Matrix%s Calloc Error",DISPLAY);
        return NULL;
    }
    
    for (i=0; i<m; i++)
    {
        Matrix[i]=(double*)calloc(n, sizeof(double));
    }
    return Matrix;
}

void Free_Matrix(double **Matrix,int m)
{
    int i;
    for (i=0; i<m; i++)
    {
        free(Matrix[i]);
    }
    free(Matrix);
}

void Scalar_Multiplication(double k, double **Matrix,int r,int m,int n)                           //数乘运算 用k数乘第r行
{
    int j;
    for (j=0;j<=n-1; j++)
        Matrix[r][j]*=k;
}

void Show_Matrix(double **Matrix,int start_row,int start_column,int end_row,int end_column,int display_flag)
{                                                                                                 //用于向屏幕打印整个矩阵
    int i,j;
    for (i=start_row-1; i<=end_row-1; i++)
    {
        for (j=start_column-1; j<=end_column-1; j++)
        {
            if (display_flag==1)
            {
                if (Matrix[i][j]==floor(Matrix[i][j]))
                    printf("%10d",(int)floor(Matrix[i][j]));
                else if (Matrix[i][j]==ceil(Matrix[i][j]))
                    printf("%10d",(int)ceil(Matrix[i][j]));
                else
                    printf("%10.3lf",Matrix[i][j]);
            }
            else printf("%10.3lf",Matrix[i][j]);
        }
        puts("");
    }
}

//void Approximate(double **Matrix,int m,int n,int k)
//{
//    int i,j;
//    double p=pow(10, -k);
//    for (i=0; i<=m-1; i++)
//    {
//        for (j=0; j<=n-1; j++)
//        {
//            if (fabs(Matrix[i][j])<p)
//                Matrix[i][j]=0;                                                                 //近似运算
//            if (fabs(fabs(Matrix[i][j])-1)<p)
//                Matrix[i][j]=(Matrix[i][j]>0)?1:(-1);
//        }
//    }
//}

void Approximate(double **Matrix,int m,int n,int k)
{
    int i,j;
    double p=pow(10, -k);
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)
        {
            if (Matrix[i][j]-floor(Matrix[i][j])<p)
                Matrix[i][j]=floor(Matrix[i][j]);                                            //近似运算
            else if (ceil(Matrix[i][j])-Matrix[i][j]<p)
                Matrix[i][j]=ceil(Matrix[i][j]);
        }
    }
}


void Row_Exchange(double **Matrix,int r1,int r2,int n)                                       //行交换，r1行与r2行交换
{
    double temp=0;
    int j;
    for (j=0;j<=n-1; j++)
    {
        temp=Matrix[r1][j];
        Matrix[r1][j]=Matrix[r2][j];
        Matrix[r2][j]=temp;
    }
}

void Row_Add(double **Matrix,int r1,int r2,int n,int flag)                                   //行加减运算
{
    int c;
    if(flag==0)                                                                              //flag=0时进行r1行加r2行
        for (c=0;c<=n-1;c++)
        {
            Matrix[r1][c]+=Matrix[r2][c];
        }
    if(flag==1)                                                                              //flag=1时进行r1行减r2行
        for (c=0;c<=n-1;c++)
        {
            Matrix[r1][c]-=Matrix[r2][c];
        }
}

int Find_No_Zero_Row(double **Matrix,int column,int m)                                       //从上到下找出第column列中最后一个非零元所在的行号
{
    int count=0,row;
    for (row=0; row<=m-1; row++)
    {
        if(fabs(Matrix[row][column])!=0)count=row;
    }
    
    return count;
}

int Find_Leading_Column(double **Matrix,int row,int n)
{
    int count=0,column;
    for (column=n-1; column>=0; column--)
    {
        if(fabs(Matrix[row][column])!=0)count=column;
    }
    
    return count;
}

int Check_Zero_Matrix(double **Matrix,int m,int n)
{
    int zero_element_count=0,i,j;
    for (i=0; i<=m-1; i++)                                                                   //数出增广矩阵中零元素的个数
    {
        for (j=0; j<=n-1; j++)
        {
            if (Matrix[i][j]==0)zero_element_count++;
        }
    }
    if(zero_element_count==(m)*(n))
    {
        printf("Zero Element = %d\n",zero_element_count);
        return -1;                                                                           //如果用户输入的矩阵元素全为0，则不用化简，直接返回
    }
    else return zero_element_count;
}

void Safe_Flush(FILE *fp)
{
    int ch;
    while( (ch = fgetc(fp)) != EOF && ch != '\n' );
//    while( (ch = fgetc(fp)) != EOF );
}

void Rand_Fill(double **Matrix,int m,int n,int MIN,int MAX,int MODE)
{
    int i,j;
    srand((unsigned)time(NULL));
    for (i=0; i<=m-1; i++)
    {
        for (j=0; j<=n-1; j++)                                                               //测试需要 随机整数填充增广矩阵
        {
            if (MODE==1)
                Matrix[i][j]=MIN+rand()%(MAX-MIN-1)+(double)rand()/RAND_MAX;                 //随机小数填充
            else Matrix[i][j]=MIN+rand()%MAX;                                                //随机整数填充
        }
    }
}


#endif /* Independent_Functions_h */
