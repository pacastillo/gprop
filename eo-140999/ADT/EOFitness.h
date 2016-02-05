#ifndef _EOFITNESS_H
#define _EOFITNESS_H

#include <ADT/EO.h>
#include <ADT/EOPop.h>
#include <ADT/EOEvalFunc.h>

/** @name Fitness
Fitness-related class include the fitness itself, the evaluation function
and the evaluator. The fitness is provided more as an interface that as anything
else; all normal data types, like float, which is the default fitness type, have them;
if you create your own fitness type, you will have to define these functions. \\
@author GeNeura Team
*/

//@{

/// Abstract data type for fitness objects. 
class EOFitness{
public:	
  ///
  virtual bool operator < ( const EOFitness & _f ) const = 0;
  ///
  virtual bool operator > ( const EOFitness & _f ) const {
    return (  !((*this) < _f) && !(_f == (*this))  );
  };
  ///
  virtual bool operator == ( const EOFitness & _f ) const {
    return (  !((*this) < _f) && !(_f < (*this))  );
  };
  ///
  virtual bool operator != ( const EOFitness & _f ) const { 
    return !((*this) == _f);
  };

};

/** Evaluator takes a vector of EOs and evaluates its fitness
* returning void. Template instances must be of fitness and EO type
*/
template<class EOT>
class EOEvaluator{
public:
	/// Needed virtual destructor
	virtual ~EOEvaluator() {};

	/// Sets the evaluation function
	virtual void Ef( const EOEvalFunc< EOT> & _ef ) = 0;

	/** This is the actual function: it takes a vector of pointers to eo
	 * @param _vEO is a vector of pointers to eo, that will be evaluated
	 */
	virtual void operator()( EOPop< EOT >& _vEO ) = 0;
	
};
//@}

#endif
