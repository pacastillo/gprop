#ifndef _EOEVALALL_H
#define _EOEVALALL_H

#include <ADT/EOFitness.h>	// for evalFunc
#include <ADT/EOPop.h>		// for EOPop
#include <algorithm>		// For sort

/** Particular instantiation of the EOEvaluator class
It takes each member in the population, and evaluates it, applying
the evaluation function it´s been initialized with 
*/
template<class EOT>
class EOEvalAll: public EOEvaluator<EOT> {
public:

	/// Ctors/dtors
	EOEvalAll( EOEvalFunc<EOT> & _ef ):ef(_ef) {};
	
	///
	virtual ~EOEvalAll() {};

	/** Follows the var-setting convention: capitalized name of
	the variable
	*/
	virtual void Ef( const EOEvalFunc< EOT> & _ef ) { 
		ef = _ef; 
	};

	/** Applies evaluation function to all members in the population, and sets
	    their fitnesses
	Reference is non-const since it orders the population by any order 
	it´s been defined */
#ifdef _MSC_VER
	typedef EOT::EOFitT EOFitT;
#else
	typedef typename EOT::EOFitT EOFitT;
#endif

	virtual void operator() ( EOPop< EOT >& _vEO )  {
	  for ( typename EOPop<EOT>::iterator i = _vEO.begin(); i != _vEO.end(); i ++ ){
	    i->fitness( ef( *i ) );
	  }
	  sort( _vEO.begin(), _vEO.end(), SortEO<EOT>() ); 
	};
	
private:
	EOEvalFunc<EOT>& ef;
};

#endif
