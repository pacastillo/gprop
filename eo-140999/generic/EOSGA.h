#ifndef _EOSGA_H
#define _EOSGA_H

#include <vector>				// for vector
#include <iterator>				// for iterator


#include <generic/EOEvalAll.h>      // for Evaluator
#include <generic/EOGOps.h>
#include <generic/EOEasyGA.h>
#include <generic/EOselect/EOLottery.h>     // for EOSelector and Breeder
#include <generic/EOselect/EOTournament.h>     // for EOSelector 
#include <generic/EOselect/EORandomBreed.h>     // for Breeder
#include <generic/EOGenTerm.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>        // For EOBitFlip

using namespace std;

/** Simple algorithm for evolutionary computation
  It is a wrapper for EasyGA, but leaves the user only the decisions
  of some parameters
*/
class EOSGA: public EOAlgo< EOBin<float> > {
public:
  typedef EOBin<float> Chrom; // DON'T MOVE THIS OUTSIDE OF THE CLASS

	/** The ctor takes all necessary object for an evolutionary algorithm
	* as pointers. These are used so that they can have default null values
	* Initial population can also be passed on. If not, it has to be
	* generated randomly
	*/
  EOSGA( EOEvalFunc<Chrom>&  _ef,
	 EOPop<Chrom>& _pop, unsigned _popSize,
	 unsigned _numGenes, unsigned _numBits,
	 float _mut = 0.2,
	 float _xOver = 0.8,
	 unsigned _numGenerations = 100,
	 bool _verbose = false )
    :EOAlgo<Chrom>(), eval (_ef ), select(),
     breed( ), term( _numGenerations ),  replace( ), bitFlip(_mut) ,xOver( _xOver ),
     ga(  eval, select, breed, term, replace, _verbose ){

       // Add the operators to the breeder
       breed.addOp( &xOver );
       breed.addOp( &bitFlip );

       EOBinFactory<float> factory( "SGA", _numGenes, _numBits );
       Chrom* pchrom = factory.make(); // Creates a chrom of specified length
       EOMutation<float> mutation(0.5); // High initial mutation
       for ( unsigned i = 0; i < _popSize; i++ ) {
	 Chrom* pchrom2 = factory.make( *pchrom, &mutation); // Creates a chrom of specified length
	 _pop.push_back( *pchrom2 );   // Adds it to the population
       }
	 
       // Evaluate initial population
       eval( _pop );
  };
  
  /// Ctor must ve virtual
  virtual ~EOSGA() {};

  /** Run the algorithm
  @exception _e might have trouble with non-evaluated eos
  */
  virtual void operator () ( EOPop<Chrom>& _pop )  {
    // and apply the algorithm
	try {
		ga( _pop );
	} catch ( exception& _e ) {
	  cout << _e.what() << endl;
	}

  };

 private:
  EOEvalAll<Chrom>  eval;
  EOLottery<Chrom> select;
  EORandomBreed<Chrom> breed;
  EOGenTerm<Chrom> term;
  EOElimAll<Chrom> replace;
  EOBitFlip<float> bitFlip;
  EOXOver2<Chrom> xOver; 

  EOEasyGA<Chrom> ga;  
  
};
	

#endif
