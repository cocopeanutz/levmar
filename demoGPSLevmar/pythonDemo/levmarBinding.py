from ctypes import *
import os


G_CONS = 9.80665;

LM_OPTS_SZ = 5
LM_INFO_SZ = 10
#define LM_ERROR         -1
#define LM_INIT_MU    	 1E-03
#define LM_STOP_THRESH	 1E-17
#define LM_DIFF_DELTA    1E-06
#define LM_VERSION       "2.6 (November 2011)"
x--> output
p--> variable that gets fitted
data-->incoming data
n--> banyaknya output
m--> banyak variable yg difit

fun = @(x,xdata) sum(([1 -x(1) x(2); 0 1 -x(3); 0 0 1] * diag([x(4) x(5) x(6)]) * (xdata + [x(7); x(8); x(9)])).^2);

void expfunc(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; ++i){
    x[i]=p[0]*exp(-p[1]*i) + p[2];
  }
}

/* Jacobian of expfunc() */
void jacexpfunc(double *p, double *jac, int m, int n, void *data)
{
register int i, j;

  /* fill Jacobian row by row */
  for(i=j=0; i<n; ++i){
    jac[j++]=exp(-p[1]*i);
    jac[j++]=-p[0]*i*exp(-p[1]*i);
    jac[j++]=1.0;
  }
}




os.chdir("/home/edo/SIPA/projects/levmar")
dll = cdll.LoadLibrary("/home/edo/SIPA/projects/levmar/levmar.so")
