#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define seller data structure
typedef struct SellerData {
    char sellerId[100];  // Seller ID
    int price;           // Product price
} SellerData;

// Define product data structure
typedef struct BstNode {
    char productName[100];        // Product name
    SellerData* sellers;          // Seller data
    int sellerCount;              // Number of sellers
    struct BstNode* left;         // Left child node
    struct BstNode* right;        // Right child node
} BstNode;

typedef struct ProgramState {
    int insertCount;
    int searchCount;
    int searchFailCount;
    int buyCount;
    int buyFailCount;
    int nodeCount;
    int treeHeight;
} ProgramState;

BstNode* CreateNode() {
    BstNode* root = malloc(sizeof(BstNode));
    root->sellers = NULL;
    root->sellerCount = 0;
    root->left = NULL;
    root->right = NULL;

    return root;
}

BstNode* SearchNode(BstNode* node, char* name) {
    if (node == NULL) {
        return NULL;
    }

    if (strcmp(node->productName, name) == 0) {
        return node;
    } else if (strcmp(node->productName, name) > 0) {
        return SearchNode(node->left, name);
    } else {
        return SearchNode(node->right, name);
    }
}

void MinHeapify(SellerData* arr, int size, int index) {
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int smallest = index;

    if (left < size && arr[left].price < arr[smallest].price) {
        smallest = left;
    }

    if (right < size && arr[right].price < arr[smallest].price) {
        smallest = right;
    }

    if (smallest != index) {
        SellerData temp = arr[index];
        arr[index] = arr[smallest];
        arr[smallest] = temp;
        MinHeapify(arr, size, smallest);
    }
}

void InsertSeller(BstNode* node, char* sellerId, int price) {
    SellerData* seller = malloc(sizeof(SellerData));
    strcpy(seller->sellerId, sellerId);
    seller->price = price;

    node->sellerCount++;
    node->sellers = realloc(node->sellers, sizeof(SellerData) * node->sellerCount);
    node->sellers[node->sellerCount - 1] = *seller;

    for (int i = node->sellerCount / 2 - 1; i >= 0; i--) {
        MinHeapify(node->sellers, node->sellerCount, i);
    }
}

SellerData* PopSeller(BstNode* node) {
    if (node->sellerCount == 0) {
        return NULL;
    }

    SellerData* seller = malloc(sizeof(SellerData));
    *seller = node->sellers[0];

    node->sellerCount--;
    node->sellers[0] = node->sellers[node->sellerCount];
    MinHeapify(node->sellers, node->sellerCount, 0);

    return seller;
}

void InsertNode(BstNode* root, BstNode* node) {
    if (root == NULL) {
        root = node;
        return;
    }

    if (strcmp(root->productName, node->productName) > 0) {
        if (root->left == NULL) {
            root->left = node;
        } else {
            InsertNode(root->left, node);
        }
    } else {
        if (root->right == NULL) {
            root->right = node;
        } else {
            InsertNode(root->right, node);
        }
    }
}

BstNode* RemoveNode(BstNode* root, BstNode* node) {
    if (root == NULL) {
        return NULL;
    }

    if (strcmp(root->productName, node->productName) > 0) {
        root->left = RemoveNode(root->left, node);
    } else if (strcmp(root->productName, node->productName) < 0) {
        root->right = RemoveNode(root->right, node);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            BstNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BstNode* temp = root->left;
            free(root);
            return temp;
        } else {
            BstNode* temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy(root->productName, temp->productName);
            root->right = RemoveNode(root->right, temp);
        }
    }

    return root;
}

void SortNode(BstNode* node, FILE* f) {
    if (node == NULL) {
        return;
    }

    SortNode(node->left, f);
    fprintf(f, "%s\n", node->productName);
    SortNode(node->right, f);
}

int CalculateHeight(BstNode* root) {
    if (root == NULL) {
        return 0;
    }

    int left = CalculateHeight(root->left);
    int right = CalculateHeight(root->right);

    return left > right ? left + 1 : right + 1;
}

int main() {
    char inputLine[512];

    FILE* inputFile = fopen("input.txt", "r");

    char* token;

    ProgramState state = {0};
    BstNode* root = NULL;

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
        printf("^%s\n", token);

        if (!strcmp(token, "insert")) {
            state.insertCount++;
            char* name = strtok(NULL, " ");
            char* sellerId = strtok(NULL, " ");
            int price = atoi(strtok(NULL, " "));

            if (root == NULL) {
                root = CreateNode();
                strcpy(root->productName, name);
                state.nodeCount++;
            }

            BstNode* node = SearchNode(root, name);

            if (node == NULL) {
                node = CreateNode();
                strcpy(node->productName, name);
                InsertNode(root, node);
                state.nodeCount++;
            }

            InsertSeller(node, sellerId, price);
            printf("#in %s by %s $$ = %d\n", node->productName, sellerId, price);
            printf("#%s count: %d, low: %d\n", node->productName, node->sellerCount, node->sellers[0].price);
            for (int i = 0; i < node->sellerCount; i++) {
                printf("#%d ", node->sellers[i].price);
            }
            printf("\n");
        }

        else if (!strcmp(token, "search")) {
            state.searchCount++;
            char* name = strtok(NULL, " ");
            BstNode* node = SearchNode(root, name);

            if (node == NULL) {
                printf("#Search %s, count: NULL\n", name);
                fprintf(searchFile, "%s Not found\n", name);
                state.searchFailCount++;
            } else {
                printf("#Search %s, count: %d\n", node->productName, node->sellerCount);
                fprintf(searchFile, "%s\n", node->productName);
                for (int i = 0; i < node->sellerCount; i++) {
                    fprintf(searchFile, "%s %d\n", node->sellers[i].sellerId, node->sellers[i].price);
                }
            }

            fprintf(searchFile, "----------------------------\n");
        }

        else if (!strcmp(token, "buy")) {
            state.buyCount++;
            char* name = strtok(NULL, " ");
            BstNode* node = SearchNode(root, name);

            if (node == NULL) {
                printf("#Buy %s, count: NULL\n", name);
                fprintf(buyFile, "%s Not found\n", name);
                state.buyFailCount++;
            } else {
                SellerData* seller = PopSeller(node);

                printf("#Buy %s, count: %d, low: %d\n", node->productName, node->sellerCount, node->sellers[0].price);
                fprintf(buyFile, "%s %s %d\n", node->productName, seller->sellerId, seller->price);

                if (node->sellerCount == 0) {
                    RemoveNode(root, node);
                    state.nodeCount--;
                }
            }
        }

        else if (!strcmp(token, "sort")) {
            SortNode(root, sortFile);
            fprintf(sortFile, "----------------------------\n");
        }

        else if (!strcmp(token, "report")) {
            state.treeHeight = CalculateHeight(root);
            fprintf(logFile, "insert %d\n", state.insertCount);
            fprintf(logFile, "search %d %d\n", state.searchCount, state.searchFailCount);
            fprintf(logFile, "buy %d %d\n", state.buyCount, state.buyFailCount);
            fprintf(logFile, "nodeCount %d\n", state.nodeCount);
            fprintf(logFile, "treeHeight %d\n", state.treeHeight);
            fprintf(logFile, "----------------------------\n");
        }
    }

    return 0;
}