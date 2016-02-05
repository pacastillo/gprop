#ifndef EOADT_H
#define EOADT_H
/** @name Abstract Data Types
    Abstract data types are the foundation of the EO library. They
    provide the interface that objects have to follow to be able to
    interact with the other objects in the EO library. EO ADTs are
    usually defined by the EO coordination; theoretically, you
    shouldn´t need to define new ones. But if you do, tell us.
*/
//@{
    //@Include: ADT/*.h
//@}

// All includes in the ADT directory

// These are basically EO-level classes
#include <ADT/EOBase.h>
#include <ADT/EOBidim.h>
#include <ADT/EO.h>
#include <ADT/EOOp.h>

// Related to fitness
#include <ADT/EOEvalFunc.h>
#include <ADT/EOFitness.h>

// Population-level classes ald algorithms
#include <ADT/EOPop.h>
#include <ADT/EOSelector.h>
#include <ADT/EOBreeder.h>
#include <ADT/EOTerm.h>
#include <ADT/EOReplace.h>

//Algorithm classe
#include <ADT/EOAlgo.h>	
#include <ADT/EOPopLessAlgo.h>  

// Misc
#include <ADT/EOFactory.h>

//@{
	//@Include: ADT/EO*.h
//@}
#endif
