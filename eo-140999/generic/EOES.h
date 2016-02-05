#ifndef _EOES_H
#define _EOES_H

#include <vector>				// for vector
#include <iterator>				// for iterator


#include <generic/EOEvalAll.h>      // for Evaluator
#include <generic/EOGOps.h>
#include <generic/EOEasyGA.h>
#include <generic/EOselect/EOGSRandomSelect.h>     // for EOSelector and Breeder
#include <generic/EOselect/EORandomBreed.h>     // for Breeder
#include <generic/EOGenTerm.h>
#include <generic/EOESChrom.h>
#include <generic/EOESOps.h>        // For EOBitFlip

typedef EOESChrom<float> Chrom;

using namespace std;

/** Simple algorithm for evolutionary computation
  It is a wrapper for EasyGA, but lets the user only the decisions
of some constants
*/
class EOES: public EOAlgo< Chrom > {
public:

	/** The ctor takes all necessary object for an evolutionary algorithm
	* as pointers. These are used so that they can have default null values
	* Initial population can also be passed on. If not, it has to be
	* generated randomly
	*/
  EOES( float _lambda,
	EOEvalFunc<Chrom>&  _ef,
	EOPop<Chrom>& _pop,
	float _mut = 0.2,
	float _xOver = 0.8,
	unsigned _numGen = 100,
	bool _verbose = false )
    :EOAlgo<Chrom>(), eval (_ef ), select( _lambda ),
    breed( ), term( _numGen ), replace( _lambda ), mutate(), uxOver( ),
    es(  eval, select, breed, term, replace, _verbose ){
    
    // Add the operators to the breeder
    breed.addOp( &uxOver );
    breed.addOp( &mutate );
    
    // Evaluate initial population
    eval( _pop );
  };
  
  /// Ctor must ve virtual
  virtual ~EOES() {};

  /** Run the algorithm
  @exception _e might have trouble with non-evaluated eos
  */
  virtual void operator () ( EOPop<Chrom>& _pop )  {
    // and apply the algorithm
	try {
		es( _pop );
	} catch ( exception& _e ) {
	  cout << _e.what() << endl;
	}

  };

 private:
  EOEvalAll<Chrom> eval;
  EORandomSelect<Chrom> select;
  EORandomBreed<Chrom> breed;
  EOGenTerm<Chrom> term;
  EOElimWorst<Chrom> replace;
  EOESMutate<float> mutate;
  EOUniformCrossover<Chrom> uxOver; 

  EOEasyGA<Chrom> es;  
  
};
	

#endif
