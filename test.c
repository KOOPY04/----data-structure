#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_N 40
#define MAX_M 20

bool check(int n, int m, int countarr[MAX_N][MAX_M])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (countarr[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: %s n m start_i start_j\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int start_i = atoi(argv[3]);
    int start_j = atoi(argv[4]);

    int countarr[MAX_N][MAX_M] = {0};
    int imove[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int jmove[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    int ibug = start_i, jbug = start_j;
    int count = 1;
    int r = 50000;

    if (n < 3 || n > 40 || m < 2 || m > 20 || ibug < 0 || ibug >= n || jbug < 0 || jbug >= m)
    {
        printf("input out of range.\n");
        return 1;
    }

    countarr[ibug][jbug] = 1;
    srand(time(NULL));

    while (r--)
    {
        int a = rand() % 8;
        while (ibug + imove[a] < 0 || ibug + imove[a] >= n || jbug + jmove[a] < 0 || jbug + jmove[a] >= m)
        {
            a = rand() % 8;
        }
        ibug += imove[a];
        jbug += jmove[a];
        countarr[ibug][jbug]++;
        count++;

        if (check(n, m, countarr))
        {
            break;
        }
    }

    FILE *f = fopen("D1149488_program2.csv", "w");
    fprintf(f,"%d,\n", count);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fprintf(f,"%d ", countarr[i][j]);
            if(j != m - 1)
            {
                fprintf(f,",");
            }
        }
        fprintf(f,"\n");
    }
    fclose(f);

    return 0;
}
