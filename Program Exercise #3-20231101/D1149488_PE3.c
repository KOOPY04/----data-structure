#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_QUEUE_SIZE 10    // 排隊的最大容量為10
int global_clock;            // 全球時間
int teller_num;              // 櫃檯的數量

typedef struct Customer
{
    char id[20];  // 客戶 id
    int arr_time; // 客戶到達銀行時間
    int ser_time; // 客戶完成業務要花多少時間
} Customer;

typedef struct TellerQueue
{
    int status;                     // status, 0: close, 1:只剩下一位客戶，即將close, 2: open
    int front;                      // front pointer，指向第一個element 的index
    int rear;                       // rear pointer，指向最後一個element 的下一個index
    int count;                      // queue 中在排隊的人?
    int current_served_time;        // 目前在處理業務的客人已經花的時間
    Customer queue[MAX_QUEUE_SIZE]; // 以 array 型式模擬Circular Queue。
} TellerQueue;

Customer people[10000];             // 客戶的信息
TellerQueue *teller;                // 櫃檯的信息

// 讀取客戶信息
int ReadOneRecord(FILE *input, int n)
{
    fscanf(input,"%s %d %d" ,people[n].id, &people[n].arr_time, &people[n].ser_time);
}

// 初始化所有銀行櫃檯的狀態
void initTeller()
{
    for (int i = 0; i < teller_num; i++) 
    {
        teller[i].status = 2;               // 將櫃檯的狀態設定為 2，表示櫃檯開啟
        teller[i].front = 0;                // 初始化 front ，指向排隊客戶的第一位
        teller[i].rear = 0;                 // 初始化 rear ，指向排隊客戶的下一位（空位置）
        teller[i].count = 0;                // 初始化排隊客戶的數量，一開始都是 0
        teller[i].current_served_time = 0;  // 初始化目前正在服務客戶所花的時間，一開始為 0
    }
}

// 更新櫃檯的狀態，處理排隊中的客戶，並模擬他們的服務過程
void UpdateQueueStatus(int n)
{
    int NowTime;

    // 客戶到達時間不為0，則使用客戶的到達時間，否則使用函數的參數n
    if(people[n].arr_time != 0)
    {
        NowTime = people[n].arr_time;
    }
    else
    {
        NowTime = n;
    }

    // global_clock達到NowTime，跳出迴圈
    while (global_clock != NowTime)
    {
        global_clock++;     // 增加global_clock

        // 遍歷所有櫃檯
        for(int i = 0; i < teller_num; i++)
        {

            // 如果櫃檯沒有客戶在排隊或者櫃檯已經關閉，則繼續下一個櫃檯。
            if(teller[i].count == 0 || teller[i].status == 0)
            {
                continue;
            }
            teller[i].current_served_time++;    // 增加當前正在服務的客戶已經花的時間

            // 如果當前正在服務的客戶已經完成服務，則：
            if(teller[i].current_served_time >= teller[i].queue[teller[i].front].ser_time)
            {
                printf("%s %d %d\n" ,teller[i].queue[teller[i].front].id, global_clock, i);     // 輸出客戶的信息
                teller[i].front++;      // 移除已經完成服務的客戶， front 向前移動
                teller[i].current_served_time = 0;      // 重置正在服務的時間為0
                teller[i].count--;      // 排隊的客戶數量減少

                // 如果 front 超過排隊最大值，循環回到陣列開頭
                if(teller[i].front >= 10)
                {
                    teller[i].front = 0;
                }

                // 如果櫃檯只剩下一位客戶(1)，將其設置為關閉（0）
                if(teller[i].status == 1)
                {
                    teller[i].status == 0;
                }
            }
        }
    }
}

// 找到排隊客戶最少的櫃檯
int GetTheShortestQueue()
{
    int min = 11;   // 初始化最少排隊人數(min)為 11
    int ShortTeller;    // 最少排隊客戶的櫃檯

    // 尋找排隊客戶最少的櫃檯
    for(int i = 0; i < teller_num; i++)
    {
        // 如果櫃檯是開啟，並且它的排隊客戶數量比目前最小值 min 還小，則更新 min 和 ShortTeller。
        if(teller[i].status == 2 && teller[i].count < min)
        {
            min = teller[i].count;      // 更新最小值為該櫃檯的排隊客戶數量
            ShortTeller = i;        // 更新最少排隊客戶的櫃檯
        }
    }
    return ShortTeller;     // 返回最少排隊客戶的櫃檯
}

// 櫃檯關閉後，將其未完成服務的客戶轉移到排隊人數最少的櫃檯
void reStore(int CloseTeller, int i)
{
    int min_teller = GetTheShortestQueue();     // 找到排隊人數最少的櫃檯
    teller[min_teller].queue[teller[min_teller].rear] = teller[CloseTeller].queue[i];       // 將關閉櫃檯之後的客戶轉移到開啟並對伍最短的櫃台排隊
    teller[min_teller].count++;     // 增加隊伍最短的櫃檯排隊人數
    teller[min_teller].rear++;      //更新隊伍最短的櫃檯的 rear

    // 如果 rear 超過排隊最大值，循環回到陣列開頭
    if (teller[min_teller].rear >= 10)
    {
        teller[min_teller].rear = 0;
    }
}

// 關閉一個指定櫃檯，並在必要時將該櫃檯中未服務的客戶轉移到其他櫃檯
void CloseQueue(int n)
{
    int CloseTeller = people[n].ser_time;

    // 判斷該櫃檯中沒有客戶在排隊
    if(teller[CloseTeller].count == 0)
    {
        teller[CloseTeller].status = 0;     // 將櫃檯直接設為關閉狀態
    }
    else
    {
        teller[CloseTeller].status = 1;     // 將櫃檯設為即將關閉的狀態

        // 遍歷該櫃檯中未服務的客戶，並將他們轉移到其他櫃檯
        for(int i = teller[CloseTeller].front + 1; i != teller[CloseTeller].rear; i++)
        {
            reStore(CloseTeller, i);        // 呼叫 reStore function，將客戶轉移到其他櫃檯

            // 如果 i 到達陣列的尾部，則將它重設為陣列的起始
            if(i == 9)
            {
                i = -1;     // 在下一輪迴圈遞增後，會變成 0
            }
        }
        teller[CloseTeller].rear = teller[CloseTeller].front + 1;       // 重設該櫃檯的 rear 
        teller[CloseTeller].count = 1;      // 該櫃檯僅有一個處理中客戶
    }
}

// 將新的客戶插入排隊
void InsertNewCustomer(int n)
{
    int min_teller = GetTheShortestQueue();     // 找到排隊人數最少的櫃檯
    teller[min_teller].queue[teller[min_teller].rear] = people[n];      // 客戶插入到選定的櫃檯的排隊隊列
    teller[min_teller].count++;     // 增加櫃檯中排隊客戶
    teller[min_teller].rear++;      // 增加櫃檯的 rear ，讓它指向排隊隊列的下一個位置

    // 如果 rear 超過排隊最大值，循環回到陣列開頭
    if (teller[min_teller].rear >= 10)
    {
        teller[min_teller].rear = 0;
    }

}

// 判斷是否開啟或關閉櫃台
void judge(int n)
{
    // 檢查input第一個字符
    if(people[n].id[0] == '#')      // 第一個字符是 '#'，則執行關閉櫃檯
    {
        CloseQueue(n);
    }
    else if(people[n].id[0] == '@')     // 第一個字符是 '@'，則執行開啟櫃檯
    {
        teller[people[n].ser_time].status = 2;      // 以客戶的 'ser_time' 字段來確定要開啟哪個櫃檯
    }
    else
    {
        InsertNewCustomer(n);       // 如果第一個字符不是 '#' 也不是 '@'，則執行 InsertNewCustomer function
    }
}

// 檢查是否所有銀行櫃檯都已經沒有客戶排隊
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
    char file[25];      // 用於存儲輸入文件的名稱
    printf("輸入檔案名稱：");    // 請使用者輸入文件名稱   
    scanf("%s" ,file);          
    FILE *input = fopen(file ,"r+");        //讀取檔案
    fscanf(input, "%d", &teller_num);       
    teller = (TellerQueue*)malloc(teller_num * sizeof(TellerQueue));        // 根據 teller_num 的數量，動態分配記憶體以存儲銀行櫃檯的信息
    initTeller();       // 初始化所有的銀行櫃檯
    int n = 0;      // 客戶數組

    // 檢查文件是否結束
    while(!feof(input))
    {
        ReadOneRecord(input, n);
        // 如果客戶ID長度為0，表示客戶記錄為空，跳過這條記錄
        if(strlen(people[n].id) == 0)
        {
            continue;       // 從文件中讀取客戶記錄，存儲在 people 中的第 n 個位置
        }
        UpdateQueueStatus(n);       // 更新排隊的客戶狀態和櫃檯的狀態
        judge(n);       // 處理櫃檯的開啟或關閉
        n++;        // 增加 n，處理下一個客戶
    }

    // 檢查是否還有客戶在排隊或處理中
    while(!empty())
    {
        UpdateQueueStatus(global_clock + 1);        // 持續更新客戶排隊和櫃檯狀態，直到所有客戶完成服務
    }
    fclose(input);      // 關閉文件，釋放空間
}