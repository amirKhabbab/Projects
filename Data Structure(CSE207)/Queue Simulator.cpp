#include<bits/stdc++.h>
#include<queue>
#include<stack>
using namespace std;

struct Customer
{
    int randArrTime;
    int RgArrTime;
    int randServTime;
    int arrTime;
    int servTime;
    int custNo;
    int waitTime;
    int processTime;
    int servBegin;
    int servEnd;
};

//void CalcArrTime(struct Customer ct[],int n);

stack<Customer> s;
queue<Customer> q;
int n,totalSerTime=0,totalWaitTime=0;
float avgWaitTime,avgServTime;

void CalcArrTime(struct Customer ob[])
{
    for(int i=2; i<=n; i++)
    {
        ob[i].arrTime = ob[i-1].arrTime + ob[i].RgArrTime; //immediate customer arrival time + current customer making arr time
    }
}

void CalcWaitTime(struct Customer ct[])
{
    //if current customer arrival time is greater than immediate customer servEnd time
    // then servBegin = arrtime
    for(int i=2; i<=n; i++)
    {
        if(ct[i-1].servEnd>=ct[i].arrTime)
           ct[i].waitTime = ct[i].servEnd - ct[i].arrTime;
        else
        ct[i].waitTime = 0;
        totalWaitTime+=ct[i].waitTime;

    }
}

void CalcServTime(struct Customer ct[])
{
     ct[1].servBegin = 0;
    ct[1].servEnd =ct[1].servTime+ct[1].arrTime;
    //if current customer arrival time is greater than immediate customer servEnd time
    // then servBegin = arrtime
    for(int i=2; i<=n; i++)
    {
        if(ct[i].arrTime>ct[i-1].servEnd)
            ct[i].servBegin = ct[i].arrTime;
        else
            ct[i].servBegin = ct[i-1].servEnd;
            ct[i].servEnd = ct[i].servBegin+ct[i].servTime;

    }
}
void CalcProcessTime(struct Customer ob[])
{
    for(int i=1; i<=n; i++)
    {
        ob[i].processTime = ob[i].servTime + ob[i].waitTime;
    }

}

int main()
{
    printf("\t\t\t\t\t-----------------------\n");
    printf("\t\t\t\t\tWelcome To Our Project\n");
    printf("\t\t\t\t\t-----------------------\n\n");

    printf("\t\t\t\tWe take all input as minuts (**)\n\n\n");

    int i,j;

    printf("Number of Customer: ");
    scanf("%d",&n);
    Customer ct[n+1];


    ct[1].arrTime = 0;
    ct[1].custNo = 1;
    ct[1].randServTime = rand()%50;
    ct[1].waitTime = 0;
    ct[1].RgArrTime = 0;
    for(i=2; i<=n; i++)
    {
        ct[i].randArrTime = rand()%500+1;
        ct[i].randServTime = rand()%50+1;
        ct[i].custNo = i;
    }

    for(i=2; i<=n; i++)
    {
        if(ct[i].randArrTime>=0 && ct[i].randArrTime<=80)
            ct[i].RgArrTime = 1;
        else if(ct[i].randArrTime>80 && ct[i].randArrTime<=150)
            ct[i].RgArrTime = 2;
        else if(ct[i].randArrTime>150 && ct[i].randArrTime<=220)
            ct[i].RgArrTime = 3;
        else if(ct[i].randArrTime>220 && ct[i].randArrTime<=290)
            ct[i].RgArrTime = 4;
        else if(ct[i].randArrTime>290 && ct[i].randArrTime<=370)
            ct[i].RgArrTime = 5;
        else if(ct[i].randArrTime>370 && ct[i].randArrTime<=500)
            ct[i].RgArrTime = 6;
    }

    CalcArrTime(ct);

    for(j=1; j<=n; j++)
    {
        if(ct[j].randServTime>=0 && ct[j].randServTime<=10)
        {
            ct[j].servTime = 1;
        }


        else if(ct[j].randServTime>10 && ct[j].randServTime<=20)
        {
            ct[j].servTime = 2;
        }

        else if(ct[j].randServTime>20 && ct[j].randServTime<=30)
        {
            ct[j].servTime = 3;
        }

        else if(ct[j].randServTime>30 && ct[j].randServTime<=40)
        {
            ct[j].servTime = 4;
        }

        else if(ct[j].randServTime>40 && ct[j].randServTime<=50)
        {
            ct[j].servTime = 5;
        }
        totalSerTime+=ct[j].servTime;
    }

    CalcServTime(ct);
    CalcWaitTime(ct);
    CalcProcessTime(ct);


    for(i=1; i<=n; i++)
    {
        q.push(ct[i]);
    }


    printf("\n   ---------------------------------------------------------------------------------------------------");
    printf("\n  | Cust.  |  Rand Ar  | Arrival  | Service  | Service   | Service  |   Wait  | Total served| Number of\n");
    printf("  | No.    |    Time   |   Time   |  Time    |  Start    |   End    |   Time  |    Time    |  Element");
    printf("\n   ---------------------------------------------------------------------------------------------------\n");
    for(int i=1; i<=n; i++){
            Customer ojj = q.front();
            s.push(ojj);
            //Customer kk = s.top();
            //printf("%d\n",kk.arrTime);
             q.pop();

        printf("     %d           %d           %d         %d          %d           %d          %d          %d           %d\n"
               ,ojj.custNo,ojj.RgArrTime,ojj.arrTime,ojj.servTime,ojj.servBegin,ojj.servEnd,ojj.waitTime,ojj.processTime,q.size());
    if(i!=n){
    printf("   --------------------------------------------------------------------------------------------------------\n");
        }
    else{
        printf("   ------------------------------------====-------------------------------====-----------------------\n");
        }
    }
        printf("\t\t\t\t\t%d min \t\t\t\t  %d min",
           totalSerTime,totalWaitTime);
           printf("\n\n");
           avgWaitTime = (float)totalWaitTime/n;
           avgServTime = (float)totalSerTime/n;
           printf("Average Queue wait time: %.2f min\n",avgWaitTime);
           printf("Average Queue Service time: %.2f min\n",avgServTime);


    }




