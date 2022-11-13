"""
Author: Alexander Kellough NetID: atk133
Class:CSE-4833
Description:
    Demo of performance of asymptotically asimilar sorting algorithms, and a 
    more efficient combo of the two.
"""
import random as rand
import time
import sys

recurse = False

def sSort(nums):
    for x in range(len(nums)-1):
        m = x
        for y in range(x+1, len(nums)):
            if(nums[y] < nums[m]):
                m = y

        if m != x:
            (nums[x], nums[m]) = (nums[m], nums[x])

def partition(nums, low, high):
    pivot = nums[high]
    x = low - 1

    for y in range(low, high):
        if nums[y] <= pivot:
            x = x + 1
            (nums[x], nums[y]) = (nums[y], nums[x])

    (nums[x+1], nums[high]) = (nums[high], nums[x+1])
    return x+1

def tsSort(nums, low, high):
    for x in range(low, high):
        m = x
        for y in range(x+1, high+1):
            if(nums[y] < nums[m]):
                m = y

        if m != x:
            (nums[x], nums[m]) = (nums[m], nums[x])

def qSort(nums, low, high):
    if low < high:
        pi = partition(nums, low, high)

        qSort(nums, low, pi -1)
        qSort(nums, pi+1, high)

def hSort(nums, low, high):
    if(high-low < 20):
        tsSort(nums, low, high)

    else:
        pi = partition(nums, low, high)
    
        hSort(nums, low, pi -1)
        hSort(nums, pi+1, high)

def main():
    if(len(sys.argv) < 3):
        print("Syntax: program <int arr_size> <int iterations>")
        quit()
    
     
    size = int(sys.argv[1])
    it = int(sys.argv[2])
    nums = []
    sSortt = []
    qSortt = []
    hSortt = []

    for x in range(it):

        nums = []

        for y in range(size):
            n = rand.randint(0, 1000)
            nums.append(n)

        nums1 = nums.copy()
        nums2 = nums.copy()

        print("List:")
        print(nums)
        print()
        print()

        start = time.time()
        sSort(nums)
        end = time.time()
        print("Selection Sort: ")
        print(nums)
        print()
        sSortt.append(end-start)
        print("Sort Time: " + str(end - start))
        print()

        start = time.time()
        qSort(nums1, 0 , len(nums1)-1)
        end = time.time()
        print("Quicksort: ")
        print(nums1)
        print()
        qSortt.append(end-start)
        print("Sort Time: " + str(end - start))
        print()

        start = time.time()
        hSort(nums2, 0, len(nums)-1)
        end = time.time()
        print("Hybrid Sort: ")
        print(nums2)
        print()
        hSortt.append(end-start)
        print("Sort Time: " + str(end - start))
        print()

        print()

    sSorta = sum(sSortt)/len(sSortt)
    qSorta = sum(qSortt)/len(qSortt)
    hSorta = sum(hSortt)/len(hSortt)

    print("Selection Sort Average Time: {:.9f}".format(sSorta*1000000))
    
    print("Quicksort Average Time: {:.9f}".format(qSorta*1000000))
    
    print("Hybridsort Average Time: {:.9f}".format(hSorta*1000000))

main()
