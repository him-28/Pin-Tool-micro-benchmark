#include <stdio.h>
#include <stdlib.h>
//#define N 1000

void do_y(int u[], int N)
  {
	int i;
	 for (i = 0; i < (N+2); i++ )
	 u[i] = u[i]*u[i+1];
	 
	printf("\n Process on y =%d \n", u[i]);

  }

void foo(void)
{
	int N = 1000;
  	int x[N+3];
  	int y[N+3];
	int i,j;
	time_t t;
	
  
   srand((unsigned) time(&t));

  for ( i = 0; i < N+2; i++ ) {
	 for(j = i*5; j < (5*i)+N; j++)
	{
   x[j] = rand() ;
     y[j] = (x[j] + x[j-1] + x[j+1])/3;
    printf( "Value of j = %d , x[j] = %d , y[j] =%d \n ",j,x[j], y[j]  );  
	 do_y(y,N);  
    } 
	
  }
  
     printf( "\n" );
}


int main(int ac, const char *av[])
{
  foo();
	return 0;
}
