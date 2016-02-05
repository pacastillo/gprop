#ifndef _EOEASYGA_H
#define _EOEASYGA_H

#include <vector>				// for vector
#include <iterator>				// for iterator
#include <iostream>				// for cout

#include <ADT/EOAlgo.h>
#include <ADT/EOFitness.h>      // for Evaluator
#include <generic/EOEasyGen.h>     // for EOSelector and Breeder
#include <ADT/EOTerm.h>			// for EOTerm

using namespace std;

/** Simple algorithm for evolutionary computation
 * It is not exactly a Simple GA, as defined by Goldberg,
 * it is just a simple implementation of a Genetic Algorithm 
 */
template<class EOT>
class EOEasyGA: public EOAlgo<EOT> {
 public:
  
  /** The ctor takes all necessary object for an evolutionary algorithm
   * as references. These are used so that they can have default null values
   * Initial population can also be passed on. If not, it has to be
   * generated randomly
   */
  EOEasyGA( EOEvaluator<EOT>&  _eval,
	    EOSelector< EOT>& _select,
	    EOBreeder<EOT>& _breed,
	    EOTerm<EOT>& _term,
	    EOReplace<EOT>& _rep,
	    bool _verbose = false )
    :EOAlgo<EOT>(), generation( _eval, _select, _breed, _rep, _verbose ), 
    term( _term ) {} ;

  /** Ctor that takes a ready-made generation object
   */
  EOEasyGA( EOEasyGen<EOT>& _gen,
			EOTerm<EOT>& _term,
	    bool _verbose = false )
    :EOAlgo<EOT>(), generation( _gen ), 
    term( _term ){};

  
  /// Dtor must be virtual
  virtual ~EOEasyGA() {};
  
  /// Run the state-free algorithm
  virtual void operator () ( EOPop<EOT>& _pop ) {
    
    //    eval( oldGeneration );
    // Population must be evaluated before putting it in
    unsigned tmpGen=0;
    do {
      tmpGen++;
      generation.setGenerationNumber(tmpGen);
      generation( _pop );
    } while ( term( _pop ) );
    
  };
  
 private:
  EOEasyGen<EOT> generation;
  EOTerm<EOT>& term;
  
};


#endif
