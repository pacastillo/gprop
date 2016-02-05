#ifndef _SUMALL_H
#define _SUMALL_H

#include <numeric> // for accumulate

#include <ADT/EOEvalFunc.h>
#include <generic/EOVector.h>

/** Implementation of a function evaluation object. Adds all
values of the chromosome 
*/
struct SumAll: public EOEvalFunc< EOVector<int,int> > {
  virtual int operator() ( EOVector<int,int> & _eo ) const {
    int suma = accumulate( _eo.begin(), _eo.end(), 0 );
    return suma;
  };
};

#endif
