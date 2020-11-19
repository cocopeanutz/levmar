#pragma once
#define G_CONS (9.80665)

//add non bayesian error model
double* fromRealAccToMeasured(double* accReal, int n, double* p);
//add bayesian error model
double* giveNoiseToMeasurement(double * measuredAcc, int n);
//entry point to get the non bayesian error model
double* fitAccelerometer(double * noisyMeasuredAcc, int n, double * retval);
