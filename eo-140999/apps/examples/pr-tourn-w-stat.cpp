// Program to test several EO features

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

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
#include <generic/EOselect/EOTournament.h>
#include <generic/EOselect/EORandomBreed.h>
#include <specific/EOSIntOps.h>
#include <generic/EOGenTerm.h>
#include <generic/EOEasyGA.h>

#include <util/EOStat.h>		// for EOStat


//Utils
#include <util/Parser.h>

// Fitness
#include "SumAll.h"

main( int argc, char** argv) {
  Parser parser("pr-tourn-w-graph-1", "Demo of an easy Genetic algorithm with flexible algorithms\\
that uses Tournament selection and a population decoration -- graph ");

  parser.addParam("-s", "--size", "10", Param::INT, "Chromosome size");
  parser.addParam("-p", "--popsize", "100",  Param::INT, "Number of EOs in the population");
  parser.addParam("-r", "--range", "100", Param::FLOAT, "EO values range from -range/2 to range/2");
  parser.addParam("-g", "--generations", "10",
		  Param::INT, "Number of generations the GA will be run" ); 
  parser.addParam("-t", "--tournamentsize", "3", Param::INT, "Tournament size");

  int range;

  unsigned 
    popSize, 
    size,
    numGenerations,
    tournamentSize;

  try {
    parser.parse(argc, argv);
    
    size = parser.getInt( "-s" );
    popSize = parser.getInt( "-p" );
    range = parser.getInt("-r");
    numGenerations = parser.getInt("-g");
    tournamentSize= parser.getInt("-t" );
  }
  catch (UException & e)
    {
      cout << e.what() << endl;
    }
	
  srand( (unsigned)time( NULL ) );
  unsigned j;

  // useful typedefs
  typedef EOVector<int,int> eoInt;
  typedef eoInt* ptEoInt;
  EOPop< eoInt > initPop;
  EOVectorFactory<int,int> factory( "sa2-", size, -range/2, range/2 );
  for ( j = 0; j < popSize; j ++ ) {
    ptEoInt EOIA = factory.make();
    initPop.push_back( *EOIA );	  
    cout << *EOIA << endl;
    delete EOIA;		// Already copied
  }

  // Evaluate
  SumAll thisEvalFunc;
  EOEvalAll<eoInt> thisEval( thisEvalFunc );
  EOStat<eoInt> stat( thisEval, true );

  // Go ahead to next generation
  EOTournament< eoInt > coach( tournamentSize );
	
  // And now breed
  EORandomBreedLog<eoInt> chaperon( factory );

  // Replacement
  EOElimAll< eoInt > popKiller;

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
  EOGenTerm< eoInt> ngenerations( numGenerations );
	
  //And now the population itself
  EOEasyGA<eoInt> 
    thisAlgorithm( stat, coach, chaperon, ngenerations, popKiller );

  // Evaluate the initial population
  thisEval( initPop );

  // and apply the algorithm
  thisAlgorithm( initPop );

  return 0;
}
	
