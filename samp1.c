#include <stdio.h>
#include <stdlib.h>

void foo(void)
{
	int N = 1000;
  	int x[N+3];
  	int y[N+3];
	int i;
	time_t t;
	
  
   srand((unsigned) time(&t));

  for ( i = 0; i < N; i++ ) {
	x[i] = rand() ;
     y[i] = (x[i] + x[i-1] + x[i+1])/3;   
  }
  for ( i = 0; i < N;i++ ) {
       
        printf( "Value of i = %d , x[i] = %d , y[i] =%d \n ",i,x[i], y[i]  );
    }
     printf( "\n" );
}

int main(int ac, const char *av[])
{
  foo();
	return 0;
}
