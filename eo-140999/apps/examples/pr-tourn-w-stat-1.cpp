// Program to test several EO features

#pragma warning(disable:4786)

#include <string>
#include <iostream>
#include <iterator>

using namespace std;

#include <generic/EOVector.h>
#include <generic/EOGOps.h>
#include <generic/EOEvalAll.h>
#include <generic/EOselect/EOGSTourn.h>
#include <specific/EOSIntOps.h>
#include <generic/EOGenTerm.h>
#include <generic/EOEasyGA.h>
#include <math/EOStat.h>

#include "SumAll.h"

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
	eval thisEvalFunc;
	EvalAll<int, eoInt> thisEval( thisEvalFunc );
	EOStat<int, eoInt> graphEval( thisEval );

	// Go ahead to next generation
	unsigned tournamentSize = 3;
	EOGTournament< eoInt > coach( 3 );
	
	// And now breed
	RandomBreed<eoInt> chaperon;

	//Add operators
	EOXOver2<int,eoInt> xOver( 1 ); 
	chaperon.addOp( &xOver );

	//More operators
	EODup<eoInt> dupper ( 1 );
	chaperon.addOp( &dupper );

	//Add mutation
	EOIntArrayMutate<eoInt> mutator( 1 );
	chaperon.addOp( &mutator );

	//Termination condition
	EOGenTerm< eoInt> ngenerations( 10 );
	
	//And now the population itself
	EOEasyGA<int,eoInt> 
		thisAlgorithm( graphEval, coach, chaperon, ngenerations );
	thisAlgorithm( initPop );

	return 0;
}
	
