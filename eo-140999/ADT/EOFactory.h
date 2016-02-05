//-----------------------------------------------------------------------------
// EOFactory.h
//-----------------------------------------------------------------------------

#ifndef _EOFACTORY_H
#define _EOFACTORY_H

//-----------------------------------------------------------------------------

#include <ADT/EOOp.h>

/** EO Factory. A factory is used to create other objects. In particular,
it can be used so that objects of that kind can´t be created in any other
way*/
template<class EOT>
class EOFactory {
	
public:
	
	/// constructor
	EOFactory( string _prefix = "" ) {}
	
	/// destructor
	virtual ~EOFactory() {}
	
	/// One of the factory methods: creates a random EO, usually
	virtual EOT* make() = 0;
	
	/** Another factory methods: creates an EO from another
	with an unary operator
	*/
	virtual EOT* make(	const EOT& _eo, const MonOp<EOT>* _eop = 0) = 0;
	
	
};


#endif
