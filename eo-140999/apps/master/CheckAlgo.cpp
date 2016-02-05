// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* RandMM.cpp
 Random algorithm that plays mastermind
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

main( int argc, char ** argv) {
  unsigned posiciones = 6;
  unsigned colores = 6;
  unsigned algorithm = 0;	// 0 for random, 1 for genetic

  // Initialize random seed
  srand((unsigned)time( NULL ) );

  if ( argc > 1) {
    algorithm = atoi( argv[1] );
    posiciones = atoi( argv[2] );
    colores = atoi( argv[3] );
  } else {
    cerr << "Usage : CheckAlgo [0|1 for random or genetic] [positions] [colors]";
  }

  // Random algorithm
  mmAlgo* anAlgo;
  if ( algorithm ) {
    anAlgo= new mmGenAlgo( posiciones, colores );
  } else {
    anAlgo = new mmRandomAlgo( posiciones, colores );
  }
    

  // Random string to search for
  mmMatchString secretCode;	    
  randomGen( secretCode, posiciones, colores );

  // 
  string guess = anAlgo->gimmeFirst();
  
  // Check against secret
  match hints;
  hints = secretCode.check( guess );
  anAlgo->gimmeAnswers( hints.full, hints.color );

  // Main Loop
  while ( hints.full < posiciones ) {
    guess = anAlgo->gimmeNext();
    hints = secretCode.check( guess );
    anAlgo->gimmeAnswers( hints.full, hints.color );
    cout << "String " << secretCode << "\nString " << guess <<
      " Full " << hints.full <<
      " Partial " << hints.color << endl;
  }

  return(0); // or VC++ will warn on this
}

