#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>




void fixed_pim_start(bool fixed_pim_flag )
{
	 fixed_pim_flag = false;
	printf("fixed start ");
}

void fixed_pim_done(bool  fixed_pim_flag)
{
	 fixed_pim_flag = true;
    printf("fixed done ");
}

void gen_pim_start(bool gen_pim_flag)
{
	 gen_pim_flag = false;
    printf("gen start  ");
}

void gen_pim_done(bool gen_pim_flag)
{
	 gen_pim_flag = true;
    printf("gen done ");
}


void foo(void)
{
	int N = 1000;
  	int x[N+3];
  	int y[N+3];
	int i;
	time_t t;
	bool gen_pim_flag ; //= false;
	bool fixed_pim_flag; // = false;
  
   srand((unsigned) time(&t));

  for ( i = 0; i < N; i++ ) {
	x[i] = rand() ;
	fixed_pim_start(fixed_pim_flag);
	gen_pim_start(gen_pim_flag);
     y[i] = (x[i] + x[i-1] + x[i+1])/3;  
	printf( "Value of every Y =%d \n ", y[i]  ); 
	fixed_pim_done(fixed_pim_flag);
	gen_pim_done(gen_pim_flag);
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
