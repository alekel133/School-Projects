// cc quick-sort.c
#include <stdio.h>

void quick_sort (int *a, int n, char *side) 
{
  if(n < 2) return;
  
  // Print the array before sorting
  static int frame_count = 0;
  int local_frame_count = frame_count;
  printf("-----------------------------------\n");
  printf("%d %s: ", frame_count++, side);
  for(int i = 0; i < n; i++) {
    printf("%d ", a[i]); 
  }
  printf("\n");

  // Check out LHS and RHS for static int frame_count
   printf("      frame_count l-value (LHS): %ld,      r-value (RHS): %d\n", 
          (long)&frame_count, frame_count); 

  // Check out LHS and RHS for int local_frame_count
   printf("local_frame_count l-value (LHS): %ld, r-value (RHS): %d\n", 
          (long)&local_frame_count, local_frame_count); 

  // Do the rest of quick_sort
  {
    int p = a[n / 2];
    int *l = a;
    int *r = a + n - 1;
    while (l <= r) {
      if (*l < p) {
        l++;
      }
      else if (*r > p) {
        r--;
      }
      else {
        int t = *l;
        *l = *r;
        *r = t;
        l++;
        r--;
      }
    }
    quick_sort(a, r - a + 1, "left");
    quick_sort(l, a + n - l, "right");
  }

  // Now print the sorted array
  printf("%d %s: ", local_frame_count, side);
  for(int i = 0; i < n; i++) {
      printf("%d ", a[i]);
  } 
  printf("\n");
}
 
int main () {
  int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
  int n = sizeof a / sizeof a[0];
  printf("size of a: %d\n", n);
  quick_sort(a, n, "whole");
  return 0;
}
