#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<time.h>
unsigned long long adj_mat[5000][5000];
int main()
{
    int n,in_degree, out_degree, sum1=0, sum2=0, i, j;
    clock_t starttime=clock();
    printf("\nEnter the number of vertices: ");
    scanf("%d", &n);
    char check;
    for ( i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++)
        {
            for(int c=0; c<n; c++)
            {
                check = rand()%2;
                if (check == 1)
                {
                    adj_mat[i][j] = 1;
                }

                else
                {
                    adj_mat[i][j] = 0;
                }
            }
        }
    }
printf("==============================Adjacency Matrix==============================\n\n\n");
    for(i=1; i<=n; i++)
    {
        for(j=1; j<=n; j++)
        {
            printf("\t%d\t\t",adj_mat[i][j]);        }
        printf("\n\n\n");
    }
    printf("\n VERTEX \t IN DEGREE \t OUT DEGREE \t TOTAL DEGREE ");
    for (i=1; i<=n; i++)
    {
        in_degree = out_degree = 0;

        for (j=1; j<=n; j++)
        {
            if(adj_mat[j][i] == 1)
            {
                in_degree++;
            }
        }

        for (j=1; j<=n; j++)
        {
            if (adj_mat[i][j] == 1 )
            {
                out_degree++;
            }
        }
        printf("\n\n %5d\t\t\t%d\t\t%d\t\t%d\n\n",i,in_degree,out_degree,in_degree+out_degree);
        sum1+=in_degree;
        sum2+=out_degree;
    }
    clock_t endtime=clock();
    double t=(double)(endtime-starttime)/CLOCKS_PER_SEC;
    double tn=t*1000000000;
    printf("Sum of in degress = %d\n", sum1);
    printf("Sum of out degress = %d\n\n\n", sum2);
    printf("Sum of in degrees = Sum of out degrees    (proved)\n");
    printf("\ntime: %lf second\n",t);
    printf("\ntime: %lf nano second\n",tn);
    return 0;
}

