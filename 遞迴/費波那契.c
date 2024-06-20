#include<stdio.h>

int fibonacci(int n, int ans)
{
    if(n == 0 || n == 1)
    {
        return ans = n ;
    }
    else
    {
        return ans = fibonacci(n - 1, ans) + fibonacci(n - 2, ans);
    }
}
int main()
{
    int n, ans = 0;
    scanf("%d" ,&n);
    printf("%d\n" ,fibonacci(n, ans));
    return 0;
}