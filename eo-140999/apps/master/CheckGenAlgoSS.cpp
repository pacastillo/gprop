// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* CheckGenAlgoSS.cpp
 Check the genetic algorithm step by step
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


#include "mmMatchString.h" // For matching strings
#include "mmRandomAlgo.h"  // for random algoritm
#include "mmGenAlgo.h"     // For the genetic algorithm

#include "rules.h"

// Compulsory global variables. Don´t like this, but GUI obliges

// Random algorithm
mmGenAlgo *anAlgo;
  
// Random string to search for
mmMatchString secretCode;	    

// Possible idle function for a GUI
void Idle() {
	int thisScore;
	string guess;
	match hints;

	const EOPop<mmChrom>& thisPop = anAlgo->getPop();
	const rules& theseRules = anAlgo->getRules();

	anAlgo->singleStep();
	thisScore = -theseRules.dist( thisPop[0] );
	if ( !thisScore ) {
		guess = thisPop[0];
		hints = secretCode.check( guess );
		anAlgo->setLast( guess );
		anAlgo->gimmeAnswers( hints.full, hints.color );
		cout << "String " << secretCode << "\nString " << guess <<
		" Full " << hints.full <<
		" Partial " << hints.color << endl;
		anAlgo->initAlgo();
	}
	copy( thisPop.begin(), thisPop.end(), ostream_iterator<mmChrom>( cout, "\n" ) );

}

main( int argc, char ** argv) {
  unsigned posiciones = 6;
  unsigned colores = 6;
  unsigned initMode = 1;	// Knuth's mode
  unsigned popSize = 20;	// To have easy visualization

  // Initialize random seed
  srand((unsigned)time( NULL ) );

  if ( argc > 1) {
    posiciones = atoi( argv[1] );
    colores = atoi( argv[2] );
  } else {
    cerr << "Usage : " << argv[0] << " [positions] [colors]";
  }

  // Random algorithm
  anAlgo = new mmGenAlgo( posiciones, colores, initMode, popSize );
  
  randomGen( secretCode, posiciones, colores );

  // 
  string guess = anAlgo->gimmeFirst();
  
  // Check against secret
  match hints;
  hints = secretCode.check( guess );
  anAlgo->gimmeAnswers( hints.full, hints.color );

  const EOPop<mmChrom>& thisPop = anAlgo->getPop();

  // Main Loop
  while ( hints.full < posiciones ) {
	anAlgo->initAlgo();
	Idle();
	guess = thisPop[0];
	hints = secretCode.check( guess );
  }

  return(0); // or VC++ will warn on this
}

