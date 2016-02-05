#ifndef _EOTOURNAMENT_H
#define _EOTOURNAMENT_H

#include <math.h>                       // for ceil
#include <ADT/EOSelector.h>

/** Selects those who are going to reproduce, eliminates those which
 * are not fit using Tournament selection: a subset of the population
 * of size tournamentSize is chosen, and the best is selected for the 
 * population 
@author JJ Merelo, 1998
*/
template<class EOT>
class EOTournament:public EOSelector<EOT>{
public:

  /// Proportion of guys that are going to be eliminated
  EOTournament( unsigned _tSize): repTournamentSize(_tSize){};

  /// Virtual dtor
  ~EOTournament(){};

  /// Set tourn size
  void tournamentSize( unsigned _size ) { repTournamentSize = _size; };

  /** 
   * Returns the genetic pool of the reproducers
   * In this function, the only requisite is that EOT and fitnessT
   * can be used to instantiate evedEO 
   */
  virtual void operator() ( EOPop<EOT>& _vEO, EOPop<EOT>& _aVEO) const {
    
    unsigned thisSize = _vEO.size();
    
    // Build vector
    for ( unsigned j = 0; j < thisSize; j ++ ) {
      // Randomly select a tournamentSize set, and choose the best
      EOPop<EOT> veoTournament;
      for ( unsigned k = 0; k < repTournamentSize; k++ ) {
	unsigned chosen = rand() % thisSize;
	EOT newEO =  _vEO[chosen];
	veoTournament.push_back( newEO );
      }
      sort( veoTournament.begin(), veoTournament.end(), SortEO<EOT>() );
      // The first is chosen for the new population
      _aVEO.push_back( veoTournament.front() ); 
    }
  };
  
 private:
  unsigned repTournamentSize;
  
};

#endif
