#include "levmarIMUWrapper.h"
#include <math.h>


int main(){
    
  int n = 29;
  double accReal [n*3];
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

  double retval[9];
  fitAccelerometer(noisyMeasuredAcc, n, retval);
}
//0 0 -0.0107586 1 0.9967319 1.007048 0 0.001993764 0.001350793 with real
