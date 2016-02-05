#ifndef _EOGENGA_H
#define _EOGENGA_H

#include <vector>				// for vector
#include <iterator>				// for iterator
#include <iostream>				// for cout

#include <ADT/EOAlgo.h>
#include <ADT/EOFitness.h>      // for Evaluator
#include <generic/EOEasyGen.h>     // for EOSelector and Breeder
#include <ADT/EOTerm.h>			// for EOTerm

using namespace std;

/** Same as EasyGA, except that the generation object must be built
    in advance.
 */
template<class EOT>
class EOGenGA: public EOAlgo<EOT> {
 public:
  
  /** Ctor that takes a ready-made generation object
   */
  EOGenGA( EOEasyGen<EOT>& _gen,
	    EOTerm<EOT>& _term,
	    bool _verbose = false )
    :EOAlgo<EOT>(), generation( _gen ), 
    term( _term ){};

  
  /// Dtor must be virtual
  virtual ~EOGenGA() {};
  
  /// Run the state-free algorithm
  virtual void operator () ( EOPop<EOT>& _pop ) {
    
    //    eval( oldGeneration );
    // Population must be evaluated before putting it in
    do {
      generation( _pop );
    } while ( term( _pop ) );
    
  };
  
 private:
  EOEasyGen<EOT>& generation;
  EOTerm<EOT>& term;
  
};


#endif
