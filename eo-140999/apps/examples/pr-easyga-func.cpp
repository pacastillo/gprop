// Program to test several EO features

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif 

#include <string>
#include <iostream>
#include <iterator>

using namespace std;

#include <generic/EOVector.h>
#include <generic/EOGOps.h>
#include <generic/EOselect/EORandomBreed.h>
#include <generic/EOselect/EOTournament.h>
#include <generic/EOselect/EOReplacers.h>
#include <specific/EOSIntOps.h>
#include <generic/EOGenTerm.h>
#include <generic/EOEasyGAwFunc.h>
#include <numeric>


int evaluate( EOVector<int,int> & _eo ) {
  int suma = accumulate( _eo.begin(), _eo.end(), 0 );
  return suma;
};

main() {
	const unsigned size = 10, popSize = 100, range = 100;
	srand( (unsigned)time( NULL ) );
	unsigned i, j;

	// useful typedefs
	typedef EOVector<int,int> eoInt;
	typedef eoInt* ptEoInt;
	EOPop< eoInt > initPop;
	for ( j = 0; j < popSize; j ++ ) {
	  ptEoInt EOIA = new eoInt;	
	  for ( i = 0; i < size; i ++ ) {
	    EOIA->push_back( rand() % range - range/2 );
	  }
	  initPop.push_back( *EOIA );	  
	  copy( EOIA->begin(), EOIA->end(), ostream_iterator<int>( cout, " ") );
	  cout << endl;
	}

	// Evaluate
	int (*thisEval)( EOVector<int,int> & ) = &evaluate;

	// Go ahead to next generation
	unsigned tournamentSize = 3;
	EOTournament< eoInt > coach( tournamentSize );
	
	// And now breed
	EORandomBreed<eoInt> chaperon;

	//Add operators
	EOXOver2<eoInt> xOver( 1 ); 
	chaperon.addOp( &xOver );

	//More operators
	EODup<eoInt> dupper ( 1 );
	chaperon.addOp( &dupper );

	//Add mutation
	EOIntArrayMutate<eoInt> mutator( 1 );
	chaperon.addOp( &mutator );

	//Termination condition
	EOGenTerm< eoInt> ngenerations( 10 );

	// Replacement
	EOElimAll< eoInt > popKiller;

	//And now the population itself
	EOEasyGAwFunc<eoInt> 
		thisAlgorithm( thisEval, coach, chaperon, ngenerations, popKiller, true );

	// Evaluate the initial population
	// evaluate the new ones. Population is ranked
	for ( EOPop< EOVector<int,int>  >::iterator l = initPop.begin(); l != initPop.end(); l++ ) {
	  int fitness = evaluate( *l );
	  l->fitness( fitness );
	}
	sort( initPop.begin(), initPop.end(), SortEO< EOVector<int,int>  >() ); 
								    
	thisAlgorithm( initPop );

	return 0;
}
	
