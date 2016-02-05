#ifndef _EOESOPS_H
#define _EOESOPS_H

#include <util/Random.h>
#include <math.h>		// for exp

/** ES-style mutation: each gene in the chromosome is added its own sigma\\
Obviously, valid only for EOESChrom
*/
template <class fitT>
class EOESMutate: public MonOp< EOESChrom<fitT> > {
public:
	///
	EOESMutate( float _priority = 1 )
		: MonOp< EOESChrom<fitT> >( _priority ){};

	/// needed virtual dtor
	virtual ~EOESMutate() {};

	///
	virtual void operator()( EOESChrom<fitT> & _eo ) const {
		for ( EOESChrom<fitT>::iterator i = _eo.begin(); i != _eo.end(); i ++ ) {
			i->val+=Random::normal(0,i->sigma);
			i->sigma*=exp(Random::normal(0,i->sigma) );
		}
	}

	/// Applies it only to one chromosome
	virtual void applyAt( EOESChrom<fitT>& _eo, unsigned _i, unsigned _j = 0) const {
		_eo[_i].val += _eo[_i].sigma;
	}
};

#endif
