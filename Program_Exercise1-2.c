#include <stdio.h>
#include <stdlib.h>

void recursive(int n)
{
    if (n == 0 || n == 1 || n == 2 || n == 3 || n == 4 || n == 5 || n == 6 || n == 7)
    {
        printf("%d", n);
    }
    else if (n % 8 == 0)
    {
        recursive(n / 8);
        printf("0");
    }
    else
    {
        recursive(n / 8);
        printf("%d" ,n % 8);
    }
}

int main()
{
    int num;
    scanf("%d", &num);

    for (int i = 1; i <= num; i++)
    {
        int n;
        scanf("%d", &n);

        printf("Case #%d: ", i);
        recursive(n);
        printf("\n");
    }

    return 0;
}
