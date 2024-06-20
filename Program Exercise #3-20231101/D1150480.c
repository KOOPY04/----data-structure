#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_QUEUE_SIZE 10 // �@���d�x�̦h���H
int global_clock;
int window_num;

typedef struct Customer
{
    char id[20];  // �Ȥ� id
    int arr_time; // �Ȥ��F��?�ɶ�
    int ser_time; // �Ȥ᧹���~�ȭn��h�֮ɶ�
} Customer;

typedef struct TellerQueue
{
    int status;                     // status, 0: close, 1:�u�ѤU�@��Ȥ�A�Y�Nclose, 2: open
    int front;                      // front pointer�A���V�Ĥ@��element ��index
    int rear;                       // rear pointer�A���V�̫�@��element ���U�@��index
    int count;                      // queue ���b�ƶ����H?
    int served_time;                // �ثe�b�B?�~�Ȫ��ȤH�w�g�᪺�ɶ�
    Customer queue[MAX_QUEUE_SIZE]; // �H array ��������Circular Queue�C
} TellerQueue;

Customer people[10000];
TellerQueue *Teller;

void Update(int now)
{

    // int now_time = people[now].arr_time;
    int now_time;
    if (people[now].arr_time != 0)
        now_time = people[now].arr_time;
    else
        now_time = now;
    while (global_clock != now_time)
    {
        global_clock++;
        for (int i = 0; i < window_num; i++)
        {
            if (Teller[i].count == 0 || Teller[i].status == 0)
            {
                continue;
            }
            Teller[i].served_time++;
            if (Teller[i].served_time >= Teller[i].queue[Teller[i].front].ser_time)
            {
                printf("%s %d %d\n", Teller[i].queue[Teller[i].front].id, global_clock, i);
                Teller[i].front++;
                Teller[i].served_time = 0;
                Teller[i].count--;
                if (Teller[i].front >= 10)
                {
                    Teller[i].front = 0;
                }
                if (Teller[i].status == 1)
                {
                    Teller[i].status = 0;
                }
            }
        }
        // printf("%d %d\n", Teller[0].count, Teller[1].count);
    }
}

// ��̤֤H���d�i
int shortest()
{
    int min = 11;
    int least_window;
    for (int i = 0; i < window_num; i++)
    {
        if (Teller[i].status == 2 && Teller[i].count < min)
        {
            min = Teller[i].count;
            least_window = i;
        }
    }
    return least_window;
}

void insert_Customer(int now)
{
    int minTeller = shortest();
    Teller[minTeller].queue[Teller[minTeller].rear] = people[now];
    Teller[minTeller].count++;
    Teller[minTeller].rear++;

    if (Teller[minTeller].rear >= 10)
    {
        Teller[minTeller].rear = 0;
    }
}
void reInsert(int close_Teller, int i)
{
    int minTeller = shortest();
    Teller[minTeller].queue[Teller[minTeller].rear] = Teller[close_Teller].queue[i];
    Teller[minTeller].count++;
    Teller[minTeller].rear++;
    if (Teller[minTeller].rear >= 10)
    {
        Teller[minTeller].rear = 0;
    }
}
void close(int now)
{
    int close_Teller = people[now].ser_time;
    if (Teller[close_Teller].count == 0)
    {
        Teller[close_Teller].status = 0;
    }
    else
    {
        Teller[close_Teller].status = 1;
        for (int i = Teller[close_Teller].front + 1; i < Teller[close_Teller].rear; i++)
        {
            reInsert(close_Teller, i);
        }
        Teller[close_Teller].rear = Teller[close_Teller].front + 1;
        Teller[close_Teller].count = 1;
    }
}

int empty()
{
    for (int i = 0; i < window_num; i++)
    {
        if (Teller[i].count != 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    char file_name[30];
    scanf("%s", file_name);
    FILE *input = fopen(file_name, "r+");
    fscanf(input, "%d", &window_num);
    Teller = (TellerQueue *)malloc(window_num * sizeof(TellerQueue));
    for (int i = 0; i < window_num; i++)
    {
        Teller[i].status = 2;
        Teller[i].front = 0;
        Teller[i].rear = 0;
        Teller[i].count = 0;
        Teller[i].served_time = 0;
    }
    int num = 0;
    while (!feof(input))
    {
        fscanf(input, "%s %d %d", people[num].id, &people[num].arr_time, &people[num].ser_time);
        if (strlen(people[num].id) == 0)
            continue;
        Update(num);
        if (people[num].id[0] == '#')
        {
            close(num);
        }
        else if (people[num].id[0] == '@')
        {
            Teller[people[num].ser_time].status = 2;
        }
        else
        {
            insert_Customer(num);
        }
        num++;
    }
    while (!empty())
    {
        // people[num].arr_time = global_clock + 1;
        // Update(num);
        // num++;
        Update(global_clock + 1);
    }
    fclose(input);
}