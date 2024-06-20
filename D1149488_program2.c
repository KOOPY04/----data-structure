#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_N 40
#define MAX_M 20

// 檢查所有格子是否都走過
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
    int n = atoi(argv[1]);      // 格子的行數
    int m = atoi(argv[2]);      // 格子的列
    int start_i = atoi(argv[3]); // 起始位置的行座標
    int start_j = atoi(argv[4]); // 起始位置的列座標

    int countarr[MAX_N][MAX_M] = {0}; // 紀錄每個格子的訪問次數
    int imove[8] = {-1, 0, 1, 1, 1, 0, -1, -1}; // 行方向的移動
    int jmove[8] = {1, 1, 1, 0, -1, -1, -1, 0}; // 列方向的移動
    int ibug = start_i, jbug = start_j; // 蟲的當前位置
    int count = 1; // 紀錄蟲的移動次數
    int r = 50000; // 最大移動次數

    // 判斷輸入的行與列是否在有效範圍內
    if (n < 3 || n > 40 || m < 2 || m > 20 || ibug < 0 || ibug >= n || jbug < 0 || jbug >= m)
    {
        printf("ERROR!\n");
        return 0;
    }

    countarr[ibug][jbug] = 1; // 起始位置標記為已訪問
    srand(time(NULL));

    while (r--)
    {
        int a = rand() % 8; // 隨機生成下一次的移動方向
        while (ibug + imove[a] < 0 || ibug + imove[a] >= n || jbug + jmove[a] < 0 || jbug + jmove[a] >= m)
        {
            a = rand() % 8; // 如果碰到牆重新選擇移動方向
        }
        ibug += imove[a]; // 更新蟲的位置
        jbug += jmove[a];
        countarr[ibug][jbug]++; // 紀錄新位置的訪問次數
        count++;    // 紀錄蟲的訪問次數

        // 如果所有格子都已走過，則跳出循環
        if (check(n, m, countarr))
        {
            break;
        }
    }

    // 將結果寫入檔案
    FILE *f = fopen("D1149488_program2.csv", "w+");
    fprintf(f, "%d,\n", count);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fprintf(f, "%d", countarr[i][j]);
            if (j != m - 1)
            {
                fprintf(f, ",");
            }
        }
        fprintf(f, "\n");
    }
    fclose(f);

    return 0;
}
