#ifndef _EOTERM_H
#define _EOTERM_H

#include <ADT/EOPop.h>

/** Termination condition for the genetic algorithm
 * Takes the population as input, returns true for continue,
 * false for termination
 */
template< class EOT>
class EOTerm {
public:

	/// Ctors/dtors
	virtual ~EOTerm() {};

	/** Returns false if the training has to stop, true if it
	 continues \\
	 It is non-const since it might change the internal state
	 of the object, for instance, updating a counter
	*/
	virtual bool operator() ( const EOPop< EOT >& _vEO ) = 0 ;
	
};

#endif
