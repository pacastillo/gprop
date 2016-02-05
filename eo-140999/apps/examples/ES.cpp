/** @name Evolution Strategy

This program implements a simple evolution strategy as defined by Rechenberg and Schwefel
: Floating-point Chromosome  with increments, random selection, random reproduction, with lambda+mu
replacement and generational termination.\\

The operators are the standard ones in ES: uniform xOver and mutation, which includes mutation of
the chromosome value and the sigma

  The only thing the user has to define is, obviously, the fitness function, which
  in EO becomes a fitness function {\em class}, the initial population, which in
  this case is created randomly, and the parameters for mutation and crossover.\\

  If you want a more powerful way of defining the GA, and more control over it,
  use other algorithm (#EOAlgo# class, like EasyGA).\\

  @see EOAlgo
  @see EOESChrom
  @see EOTerm
  */
//@{

#include <generic/EOESChrom.h>
#include <generic/EOEvalAll.h>
#include <generic/EOES.h>
#include <util/Parser.h>

// Fitness Function
#include "MinDistES.h"

//-----------------------------------------------------------------------------

typedef EOESChrom<float> Chrom;
typedef EOPop<Chrom> Pop;


/** Function that creates the initial population
@param _pop population object, passed by value
@param _numChrom number of chromosomes that will be created
@param _numBits fixed number of bits in each chromosome
*/
void createInitialPop( Pop& _pop, unsigned _lambda, unsigned _dim, float _range ) {
  for ( unsigned i = 0; i < _lambda; i++ ) {
    Chrom aChrom;
    Uniform<float> rnd( -_range/2, _range/2 ), rndSigma( 0, _range/100 );
    for ( unsigned j = 0; j < _dim; j ++ ) {
      EOESGene g1( rnd(), rndSigma() );
      aChrom.push_back( g1 );
    }
    _pop.push_back( aChrom );
  }	
}

/// Parses the program´s command line, by creating a parsing object
void getParams( int _argc, char** _argv, 
		bool& _verbose, unsigned& _l, unsigned& _u, unsigned& _d,
		float& _r, unsigned& _ngen, float& _x, float& _m ) {
  // Create the command-line parser
  Parser parser( "ES", "Demo of a Evolution Strategy");
  
  parser.addFlag("-v", "--verbose","Verbosely print all things during evolution");
  parser.addFlag("-h", "--help", "Shows this help");
  parser.addParam("-g", "--generations", "20", Param::INT, 
		  "Number of generations the GA will be run" );
  parser.addParam("-l", "--lambda", "20", Param::INT, 
		  "Number of EOs in the population: ES lambda param");
  parser.addParam("-u", "--mu", "20", Param::INT, 
		  "Number of EOs in the new population: ES mu param");
  parser.addParam("-x", "--xover", "0.8", Param::FLOAT, 
		  "Crossover rate or priority: ES mu param" );
  parser.addParam("-m", "--mutation", "0.2", Param::FLOAT, 
		  "Mutation rate or priority"  );
  parser.addParam("-d", "--dim", "2", Param::INT, 
		  "Chromosome/EO length" );
  parser.addParam("-r", "--range", "10.0", Param::FLOAT, 
		  "Range of chromosomes" );
  
  try {
    parser.parse(_argc, _argv);		// Do the parsing
    _verbose = parser.isEnabled("-v");
    _l = parser.getInt("-l");	
    _u = parser.getInt("-u");	
    _d = parser.getInt("-d");	
    _r = parser.getFloat( "-r" );
    _ngen = parser.getInt("-g");	
    _x = parser.getFloat("-x");	
    _m = parser.getFloat("-m");	

    if (parser.isEnabled("-h") || parser.isEnabled("--help") ) {
      parser.printHelp();
      exit(1);
    }
  }
  catch (exception & e) {
    cout << e.what() << endl;
    parser.printHelp();
    exit(1);
  }
}
//@}

//-----------------------------------------------------------------------------

main( int argc, char** argv) {

  unsigned lambda,
    mu,
    numGenerations,
	dim;
  float xOverPr,
    mutPr;			// Operator probabilities
  float range;		// Chromosome range
  bool verbose;
  
  // Obtain command line parameters or default values
  getParams( argc, argv, verbose, lambda, mu, dim, range, numGenerations,
			xOverPr, mutPr );
  
  // Create an initial population
  Pop pop;
  createInitialPop( pop, lambda, dim, range );
  // Create an instance of the evaluation function object 
  vector<float> sample;
  Uniform<float> rndGen( range*(-0.5), range*0.5 );
  for ( unsigned i = 0; i < dim; i ++ ) {
    sample.push_back( rndGen() );
  }
  MinDist thisEvalFunc( sample );

  // Finally, instantiate the algorithm handing it the statistical
  // evaluator. Values for the mutation, xOver and number of generations
  // are default: 0.2, 0.8 and 100;
  EOES es( lambda, thisEvalFunc, pop, 0.2f, 0.8f, 20, true );
  // Now, apply the algorithm to the population
  es( pop );
  // Sort Population
  sort( pop.begin(), pop.end(), SortEO<Chrom>() ); 
  // And print results!
  cout << "minFitness() ....... " << pop[0].fitness() << endl
       << "Value..............  " << pop[0] << endl;
  return 0;
}

