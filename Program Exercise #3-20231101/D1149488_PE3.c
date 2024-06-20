#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_QUEUE_SIZE 10    // �ƶ����̤j�e�q��10
int global_clock;            // ���y�ɶ�
int teller_num;              // �d�i���ƶq

typedef struct Customer
{
    char id[20];  // �Ȥ� id
    int arr_time; // �Ȥ��F�Ȧ�ɶ�
    int ser_time; // �Ȥ᧹���~�ȭn��h�֮ɶ�
} Customer;

typedef struct TellerQueue
{
    int status;                     // status, 0: close, 1:�u�ѤU�@��Ȥ�A�Y�Nclose, 2: open
    int front;                      // front pointer�A���V�Ĥ@��element ��index
    int rear;                       // rear pointer�A���V�̫�@��element ���U�@��index
    int count;                      // queue ���b�ƶ����H?
    int current_served_time;        // �ثe�b�B�z�~�Ȫ��ȤH�w�g�᪺�ɶ�
    Customer queue[MAX_QUEUE_SIZE]; // �H array ��������Circular Queue�C
} TellerQueue;

Customer people[10000];             // �Ȥ᪺�H��
TellerQueue *teller;                // �d�i���H��

// Ū���Ȥ�H��
int ReadOneRecord(FILE *input, int n)
{
    fscanf(input,"%s %d %d" ,people[n].id, &people[n].arr_time, &people[n].ser_time);
}

// ��l�ƩҦ��Ȧ��d�i�����A
void initTeller()
{
    for (int i = 0; i < teller_num; i++) 
    {
        teller[i].status = 2;               // �N�d�i�����A�]�w�� 2�A����d�i�}��
        teller[i].front = 0;                // ��l�� front �A���V�ƶ��Ȥ᪺�Ĥ@��
        teller[i].rear = 0;                 // ��l�� rear �A���V�ƶ��Ȥ᪺�U�@��]�Ŧ�m�^
        teller[i].count = 0;                // ��l�Ʊƶ��Ȥ᪺�ƶq�A�@�}�l���O 0
        teller[i].current_served_time = 0;  // ��l�ƥثe���b�A�ȫȤ�Ҫ᪺�ɶ��A�@�}�l�� 0
    }
}

// ��s�d�i�����A�A�B�z�ƶ������Ȥ�A�ü����L�̪��A�ȹL�{
void UpdateQueueStatus(int n)
{
    int NowTime;

    // �Ȥ��F�ɶ�����0�A�h�ϥΫȤ᪺��F�ɶ��A�_�h�ϥΨ�ƪ��Ѽ�n
    if(people[n].arr_time != 0)
    {
        NowTime = people[n].arr_time;
    }
    else
    {
        NowTime = n;
    }

    // global_clock�F��NowTime�A���X�j��
    while (global_clock != NowTime)
    {
        global_clock++;     // �W�[global_clock

        // �M���Ҧ��d�i
        for(int i = 0; i < teller_num; i++)
        {

            // �p�G�d�i�S���Ȥ�b�ƶ��Ϊ��d�i�w�g�����A�h�~��U�@���d�i�C
            if(teller[i].count == 0 || teller[i].status == 0)
            {
                continue;
            }
            teller[i].current_served_time++;    // �W�[��e���b�A�Ȫ��Ȥ�w�g�᪺�ɶ�

            // �p�G��e���b�A�Ȫ��Ȥ�w�g�����A�ȡA�h�G
            if(teller[i].current_served_time >= teller[i].queue[teller[i].front].ser_time)
            {
                printf("%s %d %d\n" ,teller[i].queue[teller[i].front].id, global_clock, i);     // ��X�Ȥ᪺�H��
                teller[i].front++;      // �����w�g�����A�Ȫ��Ȥ�A front �V�e����
                teller[i].current_served_time = 0;      // ���m���b�A�Ȫ��ɶ���0
                teller[i].count--;      // �ƶ����Ȥ�ƶq���

                // �p�G front �W�L�ƶ��̤j�ȡA�`���^��}�C�}�Y
                if(teller[i].front >= 10)
                {
                    teller[i].front = 0;
                }

                // �p�G�d�i�u�ѤU�@��Ȥ�(1)�A�N��]�m�������]0�^
                if(teller[i].status == 1)
                {
                    teller[i].status == 0;
                }
            }
        }
    }
}

// ���ƶ��Ȥ�̤֪��d�i
int GetTheShortestQueue()
{
    int min = 11;   // ��l�Ƴֱ̤ƶ��H��(min)�� 11
    int ShortTeller;    // �ֱ̤ƶ��Ȥ᪺�d�i

    // �M��ƶ��Ȥ�̤֪��d�i
    for(int i = 0; i < teller_num; i++)
    {
        // �p�G�d�i�O�}�ҡA�åB�����ƶ��Ȥ�ƶq��ثe�̤p�� min �٤p�A�h��s min �M ShortTeller�C
        if(teller[i].status == 2 && teller[i].count < min)
        {
            min = teller[i].count;      // ��s�̤p�Ȭ����d�i���ƶ��Ȥ�ƶq
            ShortTeller = i;        // ��s�ֱ̤ƶ��Ȥ᪺�d�i
        }
    }
    return ShortTeller;     // ��^�ֱ̤ƶ��Ȥ᪺�d�i
}

// �d�i������A�N�䥼�����A�Ȫ��Ȥ��ಾ��ƶ��H�Ƴ̤֪��d�i
void reStore(int CloseTeller, int i)
{
    int min_teller = GetTheShortestQueue();     // ���ƶ��H�Ƴ̤֪��d�i
    teller[min_teller].queue[teller[min_teller].rear] = teller[CloseTeller].queue[i];       // �N�����d�i���᪺�Ȥ��ಾ��}�Ҩù��̵u���d�x�ƶ�
    teller[min_teller].count++;     // �W�[����̵u���d�i�ƶ��H��
    teller[min_teller].rear++;      //��s����̵u���d�i�� rear

    // �p�G rear �W�L�ƶ��̤j�ȡA�`���^��}�C�}�Y
    if (teller[min_teller].rear >= 10)
    {
        teller[min_teller].rear = 0;
    }
}

// �����@�ӫ��w�d�i�A�æb���n�ɱN���d�i�����A�Ȫ��Ȥ��ಾ���L�d�i
void CloseQueue(int n)
{
    int CloseTeller = people[n].ser_time;

    // �P�_���d�i���S���Ȥ�b�ƶ�
    if(teller[CloseTeller].count == 0)
    {
        teller[CloseTeller].status = 0;     // �N�d�i�����]���������A
    }
    else
    {
        teller[CloseTeller].status = 1;     // �N�d�i�]���Y�N���������A

        // �M�����d�i�����A�Ȫ��Ȥ�A�ñN�L���ಾ���L�d�i
        for(int i = teller[CloseTeller].front + 1; i != teller[CloseTeller].rear; i++)
        {
            reStore(CloseTeller, i);        // �I�s reStore function�A�N�Ȥ��ಾ���L�d�i

            // �p�G i ��F�}�C�������A�h�N�����]���}�C���_�l
            if(i == 9)
            {
                i = -1;     // �b�U�@���j�黼�W��A�|�ܦ� 0
            }
        }
        teller[CloseTeller].rear = teller[CloseTeller].front + 1;       // ���]���d�i�� rear 
        teller[CloseTeller].count = 1;      // ���d�i�Ȧ��@�ӳB�z���Ȥ�
    }
}

// �N�s���Ȥᴡ�J�ƶ�
void InsertNewCustomer(int n)
{
    int min_teller = GetTheShortestQueue();     // ���ƶ��H�Ƴ̤֪��d�i
    teller[min_teller].queue[teller[min_teller].rear] = people[n];      // �Ȥᴡ�J���w���d�i���ƶ����C
    teller[min_teller].count++;     // �W�[�d�i���ƶ��Ȥ�
    teller[min_teller].rear++;      // �W�[�d�i�� rear �A�������V�ƶ����C���U�@�Ӧ�m

    // �p�G rear �W�L�ƶ��̤j�ȡA�`���^��}�C�}�Y
    if (teller[min_teller].rear >= 10)
    {
        teller[min_teller].rear = 0;
    }

}

// �P�_�O�_�}�ҩ������d�x
void judge(int n)
{
    // �ˬdinput�Ĥ@�Ӧr��
    if(people[n].id[0] == '#')      // �Ĥ@�Ӧr�ŬO '#'�A�h���������d�i
    {
        CloseQueue(n);
    }
    else if(people[n].id[0] == '@')     // �Ĥ@�Ӧr�ŬO '@'�A�h����}���d�i
    {
        teller[people[n].ser_time].status = 2;      // �H�Ȥ᪺ 'ser_time' �r�q�ӽT�w�n�}�ҭ����d�i
    }
    else
    {
        InsertNewCustomer(n);       // �p�G�Ĥ@�Ӧr�Ť��O '#' �]���O '@'�A�h���� InsertNewCustomer function
    }
}

// �ˬd�O�_�Ҧ��Ȧ��d�i���w�g�S���Ȥ�ƶ�
bool empty()
{
    for (int i = 0; i < teller_num; i++)
    {
        if (teller[i].count != 0)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    char file[25];      // �Ω�s�x��J��󪺦W��
    printf("��J�ɮצW�١G");    // �ШϥΪ̿�J���W��   
    scanf("%s" ,file);          
    FILE *input = fopen(file ,"r+");        //Ū���ɮ�
    fscanf(input, "%d", &teller_num);       
    teller = (TellerQueue*)malloc(teller_num * sizeof(TellerQueue));        // �ھ� teller_num ���ƶq�A�ʺA���t�O����H�s�x�Ȧ��d�i���H��
    initTeller();       // ��l�ƩҦ����Ȧ��d�i
    int n = 0;      // �Ȥ�Ʋ�

    // �ˬd���O�_����
    while(!feof(input))
    {
        ReadOneRecord(input, n);
        // �p�G�Ȥ�ID���׬�0�A��ܫȤ�O�����šA���L�o���O��
        if(strlen(people[n].id) == 0)
        {
            continue;       // �q���Ū���Ȥ�O���A�s�x�b people ������ n �Ӧ�m
        }
        UpdateQueueStatus(n);       // ��s�ƶ����Ȥ᪬�A�M�d�i�����A
        judge(n);       // �B�z�d�i���}�ҩ�����
        n++;        // �W�[ n�A�B�z�U�@�ӫȤ�
    }

    // �ˬd�O�_�٦��Ȥ�b�ƶ��γB�z��
    while(!empty())
    {
        UpdateQueueStatus(global_clock + 1);        // �����s�Ȥ�ƶ��M�d�i���A�A����Ҧ��Ȥ᧹���A��
    }
    fclose(input);      // �������A����Ŷ�
}