// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-

/*   SGA.cpp
     
Program that demos a simple genetic algorithm using EO
     
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/** @name A Simple Genetic Algorithm

This is probably the easiest way to start with EO. It´s a simple Genetic Algorithm
as especified by Goldberg: Binary Chromosome (#Chrom#, that is, #EOBin<float>#),
Lottery selection, Random Reproduction, generational termination( it ends
after a fixed number of generations and mutation and 2 point crossover
as genetic operators. It optimizes a simple function: add the number of ones in 
the chromosome. All the operators are hidden inside the SGA class.\\

  The only thing the user has to define is, obviously, the fitness function, which
  in EO becomes a fitness function {\em class}, the initial population, which in
  this case is created randomly, and the parameters for mutation and crossover.\\

  If you want a more powerful way of defining the GA, and more control over it,
  use other algorithm (#EOAlgo# class, like EasyGA).\\

  @see EOAlgo
  @see EOBin
  @see EOTerm
  @see EOSGA
  */
//@{

#include <generic/EOSGA.h>
#include <util/Parser.h>		// For the command-line parser Parser

//-----------------------------------------------------------------------------

typedef EOBin<float> Chrom;
typedef EOPop<Chrom> Pop;	// Chrom defined as EOBin<float>

//-----------------------------------------------------------------------------
/** Implementation of a function evaluation object.\
Adds the number of ones in a binary chromosome
*/
struct NumOnes: public EOEvalFunc< Chrom > {
  virtual float operator()( Chrom & _eo ) const {
    float numOnes = 0;
    for ( unsigned i = 0; i < _eo.length(); i ++ ) 
      numOnes += ( _eo[i] == true)?1:0 ;
    _eo.fitness( numOnes );
    return numOnes;
  };
};

/// Parses the program´s command line, by creating a parsing object
void getParams( int _argc, char** _argv, 
		bool& _verbose, unsigned& _p, unsigned& _nb,
		unsigned& _ngen, float& _x, float& _m ) {
  // Create the command-line parser
  Parser parser( "SGA", "Demo of a Simple Genetic algorithms a la Holland");
  
  parser.addFlag("-v", "--verbose", "Verbosely print all things during evolution");
  parser.addFlag("-h", "--help", "Shows this help");
  parser.addParam("-g", "--generations", "20", Param::INT, 
		  "Number of generations the GA will be run" );
  parser.addParam("-p", "--popsize", "20", Param::INT, 
		  "Number of EOs in the population");
  parser.addParam("-x", "--xover", "0.8", Param::FLOAT, 
		  "Crossover rate or priority" );
  parser.addParam("-m", "--mutation", "0.2", Param::FLOAT, 
		  "Mutation rate or priority"  );
  parser.addParam("-nb", "--numbits", "20", Param::INT, 
		  "Chromosome/EO length" );
  
  try {
    parser.parse(_argc, _argv);		// Do the parsing
    _verbose = parser.isEnabled("-v");
    _p = parser.getInt("-p");	
    _nb = parser.getInt("-nb");	
    _ngen = parser.getInt("-g");	
    _x = parser.getFloat("-x");	
    _m = parser.getFloat("-m");	

    if (parser.isEnabled("-h") || parser.isEnabled("--help") ) {
      parser.printHelp();
      exit(1);
    }
  }
  catch (UException & e){
    cout << e.what() << endl;
    parser.printHelp();
    exit(1);
  }
}
//@}

//-----------------------------------------------------------------------------

main( int argc, char** argv) {
  const unsigned numGenes = 1;
  unsigned NUM_CHROM,
    NUM_BITS,
    numGenerations;
  float xOverPr,
    mutPr;
  bool verbose;
  
  // Obtain command line parameters or default values
  getParams( argc, argv, verbose, NUM_CHROM, NUM_BITS, numGenerations,
	     xOverPr, mutPr );  
  // Create an initial population
  Pop pop;
  // Create an instance of the evaluation function object 
  NumOnes thisEvalFunc;
  // Finally, instantiate the algorithm handing it the statistical
  // evaluator. Values for the mutation, xOver and number of generations
  // are default: 0.2, 0.8 and 100;
  EOSGA sga( thisEvalFunc, pop, NUM_CHROM, numGenes, NUM_BITS, mutPr, xOverPr, 
	     numGenerations, verbose );
  // Now, apply the algorithm to the population
  sga( pop );
  // Sort Population
  sort( pop.begin(), pop.end(), SortEO<Chrom>() ); 
  // And print results!
  cout << "minFitness() ....... " << pop[0].fitness() << endl
       << "Value..............  " << pop[0] << endl;
  return 0;
}

