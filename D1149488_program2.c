#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_N 40
#define MAX_M 20

// �ˬd�Ҧ���l�O�_�����L
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
    int n = atoi(argv[1]);      // ��l�����
    int m = atoi(argv[2]);      // ��l���C
    int start_i = atoi(argv[3]); // �_�l��m����y��
    int start_j = atoi(argv[4]); // �_�l��m���C�y��

    int countarr[MAX_N][MAX_M] = {0}; // �����C�Ӯ�l���X�ݦ���
    int imove[8] = {-1, 0, 1, 1, 1, 0, -1, -1}; // ���V������
    int jmove[8] = {1, 1, 1, 0, -1, -1, -1, 0}; // �C��V������
    int ibug = start_i, jbug = start_j; // �Ϊ���e��m
    int count = 1; // �����Ϊ����ʦ���
    int r = 50000; // �̤j���ʦ���

    // �P�_��J����P�C�O�_�b���Ľd��
    if (n < 3 || n > 40 || m < 2 || m > 20 || ibug < 0 || ibug >= n || jbug < 0 || jbug >= m)
    {
        printf("ERROR!\n");
        return 0;
    }

    countarr[ibug][jbug] = 1; // �_�l��m�аO���w�X��
    srand(time(NULL));

    while (r--)
    {
        int a = rand() % 8; // �H���ͦ��U�@�������ʤ�V
        while (ibug + imove[a] < 0 || ibug + imove[a] >= n || jbug + jmove[a] < 0 || jbug + jmove[a] >= m)
        {
            a = rand() % 8; // �p�G�I���𭫷s��ܲ��ʤ�V
        }
        ibug += imove[a]; // ��s�Ϊ���m
        jbug += jmove[a];
        countarr[ibug][jbug]++; // �����s��m���X�ݦ���
        count++;    // �����Ϊ��X�ݦ���

        // �p�G�Ҧ���l���w���L�A�h���X�`��
        if (check(n, m, countarr))
        {
            break;
        }
    }

    // �N���G�g�J�ɮ�
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
