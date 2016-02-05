#ifndef _EOGENTERM_H
#define _EOGENTERM_H

#include <ADT/EOTerm.h>

/** Generational termination: terminates after a number of generations
*/
template< class EOT>
class EOGenTerm: public EOTerm<EOT> {
public:

	/// Ctors/dtors
	EOGenTerm( unsigned _totalGens)
		: EOTerm<EOT> (), repTotalGenerations( _totalGens ), 
	  thisGeneration(0){};

	/// Copy Ctor
	EOGenTerm( const EOGenTerm& _t)
		: EOTerm<EOT> ( _t ), repTotalGenerations( _t.repTotalGenerations ), 
	  thisGeneration(0){};

	/// Assignment Operator
	const EOGenTerm& operator = ( const EOGenTerm& _t) {
	  if ( &_t != this ) {
	    repTotalGenerations =  _t.repTotalGenerations; 
	    thisGeneration = 0;
	  }
	  return *this;
	}

	/// Dtor
	virtual ~EOGenTerm() {};

	/** Returns false when a certain number of generations is
	* reached */
	virtual bool operator() ( const EOPop<EOT>& _vEO ) {
	  thisGeneration++;
	  //	  cout << " [" << thisGeneration << "] ";
	  return (thisGeneration < repTotalGenerations) ; // for the postincrement
	}

	/** Sets the number of generations to reach 
	    and sets the current generation to 0 (the begin)*/
	virtual void totalGenerations( unsigned _tg ) { 
	  repTotalGenerations = _tg; 
	  //	  thisGeneration = 0;
	};

	/** Returns the number of generations to reach*/
	virtual unsigned totalGenerations( ) {  return repTotalGenerations; };
	
private:
	unsigned repTotalGenerations, thisGeneration;
};

#endif
