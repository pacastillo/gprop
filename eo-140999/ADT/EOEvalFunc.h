//-----------------------------------------------------------------------------
// EOEvalFunc.h
//-----------------------------------------------------------------------------

#ifndef EOEVALFUNC_H
#define EOEVALFUNC_H

/** Evaluate: takes one EO and sets its "fitness" property
 * returning this fitness also. That is why EOT is passed by
 * non-const reference: it must be altered within evaluate.\\

 The requirements on the types with which this class is to be
 instantiated with are null, or else, they depend on the particular
 class it's going to be applied to; EO does not impose any requirement 
 on it. If you subclass this abstract class, and use it to evaluate an 
 EO, the requirements on this EO will depend on the evaluator.
 */
template< class EOT >
struct EOEvalFunc {

#ifdef _MSC_VER
	typedef EOT::EOFitT EOFitT;
#else
	typedef typename EOT::EOFitT EOFitT;
#endif

  /// Effectively applies the evaluation function to an EO or urEO
  virtual EOFitT operator() ( EOT & _eo ) const = 0;
};

#endif
