#ifndef _EOAGEGA_H
#define _EOAGEGA_H

#include <ADT/EOAlgo.h>
#include <ADT/EOFitness.h>                 // for Evaluator
#include <ADT/EOSelector.h>                // for EOSelector and Breeder
#include <generic/EOselect/EOReplacers.h>  // for Replacement operators
#include <ADT/EOTerm.h>			   // for EOTerm
#include <generic/EOselect/EORandomSelect.h>     // EORandomSelect
#include <generic/EOselect/EOAgeReplace.h>         // EOAgeReplace

using namespace std;

/** Age-based genetic algorithm
 */
template<class Chrom>
class EOAgeGA: public EOAlgo<Chrom> {
 public:
  
  /** The ctor takes all necessary object for an evolutionary algorithm
   * as pointers. These are used so that they can have default null values
   * Initial population can also be passed on. If not, it has to be
   * generated randomly
   */
  EOAgeGA( EOEvaluator<Chrom>&  _eval,
	   EOBreeder<Chrom>& _breed,
	   EOTerm<Chrom>& _term):
    EOAlgo<Chrom>(), eval (_eval), select(EORandomSelect<Chrom>(0.4f)), 
    breed( _breed), replace(EOAgeReplace<Chrom>()), term( _term ) {}
  
  /// Ctor must ve virtual
  virtual ~EOAgeGA() {}
  
  /// Run the algorithm
  virtual void operator () ( EOPop<Chrom>& pop ) {
    
    eval(pop);
    
    while (term(pop))
      {
    	EOPop<Chrom> breeders, newPop;

	cout << "pop al principio:" << endl;
	copy(pop.begin(), pop.end(), ostream_iterator<Chrom>(cout, "\n"));
	cout << "breeder al principio:" << endl;
	copy(breeders.begin(), breeders.end(), 
	     ostream_iterator<Chrom>(cout, "\n"));
	cout << "----------------------------------------------------" << endl;

	select(pop, breeders);
	cout << "pop tras select:" << endl;
	copy(pop.begin(), pop.end(), ostream_iterator<Chrom>(cout, "\n"));
	cout << "breeder tras select:" << endl;
	copy(breeders.begin(), breeders.end(), 
	     ostream_iterator<Chrom>(cout, "\n"));
	cout << "----------------------------------------------------" << endl;

	breed(breeders, newPop);
	cout << "breeder tras breeder:" << endl;
	copy(newPop.begin(), newPop.end(), 
	     ostream_iterator<Chrom>(cout, "\n"));
	cout << "----------------------------------------------------" << endl;
 
	eval(newPop);
	replace(newPop, pop);
	cout << "pop tras replace:" << endl;
	copy(pop.begin(), pop.end(), ostream_iterator<Chrom>(cout, "\n"));
	cout << "----------------------------------------------------" << endl;
      }
  }
  
 private:
  EOEvaluator<Chrom>&   eval;
  EORandomSelect<Chrom> select;
  EOBreeder<Chrom>&     breed;
  EOAgeReplace<Chrom>   replace;
  EOTerm<Chrom>&        term;
};

//-----------------------------------------------------------------------------

#endif
