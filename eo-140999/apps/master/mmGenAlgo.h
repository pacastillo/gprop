
// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* mmGenAlgo.h
 Class for genetic mastermind algorithms
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
-----------------------------------------------------------------------------*/


#ifndef _mmGenAlgo_H
#define _mmGenAlgo_H

#include "mmResetGen.h" // for RandomGen
#include "mmAlgo.h"     // Base class
#include "rules.h"

//Mastermind classes
#include "rules.h"

// EO classes
#include <ADT/EOFitness.h>		// for EOEvalFunc

#include <generic/EOEvalAll.h>
#include <generic/EOGOps.h>
#include <generic/EOGenGA.h>
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOselect/EOReplacers.h>

#include <specific/EOString.h>	// for EOString

#include <util/EOStat.h>		// For printing stats
#include <util/Parser.h>		// For parser

// Local MasterMind application objects
#include "mmCoincidenceTerm.h"	// Local termination operator
#include "CharCircMut.h"		// Local mutation operator
#include "ZapMutate.h"			// Local mutation operator
#include "mmResetGen.h"			// Resetting and related operators
#include "mmEvalRules.h"


/** This class encapsulates the algorithmic part of mastermind, in such a way
    that it is independent of the program. The algorithm is supposed to store
the game internally*/
class mmGenAlgo: public mmAlgo{
public:
  /// Ctor
  mmGenAlgo( unsigned _pos, unsigned _c, bool _initMode = 0, unsigned _popSize = 500,
	     float _xOverPr=2, float _mutPr=1, float _zapPr=0, float _trPr=2, float _elite=0.5,
	bool _showStats = true, unsigned _resetLength = 15 )
    : mmAlgo(), posiciones (_pos), colores (_c), totalGuesses( 0 ), 
    totalEvaluated( 0 ), lastPlayed(""), theseRules(), initMode( _initMode ),
    evaluator( theseRules), thisEval( evaluator ), graphEval( thisEval, _showStats ), 
    coach( _elite ), killSome(  _elite ),  chaperon(),
    xOver( _xOverPr ),  transposer( _trPr ), circler('1','0'+colores, _mutPr ),
    zapper('1','0'+colores, _zapPr ), termWhenValid( theseRules ), initPop(),  
    resetter( _popSize, posiciones, colores), 
    resetGen( graphEval, coach, chaperon, killSome, false, _resetLength, resetter ),
    thisAlgorithm( resetGen, termWhenValid )  
  {
    // Add operators to the breeder
    chaperon.addOp( &xOver );
    chaperon.addOp( &transposer );
    chaperon.addOp( &circler );
    chaperon.addOp( &zapper );
    
    // Create the initial population in a random way
    resetter( initPop );
    
  };

  /// Dtor
  virtual ~mmGenAlgo() {};

  /// Issues the first guess
  virtual string gimmeFirst() {
    unsigned i;
    if ( !initMode ) { // Use half colors
      for ( i = 0; i < posiciones; i ++ ) {
	lastPlayed += '1' + i % colores;
      } 
    } else {
      for ( i = 0; i < posiciones; i ++ ) {
	lastPlayed += '1' + i % (colores/2);
      }
    }	
    return lastPlayed;
  }
  

  /// Gets answers from the oracle to the last game
  virtual void gimmeAnswers( unsigned _black, unsigned _white ) {
    match hints={_black,_white};
    if ( lastPlayed == "" ) {
      throw runtime_error ( "No string played so far" );
    }
    theseRules.addRule( lastPlayed, hints);
  }
  
  /** Issues the rest of the guesses
      @return The string to play
  */
  virtual string gimmeNext() {

    initAlgo();
    thisAlgorithm( initPop ); // Runs until coincidences met
    
    // Check the best
    mmChrom checkStr;
    checkStr= *(initPop.begin());
    lastPlayed = checkStr;
    return checkStr;
  };
  
  /// Returns the total number of guesses
  virtual unsigned Guesses() { return totalGuesses; };
  
  /// Returns the total number of combinations evaluated
  virtual unsigned Evaluated() { return totalEvaluated; };

  /** Access to the population, for whatever purposes
      @return a const reference to the population */
  const EOPop< mmChrom >& getPop() const { return initPop;};

  /** Access to the game stored by the algorithm
      @return a const reference to the "rules" of the game, that is, the combinations
	played along with the hints*/
  const rules& getRules() const { return theseRules;};

  /** Initialize the algorithm. Might be useful from outside, if
  the algorithm is going to be used step by step */
  void initAlgo() {
    resetGen.resetCounter();
    graphEval( initPop );     
  }

  /** Do a single step, that is a generation, in the algorithm; useful
  from GUI-type programs, where you need to watch the population evolve */
  void singleStep() {
    resetGen( initPop );
  }

  ///
  void setLast( const string& _guess ) { lastPlayed = _guess;};


private:
  unsigned posiciones, colores, totalGuesses, totalEvaluated;
  string lastPlayed;
  rules theseRules;
  
  /// true if initmode used Knuth's initialization: only half the colors are used
  bool initMode; 
  
  // More Genetic Algorithm structures
  // Evaluate
  mmEvalRules evaluator;
  EOEvalAll<mmChrom > thisEval;
  EOStat<mmChrom > graphEval;
	
  // Go ahead to next generation
  EOSteadyState<mmChrom > coach;
	
  // And now breed and replace
  EOElimWorst< mmChrom > killSome;
  EORandomBreed<mmChrom > chaperon;

  // Genetic operators
  EOXOver2<mmChrom > xOver; 
  EOTranspose<mmChrom > transposer;
  CharCircMutate<mmChrom > circler;
  ZapMutate<mmChrom > zapper;

  //Termination condition
  CoincidenceTerm termWhenValid;
	
  // Generate initial population
  EOPop< mmChrom > initPop; 
  ResetPop resetter;
	
  //And now the algorithm itself
  EOResetGen<mmChrom> resetGen;
  EOGenGA<mmChrom> thisAlgorithm;
  
};

#endif
