#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義賣家資料結構
typedef struct SELLER_TYPE {
    char SID[30]; // 賣家ID
    int PRICE;    // 商品價格
} SELLER_TYPE;

// 定義商品資料結構
typedef struct ITEM_TYPE {
    char ITEM_NAME[30];                // 商品名稱
    struct SELLER_TYPE*  SELLER_HEAP;  // 賣家資料
    int SELLER_COUNT;                  // 賣家數量
    struct ITEM_TYPE* LCHILD;          // 左子節點
    struct ITEM_TYPE* RCHILD;          // 右子節點
} ITEM_TYPE;

// 定義目前狀態結構
typedef struct state {
    int insert;      // insert 總指令個數
    int search;      // search 總指令個數
    int search_fail; // search 失敗個數
    int buy;         // buy 總指令個數
    int buy_fail;    // buy 失敗個數
    int node_num;    // BST 的 node 個數
    int height;      // BST 的 height
} State;

// 字串比較
int myCmp(char *str1, char *str2) {
    while (*str1 && tolower((unsigned char)*str1) == tolower((unsigned char)*str2)) {
        ++str1;
        ++str2;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

// 創建新節點
ITEM_TYPE* createNode() {
    ITEM_TYPE* root = (ITEM_TYPE*)malloc(sizeof(ITEM_TYPE));
    root->SELLER_HEAP = NULL;
    root->SELLER_COUNT = 0;
    root->LCHILD = NULL;
    root->RCHILD = NULL;

    return root;
}

// 遞迴搜尋
ITEM_TYPE* searchNode(ITEM_TYPE* node, char name[]) {
    if (node == NULL) {
        return NULL; // 直到底 都沒有符合的節點
    }
    if (myCmp(node->ITEM_NAME, name) == 0) {
        return node; // 找到就回傳整串商品節點資訊
    }
    if (myCmp(node->ITEM_NAME, name) > 0) {
        return searchNode(node->LCHILD, name);
    }
    else {
        return searchNode(node->RCHILD, name);
    }
}

// 整理商品的 heap 陣列
void minHeap(SELLER_TYPE* heap, int size, int index) { // 賣家資訊陣列、總數、要移動的商品位置
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int small = index; //紀錄 index 與 左右節點誰比較小

    if (left < size && heap[left].PRICE < heap[small].PRICE) {
        small = left;
    }

    if (right < size && heap[right].PRICE < heap[small].PRICE) {
        small = right;
    }

    if (small != index) {
        // 如果子節點更小 與 index 交換位置 直到整理結束
        SELLER_TYPE temp = heap[index];
        heap[index] = heap[small];
        heap[small] = temp;
        minHeap(heap, size, small);
    }
}

// 插入新賣家
void insertSeller(ITEM_TYPE* node, char* sellerId, int price) {
    SELLER_TYPE* seller = malloc(sizeof(SELLER_TYPE));
    strcpy(seller->SID, sellerId);
    seller->PRICE = price;

    node->SELLER_COUNT++; //商品的賣家人數+1
    node->SELLER_HEAP = realloc(node-> SELLER_HEAP, sizeof(SELLER_TYPE) * node->SELLER_COUNT);
    node->SELLER_HEAP[node->SELLER_COUNT - 1] = *seller;

    for (int i = node->SELLER_COUNT/2-1; i >= 0; i--) {
        minHeap(node->SELLER_HEAP, node->SELLER_COUNT, i);
    }
}

// 移出賣家
SELLER_TYPE* popSeller(ITEM_TYPE* node) {
    if (node->SELLER_COUNT == 0) {
        return NULL;
    }
    SELLER_TYPE* seller = (SELLER_TYPE*)malloc(sizeof(SELLER_TYPE));
    *seller = node->SELLER_HEAP[0];
    
    node->SELLER_COUNT--;
    node->SELLER_HEAP[0] = node->SELLER_HEAP[node->SELLER_COUNT];
    minHeap(node->SELLER_HEAP, node->SELLER_COUNT, 0);
    
    return seller;
}

// 遞迴插入商品節點
ITEM_TYPE* insertNode(ITEM_TYPE* root, ITEM_TYPE* node) {
    if (root == NULL) {
        root = node;
        return root;
    }
    if (myCmp(root->ITEM_NAME, node->ITEM_NAME) > 0) {
        root->LCHILD = insertNode(root->LCHILD, node);
    }
    else {
        root->RCHILD = insertNode(root->RCHILD, node);
    }
    return root;
}

// 移除商品節點
ITEM_TYPE* removeNode(ITEM_TYPE* root, ITEM_TYPE* node) {
    if (root == NULL) { // 找不到商品
        return NULL;
    }
    if (myCmp(root->ITEM_NAME, node->ITEM_NAME) > 0) {
        root->LCHILD = removeNode(root->LCHILD, node);
    }
    else if (myCmp(root->ITEM_NAME, node->ITEM_NAME) < 0) {
        root->RCHILD = removeNode(root->RCHILD, node);
    }
    else {
        // 找到商品
        if (root->LCHILD == NULL && root->RCHILD == NULL) {
            // 無子節點 直接刪
            free(root);
            return NULL;
        }
        else if (root->LCHILD == NULL) {
            // 無左子樹 讓右子數取代原本的位
            ITEM_TYPE* temp = root->RCHILD;
            free(root);
            return temp;
        }
        else if (root->RCHILD == NULL) {
            // 無右子樹 讓左子樹取代原本的位
            ITEM_TYPE* temp = root->LCHILD;
            free(root);
            return temp;
        }
        else {
            // 兩子樹都在 找右子樹中的最小節點取代原本的位
            ITEM_TYPE* temp = root->RCHILD;
            while (temp->LCHILD != NULL) {
                temp = temp->LCHILD;
            }
            strcpy(root->ITEM_NAME, temp->ITEM_NAME);
            root->SELLER_HEAP = temp->SELLER_HEAP;
            root->SELLER_COUNT = temp->SELLER_COUNT;
            root->RCHILD = removeNode(root->RCHILD, temp);
        }
    }
    return root;
}

void SortNode(ITEM_TYPE* node, FILE* f) {
    if (node == NULL) {
        return;
    }
    
    SortNode(node->LCHILD, f);
    fprintf(f, "%s\n", node->ITEM_NAME);
    SortNode(node->RCHILD, f);
}

// 遞迴計算 BST 高度
int calculateHeight(ITEM_TYPE* node) {
    if (node == NULL) {
        return 0;
    }
    int leftHeight = calculateHeight(node->LCHILD);
    int rightHeight = calculateHeight(node->RCHILD);

    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

// 印出 BST
void printBST(ITEM_TYPE* node) {
    if (node == NULL) {
        return;
    }
    printBST(node->LCHILD);
    printf("%s %d ", node->ITEM_NAME, node->SELLER_COUNT);
    for (int i = 0; i < node->SELLER_COUNT; i++) {
        printf("%s %d ", node->SELLER_HEAP[i].SID, node->SELLER_HEAP[i].PRICE);
    }
    printf("\n");
    printBST(node->RCHILD);
}

// 釋放 BST 節點
void freeBST(ITEM_TYPE* node) {
    if (node == NULL) {
        return;
    }
    freeBST(node->LCHILD);
    freeBST(node->RCHILD);
    free(node->SELLER_HEAP);
    free(node);
}

int main() {
    char inputLine[512];
    
    FILE* inputFile = fopen("input.txt", "r");
    
    char* token;
    
    State state = {0};
    ITEM_TYPE* root = NULL;
    
    FILE* searchFile = fopen("SearchTable.txt", "w+");
    FILE* buyFile = fopen("BuyTable.txt", "w+");
    FILE* sortFile = fopen("SortTable.txt", "w+");
    FILE* logFile = fopen("LogTable.txt", "w+");
    
    while (!feof(inputFile)) {
        fgets(inputLine, 512, inputFile);
        // Remove newline
        if (inputLine[strlen(inputLine) - 1] == '\n') {
            inputLine[strlen(inputLine) - 1] = '\0';
        }
        token = strtok(inputLine, " ");
//      printf("^%s\n", token);
        
        if (!strcmp(token, "insert")) {
            state.insert++;
            char* name = strtok(NULL, " ");
            char* sellerId = strtok(NULL, " ");
            int price = atoi(strtok(NULL, " "));
            
            if (root == NULL) {
                root = createNode();
                strcpy(root->ITEM_NAME, name);
                state.node_num++;
            }
            
            ITEM_TYPE* node = searchNode(root, name);
            
            if (node == NULL) {
                node = createNode();
                strcpy(node->ITEM_NAME, name);
                root = insertNode(root, node);
                state.node_num++;
            }
            
            insertSeller(node, sellerId, price);
        }
        
        else if (!strcmp(token, "search")) {
            state.search++;
            char* name = strtok(NULL, " ");
            ITEM_TYPE* node = searchNode(root, name);
            
            if (node == NULL) {
//              printf("#Search %s, count: NULL\n", name);
                fprintf(searchFile, "%s doesn’t exist!\n", name);
                state.search_fail++;
            } else {
//              printf("#Search %s, count: %d\n", node->ITEM_NAME, node->SELLER_COUNT);
                fprintf(searchFile, "%s\n", node->ITEM_NAME);
                for (int i = 0; i < node->SELLER_COUNT; i++) {
                    fprintf(searchFile, "%s %d\n", node->SELLER_HEAP[i].SID, node->SELLER_HEAP[i].PRICE);
                }
            }
            fprintf(searchFile, "----------------------------\n");
        }
        
        else if (!strcmp(token, "buy")) {
            state.buy++;
            char* name = strtok(NULL, " ");
            ITEM_TYPE* node = searchNode(root, name);
            
            if (node == NULL) {
//              printf("#Buy %s, count: NULL\n", name);
                fprintf(buyFile, "%s doesn’t exist!\n", name);
                state.buy_fail++;
            } else {
                SELLER_TYPE* seller = popSeller(node);
                
//              printf("#Buy %s, count: %d, low: %d\n", node->ITEM_NAME, node->SELLER_COUNT, node->SELLER_HEAP[0].PRICE);
                fprintf(buyFile, "%s %s %d\n", node->ITEM_NAME, seller->SID, seller->PRICE);
                
                if (node->SELLER_COUNT == 0) {
                    root = removeNode(root, node);
                    state.node_num--;
                }
            }
        }
        
        else if (!strcmp(token, "sort")) {
            SortNode(root, sortFile);
            fprintf(sortFile, "----------------------------\n");
        }
        
        else if (!strcmp(token, "report")) {
            state.height = calculateHeight(root);
            fprintf(logFile, "insert %d\n", state.insert);
            fprintf(logFile, "search %d %d\n", state.search, state.search_fail);
            fprintf(logFile, "buy %d %d\n", state.buy, state.buy_fail);
            fprintf(logFile, "nodeCount %d\n", state.node_num);
            fprintf(logFile, "treeHeight %d\n", state.height);
            fprintf(logFile, "----------------------------\n");
        }
    }
    
    return 0;
}