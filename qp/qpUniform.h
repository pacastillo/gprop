//-----------------------------------------------------------------------------
// Uniform.h
//-----------------------------------------------------------------------------

#ifndef _QPUNIFORM_H
#define _QPUNIFORM_H

//-----------------------------------------------------------------------------

#include <time.h>
#include <stdlib.h> 

//-----------------------------------------------------------------------------
// uniform return values uniformly distributed over the interval [min, max)
//-----------------------------------------------------------------------------

/// Class for uniform random number generation
template<class T> class qpUniform
{
 public:
  ///
  qpUniform(T _min = 0, T _max = 1): min(_min), diff(_max - _min) {}
  
  ///
  T operator()()
    {
      return min + T(diff * rand() / RAND_MAX);
    }
  
 private:
  T min;
  float diff;
};

//-----------------------------------------------------------------------------

#endif
