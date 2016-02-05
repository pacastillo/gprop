// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-

/*   EOtutorial-ex1.cpp
     
Program that demos a simple genetic algorithm using EO. First examle
of the EO tutorial. 

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
  @see EOSGA
  */
//@{

#include <generic/EOSGA.h>
#include "SGA-getParams.h"	// For the command-line parser Parser
#include "deJongF2.h"           // Includes the fitness function object
 
//-----------------------------------------------------------------------------

typedef EOBin<float> Chrom;
typedef EOPop<Chrom> Pop;	// Chrom defined as EOBin<float>

//-----------------------------------------------------------------------------

main( int argc, char** argv) {

  unsigned numGenes = 2,  // Number of genes in the chromosome
    numBits ,             // Number of bits per gene
    numGenerations,       // Number of generations
    popSize;              // Population size
  float xOverPr,      // Crossover rate, from 0 to 1
    mutPr;            // Mutation rate, from 0 to 1
  bool verbose;       // true if you want many things printed
  
  // Obtain command line parameters or default values
  getParams( argc, argv, verbose, popSize, numBits, numGenerations,
	     xOverPr, mutPr );  
  // Create an initial population
  Pop pop;
  // Create an instance of the evaluation function object 
  deJongF2 thisEvalFunc;
  // Finally, instantiate the algorithm handing it the statistical
  // evaluator. Values for the mutation, xOver and number of generations
  // are default: 0.2, 0.8 and 100;
  EOSGA sga( thisEvalFunc, pop, popSize, numGenes, numBits, mutPr, xOverPr, 
	     numGenerations, verbose );
  // Now, apply the algorithm to the population
  sga( pop );
  // Sort Population. First is the best
  sort( pop.begin(), pop.end(), SortEO<Chrom>() ); 
  // And print results!
  cout << "Best fitness ....... " << pop[0].fitness() << endl
       << "Value...............  " << pop[0] << endl;
  return 0;
}

