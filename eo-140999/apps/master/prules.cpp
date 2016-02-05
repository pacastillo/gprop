// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-
// prules.cc
// to test rules

//@{
/** @name prules.cpp
\begin{center} Random MasterMind \end{center}

Program to play MasterMind with random string generation. As soon as 
a string which meets all current turns is met, it is played. Made
basically to compare with the Genetic MasterMind.

@author J. J. Merelo
@version 0.1

*/
//@}


//STL classes
#include <string>
#include <iostream>

//C classes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Mastermind classes
#include "rules.hpp"

using namespace std;

void randomGen( string& _str, unsigned _len, unsigned _colors ) {
	_str="";
	for ( unsigned j = 0; j < _len ; j ++ ){
		_str += char( '1' + rand() % _colors );
	}
}

void main( int argc, char* argv[]) {

    const unsigned numJugadas= 40;
    unsigned posiciones = 6;
    unsigned colores = 6;

    // generate string
    srand((unsigned)time( NULL ) );

    if ( argc > 1) {
      posiciones = atoi( argv[1] );
      colores = atoi( argv[2] );
    }

    string acertada;	     // string to search
	randomGen( acertada, posiciones, colores );
    matchString myStr = acertada;
    match aciertos={0,0};
    rules myRules(numJugadas);
    unsigned genTotal = 0;                        // no. cadenas generadas 
    cout << "Cadena inicial " << acertada << "\n";
    unsigned i;
    for ( i = 0; 
	(i < numJugadas) && (aciertos.full < posiciones); i ++ ) {
		
		unsigned coincidencias;
		string checkStr;
		
		do {// se generan cadenas aleatorias a lo bestia
			unsigned foo, bar;
			randomGen( checkStr, posiciones, colores );
			coincidencias = 
				myRules.numMatches( checkStr, foo, bar ) ;
			//	      cout << "Cadena " << checkStr << " Aciertos " << 
			//		coincidencias << endl;
			genTotal++;
			
		}while (coincidencias != i);
		
		aciertos =  myStr.check(checkStr);
		myRules.addRule( matchString( checkStr ), aciertos );
		cout << "Cadena " << acertada << "\nCadena " << checkStr <<
			" Plenos " << aciertos.full <<
			" Parciales " << aciertos.color << endl;
    }

    cout << "Numero de tiradas " << i << " Combinaciones generadas " 
	 << genTotal << endl;

}
