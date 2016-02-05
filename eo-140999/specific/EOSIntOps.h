#ifndef _EOSINTOPS_H
#define _EOSINTOPS_H

// Standard C libraries for rand
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// STL libraries
#include <vector>		// For vector<int>

#include <ADT/EO.h>
#include <generic/EOGOps.h>

/// Mutation for integers: adds one
template< class EOTint >
class EOIntArrayMutate: public MonOp<EOTint> {
public:
  ///
  EOIntArrayMutate( float _priority = 0):
    MonOp<EOTint>( _priority ){};

	/// needed virtual dtor
	virtual ~EOIntArrayMutate() {};

	///
	virtual void operator()( EOTint & _eo ) const {
	  unsigned toMutate = rand() % _eo.length();
	  applyAt( _eo, toMutate );
	}

	/// applies operator to one gene in the EO
	virtual void applyAt( EOTint & _eo, 
			      unsigned _i, unsigned _j = 0) const {
		int tmpC = _eo.readGene( _i );
		_eo.writeGene( _i, ++tmpC );
	}
};

#endif
