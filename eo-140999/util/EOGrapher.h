#ifndef _EOGRAPHER_H
#define  _EOGRAPHER_H

#include <ADT/EOFitness.h>

/** This is a decorator for the EOEvaluator class, as defined in the Gamma et
 * al book. What it does is to do stuff with the fitness values of all the EOs,
 * in particular, in this case, print mas and min. Can be used to add graphics
 * capabilities to the EO library. Which will be done... eventually */
template<class EOT>
class Grapher: public EOEvaluator<EOT>{
public:
  /// ctor	
  Grapher( EOEvaluator<EOT>& _thisEval )
    :thisEval( _thisEval ) {};

  ///Needed virtual dtor
  virtual ~Grapher() {};

  /// forwards to EOEvaluator setFunc
  virtual void Ef( const EOEvalFunc<EOT> & _ef ) {
    thisEval.Ef( _ef );
  }
#ifdef _MSC_VER
	typedef EOT::EOFitT EOFitT;
#else
	typedef typename EOT::EOFitT EOFitT;
#endif

  /** This is decorator: adds functionality to EOEvaluators; that is why
  * it starts calling the object it has inside, and then performs whatever
  * it is intended to. In the case of EOGrapher, it just computes max, min and
  * average fitness. Not very efficient (runs over the list 3 times) but quite
  * elegant
  */
  virtual void operator()( EOPop< EOT >& _vEO ) {
    thisEval( _vEO );
    EOT* minEO= max_element( _vEO.begin(), _vEO.end(), SortEO<EOT>() );
    EOT* maxEO= min_element( _vEO.begin(), _vEO.end(), SortEO<EOT>() );
    EOPop< EOT >::const_iterator i;
    EOFitT accumulated = 0;
    for ( i = _vEO.begin(); i != _vEO.end(); i ++ ) {
      accumulated += i->fitness();
    }
    float average = accumulated / (float) _vEO.size();	// Average is a floating-point scalar always
    cout << average << "\t" << maxEO->fitness() << "\t" 
	 << minEO->fitness() << endl;
  }

private:
  EOEvaluator<EOT>& thisEval;
  
};
#endif
