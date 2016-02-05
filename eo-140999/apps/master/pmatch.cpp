// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: 1; c-auto-newline: 1 -*-
// prules.cc
// to test rules


//#include "general.hpp"

#include "rules.hpp"

#ifndef WIN32
#include <string.h>
#include <iostream.h>
#else
#include <string>
#include <iostream>
#endif

#include <stdio.h>
#include <stdlib.h>

void randomGen( string& _str, unsigned _len, unsigned _colors ) {
	for ( unsigned j = 0; j < _len ; j ++ ){
		_str += char( '1' + rand() % _colors );
	}
}

void main() {
    matchString myStr = "123445";
	const unsigned colores = 6, posiciones = 6;
    match aciertos;
  
    cout << "Cadena inicial 123445\n";
    for ( unsigned i = 0; i < 40; i ++ ) {
		string checkStr;
		randomGen( checkStr, posiciones, colores );
		aciertos =  myStr.check(checkStr);
		cout << "Cadena 123445\nCadena " << checkStr <<
			" Plenos " << aciertos.full <<
			" Parciales " << aciertos.color << endl;
	}
	return 0;
      
}
