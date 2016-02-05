//-----------------------------------------------------------------------------
// EOEvalFuncPtr.h
//-----------------------------------------------------------------------------

#ifndef EOEVALFUNCPTR_H
#define EOEVALFUNCPTR_H

#include <ADT/EOEvalFunc.h>

/** EOEvalFuncPtr: takes one EO and sets its "fitness" property
 * returning this fitness also. That is why EOT is passed by
 * non-const reference: it must be altered within evaluate. This class
 * takes an existing function pointer and converts it into a evaluation
 * function class.
 */
template< class EOT >
struct EOEvalFuncPtr: public EOEvalFunc<EOT> {

  EOEvalFuncPtr( EOFitT (* _eval)( EOT& ) )
    : EOEvalFunc(), evalFunc( _eval ) {};
    
    /// Effectively applies the evaluation function to an EO 
    virtual EOFitT operator() ( EOT & _eo ) const {
      return (*evalFunc)( _eo );
    };
      
  private:
   EOFitT (* evalFunc )( EOT& );
};

#endif
