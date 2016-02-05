//-----------------------------------------------------------------------------
// EOLayout.h
//-----------------------------------------------------------------------------


#ifndef _EO_LAYOUT_H
#define _EO_LAYOUT_H

#include <algorithm>		// For sort
#include <ADT/EOFitness.h>	// for evalFunc
#include <ADT/EOPop.h>		// for EOPop
#include <generic/EvalAll.h>    // for EvalAll
#include <NPI.h>                // for NSOM


//-----------------------------------------------------------------------------
// class EOLayout
//-----------------------------------------------------------------------------

/**
 * Particular instantiation of the EOEvaluator class.
 * It takes each member in the population, evaluates it, applying the
 * evaluation function it´s been initialized with and then uses the member to
 * train a Self Organizing Map.
 * Uses int labels to count later how many chromosomes lays in each class.
 */
template<class T>
class EOLayout: public EvalAll<T>, public NSOM<T, unsigned long>
{
 public:
  /**
   * Constructs a random initializated layout
   * @param _ef      The evaluation function to use
   * @param _width   Width of the output plane
   * @param _height  Height of the output plane
   * @param _dim     Dimension of codevectors
   * @param _lower   Lower value for random elements
   * @param _upper   Upper value for random elements
   */
  EOLayout(EOEvalFunc<T>& _ef, const unsigned int _width,
	   const unsigned int _height, const unsigned _dim, T _lower, T _upper)
   : NSOM<T unsigned long>, (_width, _height, _dim, _lower, _upper),
     EvalAll<T>(_ef) {};

  /**
   * Loads a trained Layout from a stream
   * @param _ef      The evaluation function to use
   * @param _stream  The stream
   * @exception Exception  If there are problems reaading the vector
   */
  EOLayout(EOEvalFunc<T>& _ef, istream& _stream)
   : NSOM<T unsigned long>, (_stream), EvalAll<T>(_ef) {};

  /** 
   * Applies evaluation function to all members in the population and applies
   * each member to the som to train it.
   * @param _pop  Population
   */
  virtual void operator() (EOPop<T>& _pop)
  {
    EvalAll<T>::operator() (_pop);

    // y ahora ... lo del SOM
   };
};

#endif
