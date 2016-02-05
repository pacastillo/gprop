#ifndef _EOFITTERM_H
#define _EOFITTERM_H

#include <ADT/EOTerm.h>

/** Fitness termination: terminates after a the difference between the
fitness of the best individual and a maximum fitness to achieve is less
than certain number called epsilon., i.e., |maximum-fitness|<epsilon
*/
template< class EOT>
class EOFitTerm: public EOTerm<EOT> {
public:

	/// Ctors/dtors
	EOFitTerm( const float _maximum, const float _epsilon )
		: EOTerm<EOT> (), maximum( _maximum ), epsilon(_epsilon){};

	/// Copy ctor
	EOFitTerm( const EOFitTerm&  _t )
		: EOTerm<EOT> ( _t ), maximum( _t.maximum ), 
	  epsilon(_t.epsilon){};

	///
	virtual ~EOFitTerm() {};

	/** Returns false when a certain number of generations is
	* reached */
	virtual bool operator() ( const EOPop<EOT>& _vEO ) {
	  float bestFitness=_vEO[0].fitness();
	  float dif=bestFitness-maximum;
	  dif=(dif<0)?-dif:dif;
	  return (dif>epsilon ) || (bestFitness > maximum);
	}

private:
	float maximum, epsilon;
};

#endif
