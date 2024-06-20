#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define seller data structure
typedef struct SellerData 
{
    char sellerId[30];  // Seller ID
    int price;           // Product price
} SellerData;

// Define product data structure
typedef struct BstNode 
{
    char productName[30];        // Product name
    SellerData* sellers;          // Seller data
    int sellerCount;              // Number of sellers
    struct BstNode* left;         // Left child node
    struct BstNode* right;        // Right child node
} BstNode;

// Define program state structure
typedef struct ProgramState 
{
    int insertCount;
    int searchCount;
    int searchFailCount;
    int buyCount;
    int buyFailCount;
    int nodeCount;
    int treeHeight;
} ProgramState;

// Function to create a new node
BstNode* CreateNode(char* name, char* sellerId, int price) 
{
    BstNode* node = malloc(sizeof(BstNode));
    strcpy(node->productName, name);
    node->sellers = malloc(sizeof(SellerData));
    strcpy(node->sellers[0].sellerId, sellerId);
    node->sellers[0].price = price;
    node->sellerCount = 1;
    node->left = node->right = NULL;
    return node;
}

// Function to insert a seller into a node
void InsertSeller(BstNode* node, char* sellerId, int price) 
{
    node->sellerCount++;
    node->sellers = realloc(node->sellers, sizeof(SellerData) * node->sellerCount);
    strcpy(node->sellers[node->sellerCount - 1].sellerId, sellerId);
    node->sellers[node->sellerCount - 1].price = price;

    // Heapify the sellers
    int i = node->sellerCount - 1;
    while (i > 0 && node->sellers[(i - 1) / 2].price > node->sellers[i].price) 
    {
        // Swap with the parent if the current seller's price is smaller
        SellerData temp = node->sellers[i];
        node->sellers[i] = node->sellers[(i - 1) / 2];
        node->sellers[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// Function to pop the minimum seller from a node
SellerData PopSeller(BstNode* node) 
{
    SellerData minSeller = node->sellers[0];
    node->sellerCount--;

    if (node->sellerCount > 0) 
    {
        node->sellers[0] = node->sellers[node->sellerCount];
        // Heapify the sellers
        int i = 0;
        while (1) 
        {
            int leftChild = 2 * i + 1;
            int rightChild = 2 * i + 2;
            int smallest = i;

            if (leftChild < node->sellerCount && node->sellers[leftChild].price < node->sellers[smallest].price)
                smallest = leftChild;

            if (rightChild < node->sellerCount && node->sellers[rightChild].price < node->sellers[smallest].price)
                smallest = rightChild;

            if (smallest != i) 
            {
                // Swap the sellers
                SellerData temp = node->sellers[i];
                node->sellers[i] = node->sellers[smallest];
                node->sellers[smallest] = temp;
                i = smallest;
            } 
            else 
            {
                break;
            }
        }
    } 
    else 
    {
        free(node->sellers);
        node->sellers = NULL;
    }

    return minSeller;
}

// Function to search for a node by name
BstNode* SearchNode(BstNode* root, char* name) 
{
    if (root == NULL || strcasecmp(root->productName, name) == 0)
        return root;

    if (strcasecmp(name, root->productName) < 0)
        return SearchNode(root->left, name);
    else
        return SearchNode(root->right, name);
}

// Function to insert a node into the BST
BstNode* InsertNode(BstNode* root, char* name, char* sellerId, int price, ProgramState* state) {
    if (root == NULL) 
    {
        state->nodeCount++;
        return CreateNode(name, sellerId, price);
    }

    int compare = strcasecmp(name, root->productName);

    if (compare < 0)
        root->left = InsertNode(root->left, name, sellerId, price, state);
    else if (compare > 0)
        root->right = InsertNode(root->right, name, sellerId, price, state);
    else
        InsertSeller(root, sellerId, price);

    return root;
}

// Function to remove a node from the BST
BstNode* RemoveNode(BstNode* root, char* name, ProgramState* state) {
    if (root == NULL)
        return NULL;

    int compare = strcasecmp(name, root->productName);

    if (compare < 0)
        root->left = RemoveNode(root->left, name, state);
    else if (compare > 0)
        root->right = RemoveNode(root->right, name, state);
    else 
    {
        if (root->left == NULL && root->right == NULL) 
        {
            free(root->sellers);
            free(root);
            state->nodeCount--;
            return NULL;
        } 
        else if (root->left == NULL) 
        {
            BstNode* temp = root->right;
            free(root->sellers);
            free(root);
            state->nodeCount--;
            return temp;
        } 
        else if (root->right == NULL) 
        {
            BstNode* temp = root->left;
            free(root->sellers);
            free(root);
            state->nodeCount--;
            return temp;
        } 
        else 
        {
            BstNode* temp = root->right;
            while (temp->left != NULL)
                temp = temp->left;

            strcpy(root->productName, temp->productName);
            root->right = RemoveNode(root->right, temp->productName, state);
        }
    }

    return root;
}

// Function to perform in-order traversal and store sorted items
void SortNode(BstNode* node, FILE* f) 
{
    if (node != NULL) 
    {
        SortNode(node->left, f);
        fprintf(f, "%s\n", node->productName);
        SortNode(node->right, f);
    }
}

// Function to calculate the height of the BST
int CalculateHeight(BstNode* root) 
{
    if (root == NULL)
        return 0;

    int left = CalculateHeight(root->left);
    int right = CalculateHeight(root->right);

    return left > right ? left + 1 : right + 1;
}

// Function to generate a report
void GenerateReport(FILE* f, ProgramState* state) 
{
    fprintf(f, "insert %d\n", state->insertCount);
    fprintf(f, "search %d %d\n", state->searchCount, state->searchFailCount);
    fprintf(f, "buy %d %d\n", state->buyCount, state->buyFailCount);
    fprintf(f, "nodeCount %d\n", state->nodeCount);
    fprintf(f, "treeHeight %d\n", state->treeHeight);
}

int main() 
{
    FILE* inputFile = fopen("input.txt", "r");
    FILE* searchFile = fopen("SearchTable.txt", "w+");
    FILE* buyFile = fopen("BuyTable.txt", "w+");
    FILE* sortFile = fopen("SortTable.txt", "w+");
    FILE* logFile = fopen("LogTable.txt", "w+");

    char line[512];
    char* token;

    ProgramState state = {0};
    BstNode* root = NULL;

    while (fgets(line, sizeof(line), inputFile) != NULL) 
    {
        // Remove newline
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, " ");

        if (!strcmp(token, "insert")) 
        {
            state.insertCount++;
            char* name = strtok(NULL, " ");
            char* sellerId = strtok(NULL, " ");
            int price = atoi(strtok(NULL, " "));
            root = InsertNode(root, name, sellerId, price, &state);
        } 
        else if (!strcmp(token, "search")) 
        {
            state.searchCount++;
            char* name = strtok(NULL, " ");
            BstNode* node = SearchNode(root, name);

            if (node == NULL) 
            {
                fprintf(searchFile, "%s doesn¡¦t exist!\n", name);
                state.searchFailCount++;
            } 
            else 
            {
                fprintf(searchFile, "%s\n", node->productName);
                for (int i = 0; i < node->sellerCount; i++) 
                {
                    fprintf(searchFile, "%s %d\n", node->sellers[i].sellerId, node->sellers[i].price);
                }
            }
        } 
        else if (!strcmp(token, "buy")) 
        {
            state.buyCount++;
            char* name = strtok(NULL, " ");
            BstNode* node = SearchNode(root, name);

            if (node == NULL) 
            {
                fprintf(buyFile, "%s doesn¡¦t exist!\n", name);
                state.buyFailCount++;
            } 
            else 
            {
                SellerData seller = PopSeller(node);
                fprintf(buyFile, "%s %s %d\n", node->productName, seller.sellerId, seller.price);

                if (node->sellerCount == 0) 
                {
                    root = RemoveNode(root, node->productName, &state);
                }
            }
        } 
        else if (!strcmp(token, "sort")) 
        {
            SortNode(root, sortFile);
            fprintf(sortFile, "----------------------------\n");
        } 
        else if (!strcmp(token, "report")) 
        {
            state.treeHeight = CalculateHeight(root);
            GenerateReport(logFile, &state);
        }
    }

    // Clean up
    fclose(inputFile);
    fclose(searchFile);
    fclose(buyFile);
    fclose(sortFile);
    fclose(logFile);

    return 0;
}
