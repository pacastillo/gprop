
// Disable long names warning
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

// STL includes
#include <string>
#include <iostream>

#include <iterator>

using namespace std;

// Standard C libraries for rand
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <generic/EOVector.h>
#include <generic/EOGOps.h>
#include <generic/EOEvalAll.h>
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOselect/EORank.h>
#include <generic/EOselect/EOReplacers.h>
#include <specific/EOSIntOps.h> // for intArrayMutator
#include <generic/EOGenTerm.h>
#include <generic/EOEasyGA.h>

// Fitness Function
#include "SumAll.h"

//Utils
#include <util/Parser.h>

main( int argc, char** argv) {

	Parser parser("EasyGA", "Demo of an easy Genetic algorithm with flexible algorithms");

	parser.addParam("-s", "--size", "10", Param::INT, "Chromosome size");
	parser.addParam("-p", "--popsize", "100",  Param::INT, "Number of EOs in the population");
	parser.addParam("-r", "--range", "100", Param::FLOAT, "EO values range from -range/2 to range/2");
	parser.addParam("-g", "--generations", "10",
			Param::INT, "Number of generations the GA will be run" ); 

	unsigned size, 
	  popSize, 
	  numGenerations;
	int range;

	try {
	  parser.parse(argc, argv);

	  size = parser.getInt( "-s" );
	  popSize = parser.getInt( "-p" );
	  range = parser.getInt("-r");
	  numGenerations = parser.getInt("-g");
	}
	catch (UException & e) {  // Catching exceptions from particular to general
	    cout << e.what() << endl;
	}
	catch (exception & e) {
	    cout << e.what() << endl;
	}

	// start the algorithm
	srand( (unsigned)time( NULL ) );
	unsigned j;

	// useful typedefs
	typedef EOVector<int,int> eoInt;
	typedef eoInt* ptEoInt;

	EOPop<eoInt> initPop;
	EOVectorFactory<int,int> factory( "sa2", size, -range/2, range/2 );
	for ( j = 0; j < popSize; j ++ ) {
	  ptEoInt EOIA = factory.make();
	  initPop.push_back( *EOIA );	  
	  cout << *EOIA << endl;
	  delete EOIA;		// Already copied
	}

	// Evaluate
	SumAll thisEvalFunc;
	EOEvalAll<eoInt> thisEval( thisEvalFunc );

	// Go ahead to next generation
	float select = (float)0.3;
	EOSteadyState<eoInt> coach( select );

	// Eliminate the worst
	unsigned subPop = (unsigned) floor( select*popSize );
	EOCrowding<eoInt> popKiller( subPop );
	
	// And now breed
	EORank<eoInt> chaperon( subPop );

	//Add operators
	EOXOver2<eoInt> xOver( 1 ); 
	chaperon.addOp( &xOver );

	//More operators
	EODup<eoInt> dupper( 1 );
	chaperon.addOp( &dupper );

	//Mxore operators
	EOKill<eoInt> killer( 1 );
	chaperon.addOp( &killer );

	//Add mutation
	EOIntArrayMutate<eoInt> mutator( 1 );
	chaperon.addOp( &mutator);

	//Termination condition
	EOGenTerm< eoInt> ngenerations( numGenerations );
	
	//And now the population itself
	EOEasyGA<eoInt> 
		thisAlgorithm( thisEval, coach, chaperon, ngenerations, popKiller, true);

	// Evaluate the initial population
	thisEval( initPop );

	// Apply the genetic algorithm, and generate evolvedPop
	thisAlgorithm( initPop );

	return 0;
}
	
