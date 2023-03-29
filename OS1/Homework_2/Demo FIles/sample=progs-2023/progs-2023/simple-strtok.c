/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="ls -la > filesample &";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," \t\n");
  while (pch != NULL)
  {
	printf ("==%s==\n",pch);
	pch = strtok (NULL, " \t\n");
  }
  return 0;
}
