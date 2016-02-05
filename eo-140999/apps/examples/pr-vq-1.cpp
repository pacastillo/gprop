
/** @name Optimizing a vector quantizer
\begin{center} Testing EO for numeric optimization \end{center}

pr-vq-1 stands for pr (prueba) - vq for Vector Quantization, and 1 for 1. This is a testbed for
simple algorithm, and the features that compose it: selectors, breeders, terminators, and so on

*/
//@{

// Disable long names warning
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <string>
#include <iostream>

// STL includes
#include <iterator>

using namespace std;

#include <generic/EOVector.h>
#include <generic/EOGOps.h>
#include <generic/EOEvalAll.h>
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOselect/EORank.h>
#include <generic/EOselect/EOReplacers.h>
#include <specific/EOSFloatOps.h> // for float operators
#include <generic/EOFitTerm.h>
#include <generic/EOEasyGA.h>
#include <EOutil.h>

// Fitness function
#include "MinDist.h"

main(int argc, char **argv) {
  
  unsigned size,popSize,range;
  float select,maximumFitness,epsilon;

  Parser parser( "pr-vq-1","Program to test vector quantisation using the EO library.\n\tAlso tests the EOParser." );

  parser.addFlag( "-h", "--help","Shows this help" );
  parser.addParam( "-s","--size","5",Param::INT,"Number of genes per chromosome");
  parser.addParam( "-p","--popSize","50",Param::INT,"Number of individuals per generation");
  parser.addParam( "-r","--range","20",Param::INT,"Each gene will be between -range/2 and range/2" );
  parser.addParam( "-t","--select","0.3",Param::FLOAT,"Rate for stedy-state" );
  parser.addParam( "-m","--maximumFitness","0.6",Param::FLOAT,"Maximum fitness one wants to reach");
  parser.addParam( "-e","--epsilon",".1",Param::FLOAT,"Maximum difference to the maximum fitness allowed" );

  try {
    parser.parse( argc,argv );
    if ( parser.isEnabled( "-h" )) {
      parser.printHelp();
      exit( 1 );
    }
    size=parser.getInt( "-s" );
    popSize=parser.getInt( "-p" );
    range=parser.getInt( "-r" );
    select=parser.getFloat( "-t" );
    maximumFitness=parser.getFloat( "-m" );
    epsilon=parser.getFloat( "-e" );
  }
  catch (UException& e) {
    cout << e.what() << endl;
    parser.printHelp();
    exit(1);
  }
  catch (exception& e) {
    cout << e.what() << endl;
    parser.printHelp();
    exit(1);
  }

  unsigned i, j;
  
  // useful typedefs
  typedef EOVector<float,float> eoFloat;
  typedef eoFloat* ptEoFloat;

  vector<float >  sample;
  Uniform<float> rndGen( range*(-0.5), range*0.5 );
  for ( i = 0; i < size; i ++ ) {
    sample.push_back( rndGen() );
  }
  
  EOPop<eoFloat> initPop;
  cout << "Poblacion: "<<endl;
  for ( j = 0; j < popSize; j ++ ) {
    eoFloat EOIA;	
    for ( i = 0; i < size; i ++ ) {
      float value=rndGen();
      EOIA.push_back( value );
    }
    
    initPop.push_back( EOIA );	  
    copy( EOIA.begin(), EOIA.end(), ostream_iterator<float>( cout, " ") );
    cout << endl;
  }

  // Evaluate
  MinDist thisEvalFunc( sample );
  EOEvalAll<eoFloat> thisEval( thisEvalFunc );
  
  // Go ahead to next generation
  EOSteadyState<eoFloat> coach( select );

  // Replacement
  EOElimWorst< eoFloat > popKiller( select );

  // And now breed
  EORank<eoFloat> chaperon( select );
  
  //Add operators
  EOTranspose<eoFloat> transpose;
  chaperon.addOp( &transpose );
  
  //More operators
  EOFloatArrayMutateByPercentage <eoFloat> MutByPge( 1 );
  chaperon.addOp( &MutByPge );

  EOXOver2<eoFloat> xOver2 ( 1 );
  chaperon.addOp( &xOver2 ); 

  chaperon.addOp( new EOFloatIntermXOver<eoFloat> ); 
  
  //Termination condition
  EOFitTerm< eoFloat> min( maximumFitness, epsilon );
	
  //And now the algorithm itself
  EOEasyGA<eoFloat> 
    thisAlgorithm( thisEval, coach, chaperon, min, popKiller, true);
	
  // Evaluate the initial population
  thisEval( initPop );

  // Apply the genetic algorithm, and generate evolvedPop
  thisAlgorithm( initPop );

  // Displaying results
  cout << "Best individual: " << initPop[0] << endl;
  cout << endl << "Sample to reach: ";
  copy( sample.begin(), sample.end(), ostream_iterator<float>( cout, " " ) );
  return 0;
}

//@}
