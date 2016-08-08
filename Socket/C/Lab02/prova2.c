#include <stdio.h>
#include <string.h>

int main(){
	int i = 1234, j = 2;
	for(; j <= i/2; j++)
		if(i%j == 0) 
			printf("%d\n",j);
	
	return 0;
}
