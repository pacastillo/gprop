#include <fstream.h>
#include <stdio.h>
#include <values.h>
#include <math.h>
#include <stdlib.h>

int main(void) {

  cout << "\n ****** VALORES m M *********"
       << "\n mI=" << MININT
       << "\n MI=" << MAXINT
       << "\n RM=" << RAND_MAX
       << "\n mF=" << MINFLOAT
       << "\n MF=" << MAXFLOAT
       << "\n mD=" << MINDOUBLE
       << "\n MD=" << MAXDOUBLE
       << "\n ****************" << endl;

  float floatdiv=10.0/3.0;
  double doublediv=10.0/3.0;
  float floatsqrt=sqrt(2);
  double doublesqrt=sqrt(2);

  cout << "\n ****** PRECISIONES (reales) *********"
       << "\n (F)(10/3)=";
  printf("%.60f",floatdiv);
  cout << "\n (D)(10/3)=";
  printf("%.60f",doublediv);

  cout << "\n (F)(sqrt(2)=";
  printf("%.60f",floatsqrt);
  cout << "\n (D)(sqrt(2)=";
  printf("%.60f",doublesqrt);
  cout << "\n(F) como (D)=";
  printf("%.60f",(float)doublesqrt);
  cout << "\n(D) como (F)=";
  printf("%.60f",(double)floatsqrt);
  cout << "\n ****************" << endl;

  return 0;
}
