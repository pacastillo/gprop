#ifndef _EOISLANDGA_H
#define _EOISLANDGA_H

#include <vector>				// for vector
#include <iterator>				// for iterator
#include <iostream>				// for cout

#include <EOADT.h>
#include <EOGeneric.h>			

using namespace std;

/** Island genetic algorithm, which makes several populations evolve
    in paralel. It does not subclass EOAlgo, since it does not work on 
    a population, but on several populations, and it would be clumsy
    to have it split and join populations. However, uses the rest of
    the algorithms.
 */
template<class EOT>
class EOIslandGA: public EOAlgo<EOT> {
 public:
  
  /** The ctor takes all necessary object for an evolutionary algorithm
   * as references. These are used so that they can have default null values
   * Initial population could also be passed on. If not, it has to be
   * generated randomly
   */
  EOIslandGA( EOEasyGen<EOT>&  _generation,
	      EOTerm<EOT>& _term,
	      bool _verbose = false,
	      unsigned _numPops = 2,
	      unsigned _genToMigration = 10,
	      float _percMigration = 0.1f)
    :EOAlgo<EOT>(), 
    generation( _generation ), 
    term( _term ), verbose( _verbose ), numPops( _numPops ),
    genToMigration( _genToMigration), percMigration( _percMigration ) {} ;

  /// Dtor must be virtual
  virtual ~EOIslandGA() {};
  
  /// Run the state-free algorithm
  virtual void operator() ( EOPop<EOT>& _pop ) {

    vector< EOPop<EOT> > popVector(numPops);    // Set of populations
    unsigned i, j;

    // Assign initial populations to the rest of the populations
    EOPop<EOT> tmpPop = _pop;
    for (  i = 0; i < _pop.size()/numPops; i ++ ) {
      for (  j = 0; j < numPops; j ++ ) {
	Uniform<unsigned> u (0,tmpPop.size() );
	unsigned rnd = u();
	popVector[j].push_back( tmpPop[rnd] );
	tmpPop.erase( tmpPop.begin()+rnd);
      }
    }
    if ( verbose ) {
      for (  j = 0; j < numPops; j ++ ) {
	cout << "Pop [" << j << "]\n";
	copy( popVector[j].begin(), popVector[j].end(),
	      ostream_iterator< EOT >( cout, "\n" ) );
      }
    }

    EOGenTerm<EOT> syncGen( genToMigration);
    do {
      // Run algorithms until sync
      for ( i = 0; i < popVector.size(); i ++ ) {
	do {
	  generation( popVector[i] );
	} while ( syncGen( popVector[i] ) );
      }
      
      // Perform migration
      EOElimWorst<EOT> migrate( percMigration );
      for ( unsigned i = 0; i < popVector.size() -1; i ++ ) {
	migrate( popVector[i], popVector[i+1]);
      }
      migrate( popVector[popVector.size()-1],popVector[0] );
    } while( term( popVector[0] ) ); // dummy arg
    
    // Join the population
    _pop.erase( _pop.begin(), _pop.end() );
    for ( i = 0; i < popVector.size(); i ++ ) {
      _pop.insert(_pop.begin(), popVector[i].begin(), popVector[i].end() );
    }

  };

  
 private:
  EOEasyGen<EOT> generation;
  EOTerm<EOT>& term;
  bool verbose;
  unsigned numPops;
  unsigned genToMigration;
  float percMigration;
};


#endif
