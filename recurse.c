#include "stdio.h"
#include "stdlib.h"

int recurse(int a){
	if(a==0)
		return 5;
	return 4*(a+1) + 2*recurse(a-1)-2;
}


int main(int argc, char* argv[]){

	int m = atoi(argv[1]);
	printf("%d", recurse(m));
	return 0;
	

}
