#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct seller
{
    char sellerID[30];
    int price;
} seller;

typedef struct item_type
{
    char itemName[30];
    seller sellerHeap[8];
    int sellerHeapSize;
    struct item_type *Lchild;
    struct item_type *Rchild;
} item;

typedef struct buyRecord
{
    char itemName[30];
    char sellerID[30];
    int price;
} buyRecord;

typedef struct sortRecord
{
    char itemName[30];
} sortRecord;

sortRecord sortTable[100]; // Assuming a fixed size for sortTable, adjust as needed
int sortTableSize = 0;

buyRecord buyTable[100]; // Assuming a fixed size for buyTable, adjust as needed
int buyTableSize = 0;

item Item;
seller Seller;
item *root = NULL;
char instruction[8];
int insert = 0;
int search = 0;
int searchFail = 0;
int buy = 0;
int buyFail = 0;
int nodeCount = 0;

void minHeapify(seller sellerHeap[], int size, int index)
{
    int smallest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < size && sellerHeap[leftChild].price < sellerHeap[smallest].price)
        smallest = leftChild;

    if (rightChild < size && sellerHeap[rightChild].price < sellerHeap[smallest].price)
        smallest = rightChild;

    if (smallest != index) {
        // Swap the seller information
        seller temp = sellerHeap[index];
        sellerHeap[index] = sellerHeap[smallest];
        sellerHeap[smallest] = temp;

        // Recursively heapify the affected subtree
        minHeapify(sellerHeap, size, smallest);
    }
}

void insertMinHeap(item *root)
{
    if (root->sellerHeapSize == 8)
    {
        printf("Min Heap is full. Cannot insert more sellers.\n");
        return;
    }

    // Insert new seller at the end
    strcpy(root->sellerHeap[root->sellerHeapSize].sellerID, Seller.sellerID);
    root->sellerHeap[root->sellerHeapSize].price = Seller.price;
    (root->sellerHeapSize)++;

    int currentIndex = root->sellerHeapSize - 1;
    while (currentIndex != 0 && root->sellerHeap[currentIndex].price < root->sellerHeap[(currentIndex - 1) / 2].price)
    {
        // Swap with the parent if the current seller's price is smaller
        seller temp = root->sellerHeap[currentIndex];
        root->sellerHeap[currentIndex] = root->sellerHeap[(currentIndex - 1) / 2];
        root->sellerHeap[(currentIndex - 1) / 2] = temp;
        currentIndex = (currentIndex - 1) / 2;
    }
    minHeapify(root->sellerHeap, root->sellerHeapSize, currentIndex);
}

item *insertBST(item *root)
{
    if (root == NULL)
    {
        nodeCount++;
        // Create a new item node
        item *newItem = (item *)malloc(sizeof(item));
        strcpy(newItem->itemName, Item.itemName);
        newItem->Lchild = newItem->Rchild = NULL;
        newItem->sellerHeapSize = 1; // Initialize heap size
        strcpy(newItem->sellerHeap[0].sellerID, Seller.sellerID);
        newItem->sellerHeap[0].price = Seller.price;
        return newItem;
    }

    else
    {
        // Compare the item names
        int itemNameComparison = strcasecmp(Item.itemName, root->itemName);

        if (itemNameComparison < 0)
        {
            // If the item name is smaller, go to the left subtree
            root->Lchild = insertBST(root->Lchild);
        }
        else if (itemNameComparison > 0)
        {
            // If the item name is larger, go to the right subtree
            root->Rchild = insertBST(root->Rchild);
        }
        else
        {
            // Item with the same name found
            // Check if the seller ID is the same
            int sellerIDComparison = strcmp(Seller.sellerID, root->sellerHeap[root->sellerHeapSize - 1].sellerID);

            if (sellerIDComparison != 0)
            {
                // If the seller ID is different, insert into the min heap
                if (root->sellerHeapSize == 8)
                {
                    printf("Min Heap is full. Cannot insert more sellers.\n");
                    return root;
                }

                strcpy(root->sellerHeap[root->sellerHeapSize].sellerID, Seller.sellerID);
                root->sellerHeap[root->sellerHeapSize].price = Seller.price;
                (root->sellerHeapSize)++;

                int currentIndex = root->sellerHeapSize - 1;
                while (currentIndex != 0 && root->sellerHeap[currentIndex].price < root->sellerHeap[(currentIndex - 1) / 2].price)
                {
                    // Swap with the parent if the current seller's price is smaller
                    seller temp = root->sellerHeap[currentIndex];
                    root->sellerHeap[currentIndex] = root->sellerHeap[(currentIndex - 1) / 2];
                    root->sellerHeap[(currentIndex - 1) / 2] = temp;
                    currentIndex = (currentIndex - 1) / 2;
                }
                minHeapify(root->sellerHeap, root->sellerHeapSize, currentIndex);
            }
            // else: Seller with the same ID already exists, do nothing
        }
    }
    return root;
}

item *searchBST(item *root, char itemName[])
{
    if (root == NULL || strcasecmp(itemName, root->itemName) == 0)
    {
        return root;
    }

    // If the item name is smaller, go to the left subtree
    if (strcasecmp(itemName, root->itemName) < 0)
    {
        return searchBST(root->Lchild, itemName);
    }

    // If the item name is larger, go to the right subtree
    return searchBST(root->Rchild, itemName);
}

void searchAndPrint(item *root, char itemName[], FILE *Search)
{
    item *result = searchBST(root, itemName);

    if (result == NULL)
    {
        // Item not found
        fprintf(Search, "%s doesn't exist!\n", itemName);
        fprintf(Search, "----------------------------\n");
        searchFail++;
        search++;
    }
    else
    {
        // Print item details
        fprintf(Search, "%s\n", result->itemName);
        for (int i = 0; i < result->sellerHeapSize; ++i)
        {
            fprintf(Search, "%s %d\n", result->sellerHeap[i].sellerID, result->sellerHeap[i].price);
        }
        search++;
        fprintf(Search, "----------------------------\n");
    }
}

item *deleteNode(item *root, char itemName[])
{
    if (root == NULL)
    {
        // Item not found
        return root;
    }

    int itemNameComparison = strcasecmp(itemName, root->itemName);

    if (itemNameComparison < 0)
    {
        // If the item name is smaller, go to the left subtree
        root->Lchild = deleteNode(root->Lchild, itemName);
    }
    else if (itemNameComparison > 0)
    {
        // If the item name is larger, go to the right subtree
        root->Rchild = deleteNode(root->Rchild, itemName);
    }
    else
    {
        // Item found

        // Check if the item has sellers
        if (root->sellerHeapSize > 0)
        {
            nodeCount--;
            return root; // Item has sellers, do not delete
        }

        // No sellers, delete the item node
        if (root->Lchild == NULL)
        {
            nodeCount--;
            item *temp = root->Rchild;
            free(root);
            return temp;
        }
        else if (root->Rchild == NULL)
        {
            nodeCount--;
            item *temp = root->Lchild;
            free(root);
            return temp;
        }

        // Node with two children: get the inorder successor (smallest
        // in the right subtree)
        item *temp = root->Rchild;
        while (temp->Lchild != NULL)
        {
            temp = temp->Lchild;
        }

        // Copy the inorder successor's content to this node
        strcpy(root->itemName, temp->itemName);
        root->sellerHeapSize = temp->sellerHeapSize;
        for (int i = 0; i < temp->sellerHeapSize; ++i)
        {
            strcpy(root->sellerHeap[i].sellerID, temp->sellerHeap[i].sellerID);
            root->sellerHeap[i].price = temp->sellerHeap[i].price;
        }

        // Delete the inorder successor
        root->Rchild = deleteNode(root->Rchild, temp->itemName);
    }

    return root;
}


void removeMinSeller(item *root)
{
    if (root->sellerHeapSize <= 0)
    {
        return;
    }

    // Store the seller information in the buyTable
    strcpy(buyTable[buyTableSize].itemName, root->itemName);
    strcpy(buyTable[buyTableSize].sellerID, root->sellerHeap[0].sellerID);
    buyTable[buyTableSize].price = root->sellerHeap[0].price;
    buyTableSize++;

    // Remove the root seller by replacing it with the last seller
    root->sellerHeap[0] = root->sellerHeap[root->sellerHeapSize - 1];
    root->sellerHeapSize--;

    // Heapify the min heap
    minHeapify(root->sellerHeap, root->sellerHeapSize, 0);
}

// Function to process a buy request
void processBuyRequest(item *root, char itemName[], FILE *Buy)
{
    item *result = searchBST(root, itemName);

    if (result == NULL)
    {
        // Item not found
        fprintf(Buy, "%s doesn't exist!\n", itemName);
        buyFail++;
        buy++;
    }
    else
    {
        // Process the buy request
        removeMinSeller(result);

        // If there are no more sellers, delete the item from the BST
        if (result->sellerHeapSize == 0)
        {
            root = deleteNode(root, itemName);
        }

        fprintf(Buy, "%s %s %d\n", itemName, buyTable[buyTableSize - 1].sellerID, buyTable[buyTableSize - 1].price);
        buy++;
    }
}


// Function to store the sorted item in the sortTable
void storeSortedItem(char itemName[])
{
    // Check if the item is already in sortTable
    for (int i = 0; i < sortTableSize; ++i)
    {
        if (strcmp(sortTable[i].itemName, itemName) == 0)
        {
            return;  // Item already exists in sortTable, do nothing
        }
    }

    strcpy(sortTable[sortTableSize].itemName, itemName);
    sortTableSize++;
}


void inorderTraversalAndSort(item *root)
{
    if (root != NULL)
    {
        // Traverse the left subtree
        inorderTraversalAndSort(root->Lchild);

        // Store the item in the sortTable
        storeSortedItem(root->itemName);

        // Traverse the right subtree
        inorderTraversalAndSort(root->Rchild);
    }
}

// Function to print the sorted items
void printSortedItems(FILE *Sort)
{
    for (int i = 0; i < sortTableSize; ++i)
    {
        fprintf(Sort, "%s\n", sortTable[i].itemName);
    }
    fprintf(Sort, "----------------------------\n");
}

// Function to get the count of nodes in the BST
int getNodeCount(item *root)
{
    if (root == NULL)
        return 0;
    return 1 + getNodeCount(root->Lchild) + getNodeCount(root->Rchild);
}

// Function to get the height of the BST
int getHeight(item *root)
{
    if (root == NULL)
        return 0;
    int leftHeight = getHeight(root->Lchild);
    int rightHeight = getHeight(root->Rchild);
    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}



// Function to count the number of failed searches in sortTable
int countFailedSearches()
{
    int failedSearches = 0;
    for (int i = 0; i < sortTableSize; ++i)
    {
        if (strcmp(sortTable[i].itemName, "") == 0)
        {
            failedSearches++;
        }
    }
    return failedSearches;
}

void generateReport(FILE *Log)
{
    int totalInserts = buyTableSize + sortTableSize;

    // Count successful and failed searches
    int successfulSearches = buyTableSize - countFailedSearches();
    int failedSearches = sortTableSize;

    // Count successful and failed buys
    int successfulBuys = 0;
    int failedBuys = 0;
    for (int i = 0; i < buyTableSize; ++i)
    {
        if (strcmp(buyTable[i].itemName, "") == 0)
        {
            failedBuys++;
        }
        else
        {
            successfulBuys++;
        }
    }

    fprintf(Log, "insert %d\n", insert);
    fprintf(Log, "search %d %d\n", search, searchFail);
    fprintf(Log, "buy %d %d\n", buy, buyFail);
    fprintf(Log, "node_num %d\n", nodeCount);
    fprintf(Log, "height %d\n", getHeight(root));
}

void ReadOneRecord(FILE *input, FILE *Search, FILE *Buy, FILE *Sort, FILE *Log)
{
    if (strcmp(instruction, "insert") == 0)
    {
        fscanf(input, "%s %s %d", Item.itemName, Seller.sellerID, &Seller.price);
        root = insertBST(root);
        insert++;
        //Only insert into min heap if it's a new item in the BST
        if (root->sellerHeapSize < 1)
        {
            insertMinHeap(root);
        }
        // inorderTraversalAndPrint(root);
    }
    else if(strcmp(instruction, "search") == 0)
    {
        fscanf(input,"%s", Item.itemName);
        searchAndPrint(root, Item.itemName, Search);
    }
    else if (strcmp(instruction, "buy") == 0)
    {
        fscanf(input,"%s", Item.itemName);
        processBuyRequest(root, Item.itemName, Buy);
    }
    else if (strcmp(instruction, "sort") == 0)
    {
        // Initialize sortTable size before sorting
        sortTableSize = 0;

        // Perform inorder traversal and sort the items
        inorderTraversalAndSort(root);

        // Print the sorted items
        printSortedItems(Sort);
    }
    else if (strcmp(instruction, "report") == 0)
    {
        generateReport(Log);
    }
}

int main()
{
    FILE *input = fopen("input.txt", "r+");
    FILE *Search = fopen("searchTable.txt", "w+");
    FILE *Buy = fopen("buyTable.txt", "w+");
    FILE *Sort = fopen("sortTable.txt", "w+");
    FILE *Log = fopen("LogTable.txt", "w+");

    while (fscanf(input, "%s", instruction) != EOF)
    {
        // printf("%s ", instruction);
        ReadOneRecord(input, Search, Buy, Sort, Log);
    }
    
    return 0;
}
