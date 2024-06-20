#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE 10

typedef struct Customer {
    char id[20];
    int arr_time;
    int ser_time;
} Customer;

typedef struct TellerQueue {
    int status;
    int front;
    int rear;
    int count;
    int current_served_time;
    Customer queue[MAX_QUEUE_SIZE];
} TellerQueue;

TellerQueue teller_queue[10]; // �̦h10?�A?���f

int global_clock = 0;

// ��l��?�C
void initQueue() {
    for (int i = 0; i < 10; i++) {
        teller_queue[i].status = 2; // ��l??
        teller_queue[i].front = 0;
        teller_queue[i].rear = 0;
        teller_queue[i].count = 0;
        teller_queue[i].current_served_time = 0;
    }
}

// ��?�A?���f
void openQueue(int index) {
    teller_queue[index].status = 2; // ���s?��
}

// ??�A?���f
void closeQueue(int index) {
    teller_queue[index].status = 0; // ??
}

// ?���̵u��?���f������
int getShortestQueueId() {
    int shortest_queue_id = 0;
    int shortest_queue_length = teller_queue[0].count;

    for (int i = 1; i < 10; i++) {
        if (teller_queue[i].count < shortest_queue_length) {
            shortest_queue_length = teller_queue[i].count;
            shortest_queue_id = i;
        }
    }

    return shortest_queue_id;
}

// ���J��?��?�C��
void insertCustomer(int index, Customer newCustomer) {
    if (teller_queue[index].count < MAX_QUEUE_SIZE) {
        teller_queue[index].queue[teller_queue[index].rear] = newCustomer;
        teller_queue[index].rear = (teller_queue[index].rear + 1) % MAX_QUEUE_SIZE;
        teller_queue[index].count++;
    }
}

// ��s?�C??
void updateQueueStatus(int new_arr_time) {
    for (int i = 0; i < 10; i++) {
        if (teller_queue[i].status == 2) {
            if (teller_queue[i].count > 0) {
                Customer serving_customer = teller_queue[i].queue[teller_queue[i].front];
                serving_customer.ser_time--;

                if (serving_customer.ser_time <= 0) {
                    // ��?����??�A?�X�H��
                    printf("%s %d %d\n", serving_customer.id, global_clock, i);
                    teller_queue[i].front = (teller_queue[i].front + 1) % MAX_QUEUE_SIZE;
                    teller_queue[i].count--;
                }

                teller_queue[i].current_served_time = serving_customer.ser_time;
            }
        }
    }

    // �b?���ݭn���u�s����???���J�s��?��̵u?�C
    // ?�ⵥ��??��
}

int main() {
    int num_tellers;
    scanf("%d", &num_tellers);

    initQueue();

    char buffer[255];
    while (fgets(buffer, 255, stdin)) {
        if (buffer[0] == '#' || buffer[0] == '@') {
            // ?�z??��?��A?���f���R�O
            int index = buffer[2] - '0'; // ?�����f??
            if (buffer[0] == '#') {
                closeQueue(index);
            } else if (buffer[0] == '@') {
                openQueue(index);
            }
        } else {
            // ?�z��?�H��
            Customer newCustomer;
            sscanf(buffer, "%s %d %d", newCustomer.id, &newCustomer.arr_time, &newCustomer.ser_time);

            // ��s?�C??
            updateQueueStatus(newCustomer.arr_time);

            int shortest_queue_id = getShortestQueueId();
            insertCustomer(shortest_queue_id, newCustomer);
        }

        global_clock++; // ��s����??
    }

    // ?�z�ѧE��?
    while (1) {
        int all_queues_empty = 1;
        for (int i = 0; i < 10; i++) {
            if (teller_queue[i].count > 0) {
                all_queues_empty = 0;
                updateQueueStatus(global_clock);
            }
        }
        if (all_queues_empty) {
            break;
        }
        global_clock++;
    }

    return 0;
}
