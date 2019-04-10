#include "revert_string.h"
#include "swap.h"
#include <string.h>
#include <stdio.h>

void RevertString(char *str)
{
	int length=strlen(str);
	
	for(int i=0; i<length/2;++i)
	{
	   Swap(str+i, str+length-i-1);
   }

	   
}

