#ifndef _MINDIST_H
#define _MINDIST_H

#include <ADT/EOEvalFunc.h>
#include <generic/EOESChrom.h>

/** Implementation of an function evaluation object. Computes distance to a n-dimensional
vector
*/
struct MinDist: public EOEvalFunc< EOESChrom<float> > {

  MinDist( vector<float> _sample ): sample( _sample ) {};

  virtual ~MinDist(){};

  vector<float> sample;

  virtual float operator() ( EOESChrom<float> & _eo ) const {
    float totDist=0;
    unsigned i;
    for ( i=0; i<_eo.length(); i++ ) {
      float dist = _eo[i].val-sample[i];
      totDist+=dist*dist;
    }

    totDist=(totDist==0)?10000:1/totDist; 
    return totDist;
  };
};

#endif
