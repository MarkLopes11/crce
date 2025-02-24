#include<stdio.h>
#include<stdlib.h>
int n;
int col[20]={0};
int count=0;
void solution()
{
    int i,j;
    printf("solution: %d",count);
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            if(col[i]==j)
            {
                printf("q\t");
            }
            else{
                printf("x\t");
            }
        }
        printf("\n");
    }
    printf("\n");
}
int loc(int q,int c)
{
    int k;
    
    for(k=1;k<q;k++)
    {
        if(col[k]==c || abs(k-q) == abs(col[k]-c))
        {
            return 0;
        }
        
    }
    return 1;
}
void n_queen(int q)
{
    int c;
    for(c=1;c<=n;c++)
    {
        if(loc(q,c))
        {
            col[q]=c;
            if(q==n)
            {
                count++;
                solution();
            }
            else{
                n_queen(q+1);
            }
        }
    }
}

int main()
{
    printf("Enter no.of queen:");
    scanf("%d",&n);
    n_queen(1);
    if(count==0)
    {
        printf("No solution:");
    }
    else{
        printf("Solution: %d",count);
    }
    return 0;
}