#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_HT 100


/*---------------- Taken From: https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/ ------------------ */

// Struct for representing node in min binary tree
struct MinHeapNode { 
    char data;

    uint freq;

    struct MinHeapNode *left, *right;
};

// Struct for representing a min binary tree
struct MinHeap {

    uint size;

    uint capacity;

    struct MinHeapNode **array;
};

// Generates a new min binary tree node
struct MinHeapNode* newNode(char data, uint freq) {
     struct MinHeapNode* temp = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));

     temp->left = temp->right = NULL;
     temp->data = data;
     temp->freq = freq;

     return temp;
}

// Creates a new min binary tree
// where array.size() = capacity
struct MinHeap* createMinHeap(uint capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = 0;
    
    minHeap->array = (struct MinHeapNode **) malloc(capacity * sizeof(struct MinHeapNode *));

    return minHeap;
}

// Swaps values of 2 min binary tree nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* tmp = *a;
    *a = *b;
    *b = tmp;
}

// MinHeapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 *idx + 1;
    int right = 2 * idx + 2;

    if(left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if(right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if(smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

//Checks if size of heap is 1
int isSizeOne(struct  MinHeap* minHeap) {
    return(minHeap->size == 1);
}

// Extrats minimum Values from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    
    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// Inserts new node into minHeap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){
    ++minHeap->size;
    int i = minHeap->size  - 1;

    
}

// Builds minHeap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size-1;
    
    for(int i = (n-1)/2; i >= 0; --i)
        minHeapify(minHeap, i)
}

// Prints array
void printArr(int arr[],  int n) {
    int i;
    for(i = 0; i < n; ++i) {
        printf("%d", arr[i]);
    }

    printf("\n");
}

// Determines whether current node is a leaf
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && (root->right);
}

// Creates a minheap corresponding data arrays
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for(int i = 0; i < size; ++i) 
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);
}