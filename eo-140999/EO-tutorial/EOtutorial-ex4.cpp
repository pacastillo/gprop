// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-

/*   EOtutorial-ex4.cpp
     
Program that demos a evolutionary algorithm using EO. 5th examle
of the EO tutorial.This example demos the use of new population-level ops.

(c) J. J. Merelo, 1999
     
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

/** @name A Simple Genetic Algorithm with floating point values.

This example goes a bit further in the exploration of EO capabilities: 
a float chromosome is evolved, which does not allow us to use the SGA
class, which is geared only for binary chromosomes.

  @see EOAlgo
  @see EOVector
  @see EOEasyGA
  */
//@{


// Generic EO includes
#include <EOGeneric.h>		// Include all generic classes
#include <util/Uniform.h>

// Specific ex4 includes
#include "EORangeMutate.h"	// For the newly defined operator	
#include "ex4-getParams.h"	// For the command-line parser Parser
#include "deJongF2float.h"	// Includes the fitness function object
 
//-----------------------------------------------------------------------------

typedef EOVector<float,float> Chrom;
typedef EOPop<Chrom> Pop;	// Chrom defined as EOBin<float>

//-----------------------------------------------------------------------------

main( int argc, char** argv) {

  unsigned numGenes = 2,  // Number of genes in the chromosome
    numGenerations,       // Number of generations
    popSize,              // Population size
    tournamentSize;       // Tournament size
  float xOverPr,      // Crossover rate, from 0 to 1
    mutPr;            // Mutation rate, from 0 to 1
  bool verbose;       // true if you want many things printed
  
  // Obtain command line parameters or default values
  getParams( argc, argv, verbose, popSize, numGenerations,
	     xOverPr, mutPr, tournamentSize );  

  // Create an initial population and the population-level operators
  Pop pop;
  unsigned i, j;
  Uniform<float> u( 0, 1);
  for ( j = 0; j < popSize; j ++ ) {
    Chrom* aChrom = new Chrom;	
    for ( i = 0; i < numGenes; i ++ ) {
      aChrom->push_back( u() );
    }
    pop.push_back( *aChrom );	  
    if ( verbose ) {
      copy( aChrom->begin(), aChrom->end(), ostream_iterator<int>( cout, " ") );
      cout << endl;
    }
    delete aChrom;    
  }
  // Create an instance of the evaluation function object 
  deJongF2float thisEvalFunc;
  // From that, create an object that evaluates all the population
  EOEvalAll<Chrom>  eval( thisEvalFunc) ;
  // Then, an object that select who's going to reproduce
  EOTournament<Chrom> select( tournamentSize );
  // Another object that mates them
  EORank<Chrom> breed( popSize/ tournamentSize );
  // And another that weeds out the worst
  EOElimWorst<Chrom> replace( float(tournamentSize)/popSize );
  // This one check if the algorithm has finishd or not
  EOGenTerm<Chrom> term( numGenerations );
 
  // Then the EO-level operators
  // This is a mutation-like operator
  EORangeMutate<Chrom> rndMut( 0.1, 0, 1, mutPr );
  // And this is the classical 2-point crossover
  EOXOver2<Chrom> xOver(xOverPr); 

  // Add operators to breeder
  breed.addOp( &rndMut );
  breed.addOp( &xOver );

  // Then, the algorithm itself is instantiated
  EOEasyGA<Chrom> ga( eval, select, breed, term, replace, verbose );  
  // Now, apply the algorithm to the population
  try {
    eval( pop );
    ga( pop );
  } catch (exception& _e ) {
    cerr << "Panic! --> " << _e.what() << endl;
  }
  // Sort Population. First is the best
  sort( pop.begin(), pop.end(), SortEO<Chrom>() ); 
  // And print results!
  cout << "Best fitness ....... " << pop[0].fitness() << endl
       << "Value...............  " << pop[0] << endl;
  return 0;
}

