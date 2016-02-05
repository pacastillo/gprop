// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* mmAlgo.h
 Generic Abstract class for mastermind algorithms
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


#ifndef _mmRandomAlgo_H
#define _mmRandomAlgo_H

#include "mmResetGen.h" // for RandomGen
#include "mmAlgo.h"
#include "rules.h"

/** This class encapsulates the algorithmic part of mastermind, in such a way
    that it is independent of the program. The algorithm is supposed to store
the game internally*/

class mmRandomAlgo: public mmAlgo{
public:
  /// Ctor
  mmRandomAlgo( unsigned _pos, unsigned _c )
    : mmAlgo(), posiciones (_pos), colores (_c), totalGuesses( 0 ), 
  totalEvaluated( 0 ), lastPlayed(""), theseRules(){};

  /// Dtor
  virtual ~mmRandomAlgo() {};

  /// Issues the first guess
  virtual string gimmeFirst() {
    randomGen( lastPlayed, posiciones, colores );
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

  /// Issues the rest of the guesses
  virtual string gimmeNext() {
    string checkStr;
    unsigned distance;
    if (!theseRules.numRules() ) {
      throw runtime_error( "Rules array empty" );
    }
    do {// Generate random strings
      randomGen( checkStr, posiciones, colores );
      distance =  theseRules.dist( checkStr ) ;
      totalEvaluated++;
    }while (distance);
    totalGuesses++;
    lastPlayed = checkStr;
    return checkStr;
  };

  /// Returns the total number of guesses
  virtual unsigned Guesses() { return totalGuesses; };

  /// Returns the total number of combinations evaluated
  virtual unsigned Evaluated() { return totalEvaluated; };

  private:
  unsigned posiciones, colores, totalGuesses, totalEvaluated;
  string lastPlayed;
  rules theseRules;
};

#endif
