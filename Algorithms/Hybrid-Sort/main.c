#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int genRand()
{
   return rand() % 1000; 
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// The code for Selection Sort was taken from GeeksForGeeks.com
void sSort(int arr[], int n)
{
    for(int i = 0; i < n -1; i++)
    {
        int min_id = i;
        for(int j = i+1; j < n; j++)
        {
            if(arr[j] < arr[min_id])
                min_id = j;
        } 

        if(min_id != i)
        {
            swap(&arr[min_id], &arr[i]);
        }
    }
}

// The code for parition was taken from GeeksForGeeks.com
int partition(int *arr, int low, int high)
{
	int pivot = arr[high];
	int i = low-1;

	for(int j = low; j <= high - 1; j++)
	{
		if(arr[j] < pivot)
		{
			i++;
            swap(&arr[i], &arr[j]);
		}
	}
    swap(&arr[i+1], &arr[high]);
	return i+1;
}

//The code for Quicksor was taken from geeksforgeeks.com
void qSort(int* arr, int low, int high)
{
    if(low < high)
    {
        int pi = partition(arr, low, high);

        qSort(arr, low, pi-1);
        qSort(arr, pi+1, high);
    }
}

// The code for the Quicksort/Selection Sort hybrid was taken from geeksforgeeks.com's hybrid sort and modified to fit the needs of the program.
void sort(int* arr, int low, int high)
{
    if(high - low < 10)
    {
        sSort(arr, high);
        return;
    }    
        int pi = partition(arr, low, high);
        sort(arr, low, pi-1);
        sort(arr, pi+1, high);
}

void populateArr(int arr[], int size)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = genRand();
    }
}

void printArr(int arr[], int size)
{
    for(int i = 0; i < size; ++i)
    {
        if(i == 0)
        {
            printf("{");
        }
        printf("%d", arr[i]);
        if(i < size-1)
        {
            printf(",");
        }
        if(i == size-1)
        {
            printf("}\n");
        }
    }
}

int main(int argc, char* argv[])
{ 
    int size, iter;

    long double timeUsed;

    struct timeval start, end;
    
    srand(time(NULL));

    if(argc != 1 && argc != 3)
    {
        perror("format: program <int arr_size> <int iter>");
        exit(EXIT_FAILURE);
    }

    if(argc == 3)
    {
        sscanf(argv[1], "%d", &size);
        sscanf(argv[2], "%d", &iter);
    }

    int *arr0 = (int*) malloc(size*sizeof(int));
    int *arr1 = (int*) malloc(size*sizeof(int));
    int *arr2 = (int*) malloc(size*sizeof(int));


    for(int i = 0; i < iter; ++i)
    {
        printf("\n\n");

        populateArr(arr0, size);
        for(int j = 0; j < size; ++j)
        {
            arr1[j] = arr0[j];
            arr2[j] = arr0[j];
        }

        // Selection Sort
        printf("Selection Sort: \n"); 
        printf("Unsorted: \n");
        printArr(arr0, size);
        printf("Sorted: \n");
        gettimeofday(&start, NULL);
        sSort(arr0, size);
        gettimeofday(&end, NULL);
        printArr(arr0, size);
        timeUsed = (end.tv_sec - start.tv_sec) * 1e6;;
        timeUsed = (timeUsed + (end.tv_usec-start.tv_usec)) * 1e-6;
        printf("Time(in microseconds): %Lf\n", timeUsed*1000000);

        // Quicksort
        printf("Quicksort: \n"); 
        printf("Unsorted: \n");
        printArr(arr1, size);
        printf("Sorted: \n");
        gettimeofday(&start, NULL);
        qSort(arr1, 0, size);
        gettimeofday(&end, NULL);
        printArr(arr1, size);
        timeUsed = (end.tv_sec - start.tv_sec) * 1e6;;
        timeUsed = (timeUsed + (end.tv_usec-start.tv_usec)) * 1e-6;
        printf("Time(in microsecond)s: %Lf\n", timeUsed*1000000);
        
        // Hybrid Sort
        printf("Hybrid Sort: \n"); 
        printf("Unsorted: \n");
        printArr(arr2, size);
        printf("Sorted: \n");
        gettimeofday(&start, NULL);
        sort(arr2, 0, size);
        gettimeofday(&end, NULL);
        printArr(arr2, size);
        timeUsed = (end.tv_sec - start.tv_sec) * 1e6;;
        timeUsed = (timeUsed + (end.tv_usec-start.tv_usec)) * 1e-6;
        printf("Time(in microsecond)s: %Lf\n", timeUsed*1000000);

        printf("\n\n");
    }

    exit(EXIT_SUCCESS);
}
