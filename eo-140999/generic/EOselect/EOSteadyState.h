#ifndef _EOSTEADYSTATE_H
#define _EOSTEADYSTATE_H

#include <math.h>                       // for floor
#include <ADT/EOSelector.h>
#include <ADT/EOOp.h>

/** Selects those who are going to reproduce, eliminates those which
 * are not fit. \\
 This class can be instantiated with any class that has a default void ctor; 
 this is the requisite to instantiate a STL vector with it (at least in VC++)
*/
template<class EOT>
class EOSteadyState:public EOSelector<EOT>{
public:

  /// Proportion of guys that are going to be eliminated
  EOSteadyState( float _rate): repRate(_rate){};

  /// Virtual dtor
  ~EOSteadyState(){};

  /** Returns the genetic pool of the reproducers
   * In this function, the only requisite is that EOT and fitnessT
   * can be used to instantiate evedEO */
  virtual void operator() ( EOPop< EOT > &  _vEO, EOPop<EOT>& _aVEO) const {

    unsigned thisSize = _vEO.size();
    unsigned subPop = unsigned(floor(thisSize*repRate));

    // Eliminate bad ones
    //    _vEO.erase( _vEO.end() - subPop, _vEO.end() );
    
    // Build vector
    typename EOPop<EOT>::iterator j;
    for ( j = _vEO.begin(); 
	  ( j != _vEO.begin() + subPop ) && (j != _vEO.end());
	  j++ ) {
      _aVEO.push_back( *j );
    }
  }

  /** 
   * This is a _new_ function, non defined in the parent class
   * It´s used to set the selection rate 
   */
  void rate( float _rate ) {
    repRate = _rate;
  }

private:
  float repRate;

};

#endif
