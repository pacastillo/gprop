#ifndef _EOEASYGA_H
#define _EOEASYGA_H

#include <vector>				// for vector
#include <iterator>				// for iterator
#include <iostream>				// for cout

#include <ADT/EOAlgo.h>
#include <ADT/EOFitness.h>      // for Evaluator
#include <ADT/EOSelector.h>     // for EOSelector and Breeder
#include <generic/EOselect/EOReplacers.h>      // for Replacement operators
#include <ADT/EOTerm.h>			// for EOTerm

using namespace std;

/** Simple algorithm for evolutionary computation
 * It is not exactly a Simple GA, as defined by Goldberg,
 * it is just a simple implementation of a Genetic Algorithm 
 */
template<class EOT>
class EOEasyGAwFunc: public EOAlgo<EOT> {

#ifdef _MSC_VER
  typedef EOT::EOFitT EOFitT;
#else
  typedef typename EOT::EOFitT EOFitT;
#endif

 public:
  
  /** The ctor takes all necessary object for an evolutionary algorithm
   * as pointers. These are used so that they can have default null values
   * Initial population can also be passed on. If not, it has to be
   * generated randomly
   */
  EOEasyGAwFunc( EOFitT (* _eval)( EOT& ),
		 EOSelector< EOT>& _select,
		 EOBreeder<EOT>& _breed,
		 EOTerm<EOT>& _term,
		 EOReplace<EOT>& _rep,
		 bool _verbose = false )
    :EOAlgo<EOT>(), eval( _eval ), select( _select),
    breed( _breed), replace( _rep), term( _term ),  verbose( _verbose ){};
  
  /// Ctor must ve virtual
  virtual ~EOEasyGAwFunc() {};
  
  /// Run the algorithm
  virtual void operator () ( EOPop<EOT>& _pop ) {
    
    EOPop<EOT> &oldGeneration = _pop;
    
    //    eval( oldGeneration );
    // Population must be evaluated before putting it in
    do {
      
      // Select and eliminate
      EOPop<EOT> pool;
      select( oldGeneration, pool );
      
      EOPop<EOT>::iterator l;		// assorted iterator
      unsigned j;		// Assorted index
      if ( verbose ) {
	// Print pool
	cout << "Pool ------------------------------\n";
	for ( l = pool.begin(), j = 0; l != pool.end(); l++, j ++ ) {
	  cout << "EO# " << j << " = " << *l 
	       << " - FITNESS - " << l->fitness( ) << endl;
	}
      }
      
      //Apply to new Generation
      EOPop<EOT> newPop;
      breed( pool, newPop );
      if ( verbose ) {	// And print if needed
	cout << "New Gen ---------------------------\n";	
	for ( l = newPop.begin(), j = 0; 
	      l != newPop.end(); l++, j ++ ) {
	  cout << "EO# " << j << " = " << *l << endl;
	}
      }
      
      // evaluate the new ones. Population is ranked
      for ( l = newPop.begin(); l != newPop.end(); l++ ) {
	 EOFitT fitness = eval( *l );
	 l->fitness( fitness );
      }
      sort( pool.begin(), pool.end(), SortEO<EOT>() ); 

      // Replace after evaluation. 
      replace( pool, oldGeneration );

      try {
	sort( oldGeneration.begin(), oldGeneration.end(), SortEO<EOT>() ); 
      } catch ( UException& _e ) {
	cout << _e.what() << endl;
      }
      
      if ( verbose ) {
	cout << "Evaluated pop ----------------------\n";
	for ( j = 0; j < oldGeneration.size(); j ++ ) {
	  cout << "Fitness " << j << " " 
	       << oldGeneration[j].fitness() << endl;
	}
      }

      /*
      cout << "#------bichos actuales ----------------------\n";
      for ( j = 0; j < oldGeneration.size(); j ++ ) {
	cout << oldGeneration[j].fitness() 
	     << "   size=" << oldGeneration[j].length() 
	     << endl;
      }
      */

    } while ( term( oldGeneration ) );
    
  };
  
 private:
  EOFitT (*eval)( EOT& eo);
  EOSelector<EOT>& select;
  EOBreeder<EOT>& breed;
  EOReplace<EOT>& replace;
  EOTerm<EOT>& term;
  
  /// Indicates if it is going to be "verbose": print all things, and so on
  bool verbose;
};


#endif
