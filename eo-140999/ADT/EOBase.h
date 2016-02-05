//-----------------------------------------------------------------------------
// EOBase.h
//-----------------------------------------------------------------------------

#ifndef _EOBASE_H
#define _EOBASE_H

#include <string>					// for string

#include <util/UException.h>		// for UException

//-----------------------------------------------------------------------------

/** 
  Base class for chromosomes, it's a very basic class, which does not require 
  anything but a fitness and an ID.
 @author Geneura Team
 */
template<class fitnessT>
class EOBase {
public:

  ///
  typedef fitnessT EOFitT;

  /// Copy ctor. The fitness class must have a ctor that takes 0 as argument
  EOBase( const string& _ID = "" )
    : repID( _ID ), repFitness(0), validFitness( false ) {};

  /// Copy Ctor
  EOBase( const EOBase& _eob )
    : repID( _eob.repID ), repFitness( _eob.repFitness ),validFitness( _eob.validFitness ) {};


  /// Assignment operator
  const EOBase& operator= ( const EOBase& _eob ) {
	  if ( this != &_eob ) {
		repFitness = _eob.repFitness;
		validFitness = _eob.validFitness;
		repID = _eob.repID;
	  }
	  return *this;
  }

  ///
  virtual ~EOBase(){};

  /** This exception should be thrown when a chromosome has not been 
  evaluated, and thus, the fitness is not valid
  */
  class InvalidFitness : public UException {
  public:
    /**
       * Constructor
       */
    InvalidFitness()
	: UException("The fitness for this EO has not been evaluated") { };

    ~InvalidFitness() throw() {};
  };

  /** Fitness things: reads fitness
      @exception InvalidFitness if fitness has not been evaluated
  */
  virtual fitnessT fitness() const { 
    if ( !validFitness )
      throw InvalidFitness();
    return repFitness;
  };
  
  /** Fitness things: writes fitness and sets the #validFitness# flag
      to true. Passed by value, so the fitness must have a copy ctor
      @param _fitness new value of the fitness
  */
  virtual void fitness( const fitnessT &_fitness ) { 
    validFitness = true;
    repFitness = _fitness;
  };
  
  /** read the ID, which is supposed to be an unique identification
   * for each chromosome. It should be set only at birth */
  const string& ID() const { return repID;};
  
  /// Write ID
  void ID( const string& _id ) { repID = _id;};
  
private:
  string repID;			// A theoretically unique ID
  fitnessT repFitness;	// Fitness of this chromosome
  bool validFitness;		// true if fitness is valid

};

/** sort class for EOBase. Needs to have a fitness method, and the fitness method
must return a class with the < operation
*/
template <class EOT> 
struct SortEO {
  SortEO() {};
  ///
  bool operator () ( const EOT& _e1,
					 const EOT& _e2 ) {
    return ( _e2.fitness() < _e1.fitness() );
  };
};

#endif
