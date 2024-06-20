#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct seller {
    char sellerID[30];
    int price;
} seller;

typedef struct item_type {
    char itemName[30];
    seller sellerHeap[8];
    struct item_type* Lchild;
    struct item_type* Rchild;
} item;

item* root = NULL;

void insertMinHeap(seller sellerHeap[], char sellerID[], int price, int* heapSize) {
    if (*heapSize == 8) {
        printf("Min Heap is full. Cannot insert more sellers.\n");
        return;
    }

    // Insert new seller at the end
    strcpy(sellerHeap[*heapSize].sellerID, sellerID);
    sellerHeap[*heapSize].price = price;
    (*heapSize)++;

    int currentIndex = *heapSize - 1;
    while (currentIndex != 0 && sellerHeap[currentIndex].price < sellerHeap[(currentIndex - 1) / 2].price) {
        // Swap with the parent if the current seller's price is smaller
        seller temp = sellerHeap[currentIndex];
        sellerHeap[currentIndex] = sellerHeap[(currentIndex - 1) / 2];
        sellerHeap[(currentIndex - 1) / 2] = temp;
        currentIndex = (currentIndex - 1) / 2;
    }
}

item* insertBST(item* root, char itemName[], char sellerID[], int price) {
    if (root == NULL) {
        item* newItem = (item*)malloc(sizeof(item));
        strcpy(newItem->itemName, itemName);
        newItem->Lchild = newItem->Rchild = NULL;
        strcpy(newItem->sellerHeap[0].sellerID, sellerID);
        newItem->sellerHeap[0].price = price;
        return newItem;
    }

    if (strcmp(itemName, root->itemName) < 0)
        root->Lchild = insertBST(root->Lchild, itemName, sellerID, price);
    else if (strcmp(itemName, root->itemName) > 0)
        root->Rchild = insertBST(root->Rchild, itemName, sellerID, price);
    else {
        insertMinHeap(root->sellerHeap, sellerID, price);
    }

    return root;
}

item* searchBST(item* root, char itemName[]) {
    if (root == NULL || strcmp(itemName, root->itemName) == 0)
        return root;

    if (strcmp(itemName, root->itemName) < 0)
        return searchBST(root->Lchild, itemName);
    else
        return searchBST(root->Rchild, itemName);
}

void inorderTraversal(item* root, FILE* sort) {
    if (root != NULL) {
        inorderTraversal(root->Lchild, sort);
        fprintf(sort, "%s\n", root->itemName);
        inorderTraversal(root->Rchild, sort);
    }
}

void printSearchTableHelper(item* root, FILE* search) {
    if (root != NULL) {
        printSearchTableHelper(root->Lchild, search);
        fprintf(search, "%s\n", root->itemName);
        int i = 0;
        while (root->sellerHeap[i].price != 0) {
            fprintf(search, "%s %d\n", root->sellerHeap[i].sellerID, root->sellerHeap[i].price);
            i++;
        }
        fprintf(search, "----------------------------\n");
        printSearchTableHelper(root->Rchild, search);
    }
}

void printSearchTable(FILE* search) {
    printSearchTableHelper(root, search);
}

void buyItemHelper(item* root, char itemName[], FILE* buy) {
    if (root != NULL) {
        if (strcmp(itemName, root->itemName) < 0)
            buyItemHelper(root->Lchild, itemName, buy);
        else if (strcmp(itemName, root->itemName) > 0)
            buyItemHelper(root->Rchild, itemName, buy);
        else {
            if (root->sellerHeap[0].price != 0) {
                fprintf(buy, "%s %s %d\n", itemName, root->sellerHeap[0].sellerID, root->sellerHeap[0].price);

                int i = 0;
                while (root->sellerHeap[i + 1].price != 0) {
                    strcpy(root->sellerHeap[i].sellerID, root->sellerHeap[i + 1].sellerID);
                    root->sellerHeap[i].price = root->sellerHeap[i + 1].price;
                    i++;
                }
                root->sellerHeap[i].price = 0;

                if (root->sellerHeap[0].price == 0) {
                    // 商品沒有賣家了，從樹中刪除
                    // 這裡需要實現相應的邏輯
                    // 請確保從樹中刪除商品的相關邏輯是正確的
                }
            } else {
                fprintf(buy, "%s doesn’t exist!\n", itemName);
            }
        }
    }
}

void buyItem(char itemName[]) {
    buyItemHelper(root, itemName, stdout); // 修改 stdout 為 buy
}

int main() {
    FILE* input = fopen("input.txt", "r");
    FILE* search = fopen("searchTable.txt", "w");
    FILE* buy = fopen("buyTable.txt", "w");
    FILE* sort = fopen("sortTable.txt", "w");
    FILE* log = fopen("LogTable.txt", "w");

    char itemName[30], sellerID[30];
    int price;

    while (fscanf(input, "%s", instruction) != EOF) {
        if (strcmp(instruction, "insert") == 0) {
            fscanf(input, "%s %s %d", itemName, sellerID, &price);
            root = insertBST(root, itemName, sellerID, price);
            insertMinHeap(root->sellerHeap, sellerID, price);
        } else if (strcmp(instruction, "buy") == 0) {
            fscanf(input, "%s", itemName);
            buyItem(itemName);
        } else if (strcmp(instruction, "sort") == 0) {
            inorderTraversal(root, sort);
        } else if (strcmp(instruction, "report") == 0) {
            printSearchTable(search);
        }
    }

    fclose(input);
    fclose(search);
    fclose(buy);
    fclose(sort);
    fclose(log);

    return 0;
}
