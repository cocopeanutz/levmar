//  Copyright (C) 2008  Manolis Lourakis (lourakis at ics forth gr)
//  Institute of Computer Science, Foundation for Research & Technology - Hellas
//  Heraklion, Crete, Greece.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "levmar.h"
#ifndef LM_DBL_PREC
#error Example program assumes that levmar has been compiled with double precision, see LM_DBL_PREC!
#endif


/* the following macros concern the initialization of a random number generator for adding noise */
#undef REPEATABLE_RANDOM
#define DBL_RAND_MAX (double)(RAND_MAX)

#ifdef _MSC_VER // MSVC
#include <process.h>
#define GETPID  _getpid
#elif defined(__GNUC__) // GCC
#include <sys/types.h>
#include <unistd.h>
#define GETPID  getpid
#else
#warning Do not know the name of the function returning the process id for your OS/compiler combination
#define GETPID  0
#endif /* _MSC_VER */

#ifdef REPEATABLE_RANDOM
#define INIT_RANDOM(seed) srandom(seed)
#else
#define INIT_RANDOM(seed) srandom((int)GETPID()) // seed unused
#endif

const double G_CONS = 9.80665;

double* fromRealAccToMeasured(double* xo, int n, double* p){
  double *xn = (double*)malloc(n*3*sizeof(double));
  double *xm = xn;
  for(int i=0; i<n; i++){
    xm[0]=(xo[0]+xo[1]*p[0]+xo[2]*(-p[1]+p[0]*p[2]))/p[3]-p[6];
    xm[1]=(xo[1]+xo[2]*p[2])/p[4]-p[7];
    xm[2]=(xo[2]/p[5])-p[8];
    xm+=3;
    xo+=3;
  }
  return xn;
};

double gNoise(double m, double s);
double* giveNoiseToMeasurement(double* xo, int n){
  double *xn = (double*)malloc(n*3*sizeof(double));
  double *xm = xn;
  double noise = 0.1/16;
  for(int i=0; i<n; i++){
    xm[0]=gNoise(xo[0], noise);
    xm[1]=gNoise(xo[1], noise);;
    xm[2]=gNoise(xo[2], noise);;
    xm+=3;
    xo+=3;
  }
  return xn;
};
/* Gaussian noise with mean m and variance s, uses the Box-Muller transformation */
double gNoise(double m, double s)
{
  //say variance is 0.1 tapi diukur 4x jadinya 0.1/16
  double r1, r2, val;

  r1=((double)random())/DBL_RAND_MAX;
  r2=((double)random())/DBL_RAND_MAX;

  val=sqrt(-2.0*log(r1))*cos(2.0*M_PI*r2);

  val=s*val+m;

  return val;
}

// %αyz,αzy,αzx,sax,say,saz,bax,bay,baz
// %y=g^2
//fun = @(x,xdata) sum(([1 -x(1) x(2); 0 1 -x(3); 0 0 1] * diag([x(4) x(5) x(6)]) * (xdata + [x(7); x(8); x(9)])).^2);

#define XX ((x[0]+p[6])*p[3])
#define XY ((x[1]+p[7])*p[4])
#define XZ ((x[2]+p[8])*p[5])
#define EST_X (XX-p[0]*XY+p[1]*XZ)
#define EST_Y (XY-XZ*p[2])
#define EST_Z (XZ)
void accFunc(double *p, double *y, int m, int n, void *data)
{
  double *x = (double*)data;

  register int i;

  for(i=0; i<n; ++i){
    y[i]=pow(EST_X, 2) + pow(EST_Y, 2)+pow(EST_Z, 2);
    x+=3;

  }
}

/* Jacobian of expfunc() */
void jacAccFunc(double *p, double *jac, int m, int n, void *data)
{
  register int i;
  double *x = (double*)data;
  /* fill Jacobian row by row */
  // for(i=j=0; i<n; ++i){
  for(i=0; i<n*m;){
    jac[i++]=2*EST_X*(-XY);
    jac[i++]=2*EST_X*(XZ);
    jac[i++]=2*EST_Y*(-XZ);

    jac[i++]=2*EST_X*(x[0]+p[6]);
    jac[i++]=2*EST_X*(-p[0])*(x[1]+p[7])+2*EST_Y*(x[1]+p[7]);
    jac[i++]=2*EST_X*(p[1])*(x[2]+p[8])+2*EST_Y*(-p[2])*(x[2]+p[8])+2*XZ*(x[2]+p[8]);

    jac[i++]=2*EST_X*(p[3]);
    jac[i++]=2*EST_X*(-p[0])*(p[4])+2*EST_Y*(p[4]);
    jac[i++]=2*EST_X*(p[1])*(p[5])+2*EST_Y*(-p[2])*(p[5])+2*EST_Z*(p[5]);
    x+=3;
  }
}
#undef XX
#undef XY
#undef XZ
#undef EST_X
#undef EST_Y
#undef EST_Z

// x--> output
// p--> variable that gets fitted
// data-->incoming data
// n--> banyaknya output
// m--> banyak variable yg difit

//n --> number of measurements
/**
    y-> pseudorange
    4 parameters -> x-coor, y-coor, z-coor,
*/
double* fitGnss(double* data, int n, double* y)
{

  const int m=4; // 4 parameters
  double p[m], y[n], opts[LM_OPTS_SZ], info[LM_INFO_SZ];
  register int i;
  int ret;

  // INIT_RANDOM(0);
  // for(i=0; i<n; ++i)
  //   x[i]=(5.0*exp(-0.1*i) + 1.0) + gNoise(0.0, 0.1);
  for(int i = 0; i<n; i++){
    y[i]=pow(G_CONS,2);
  }
  /* initial parameters estimate: (1.0, 0.0, 0.0) */
  p[0]=0.0; p[1]=0.0; p[2]=0.0;
  p[3]=1.0; p[4]=1.0; p[5]=1.0;
  p[6]=0.0; p[7]=0.0; p[8]=0.0;
//opts pass default

  /* optimization control parameters; passing to levmar NULL instead of opts reverts to defaults */
  opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
  opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference Jacobian version is used

  // opts[0]=1E-9; opts[1]=1E-3; opts[2]=1E-3; opts[3]=1E-4;
  // opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference Jacobian version is used


  /* invoke the optimization function */
  ret=dlevmar_der(accFunc, jacAccFunc, p, y, m, n, 1000, NULL, info, NULL, NULL, data); // with analytic Jacobian

  //ret=dlevmar_dif(expfunc, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // without Jacobian
  printf("Levenberg-Marquardt returned in %g iter, reason %g, sumsq %g [%g]\n", info[5], info[6], info[1], info[0]);
  printf("Best fit parameters: %.7g %.7g %.7g %.7g %.7g %.7g %.7g %.7g %.7g\n", p[0], p[1], p[2],
  p[3], p[4], p[5],
  p[6], p[7], p[8]);
  exit(0);
}

int main(){
  int n = 29;
  double* accReal = (double*) malloc(sizeof(double)*n*3);
  for(int i=0; i<n; i++){
    *(accReal+i*3) = G_CONS*sin((double)i/n*3.14*2);
    *(accReal+i*3+1) = G_CONS*cos((double)i/n*3.14*2)*sin((double)i/n*3.14+1);
    *(accReal+i*3+2) = G_CONS*cos((double)i/n*3.14*2)*cos((double)i/n*3.14+1);
  }
  double p[9]={
    0.01, 0.07, 0.03,
    1.02, 1.01, 1.05,
    0.05, 0.17, 0.2
  };
  double* measuredAcc = fromRealAccToMeasured(accReal, n, p);
  double* noisyMeasuredAcc = giveNoiseToMeasurement(measuredAcc, n);
  fitAccelerometer(noisyMeasuredAcc, n);
}
//0 0 -0.0107586 1 0.9967319 1.007048 0 0.001993764 0.001350793 with real
