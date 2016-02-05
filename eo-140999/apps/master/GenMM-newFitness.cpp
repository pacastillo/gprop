// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-
/** @name Genetic Mastermind
\begin{center} Genetic Mastermind \end{center}

 Program to play MasterMind using Genetic Algorithms. It uses an standard
 genetic algorithm, with integer representation, plus an "circle mutation" 
 operator.
 
  @author J. J. Merelo
  @version 1.5
  
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
//@{
//@Include:CharCircMut.h

// for isDigit
#include <ctype.h>

//STL classes
#include <string>
#include <iostream>

using namespace std;

//Mastermind classes
#include "rules.h"

// EO classes
#include <ADT/EOFitness.h>		// for EOEvalFunc

#include <generic/EOEvalAll.h>
#include <generic/EOGOps.h>
#include <generic/EOGenGA.h>
#include <generic/EOselect/EORandomBreed.h>
#include <generic/EOselect/EOReplacers.h>

#include <specific/EOString.h>	// for EOString

#include <util/EOStat.h>		// For printing stats
#include <util/Parser.h>		// For parser

// Local MasterMind application objects
#include "mmCoincidenceTerm.h"	// Local termination operator
#include "CharCircMut.h"		// Local mutation operator
#include "ZapMutate.h"			// Local mutation operator
#include "mmResetGen.h"			// Resetting and related operators
#include "mmOutput.h"			// For device-independent output
#include "mmEvalRules.h"		// Evaluation function

//_______________________________________________________________________________

/// the object evolved in this application, a string with integer fitness
typedef EOString<int> mmChrom;


//_______________________________________________________________________________

/// Parses the program´s command line, by creating a parsing object
void getParams( int _argc, char** _argv, 
			   unsigned& _p, unsigned& _r,
			   float& _x, float& _m, float& _z, float& _t,
			   unsigned& _c, unsigned& _l,
			   string& _secret, float& _elite) {
	
	// Create the command-line parser
	Parser parser( "Mastermind", 
		"Genetic Mastermind; solve mastermind using GAs");
	
	parser.addParam("-p", "--popsize","400", Param::INT, 
		"Number of EOs in the population");
	parser.addParam("-x", "--xover", "2", Param::FLOAT, 
		"Crossover rate or priority" );
	parser.addParam("-m", "--mutation","1", Param::FLOAT,
		"Mutation rate or priority"  );
	parser.addParam("-z", "--zapping","0", Param::FLOAT, 
		"Zapping rate or priority"  );
	parser.addParam("-t", "--transpose", "2", Param::FLOAT, 
		"Transpose rate or priority"  );
	parser.addParam("-c", "--colors", "6", Param::INT, "Number of colors" );
	parser.addParam("-l", "--length", "6", Param::INT, "Combination length" );
	parser.addParam("-r", "--reset", "15", Param::INT,
		"Generations before reset" );
	parser.addParam("-s", "--secret", _secret, Param::STRING, "Secret combination" );
	parser.addParam("-e", "--elite", "0.5", Param::FLOAT,
		"Percentage of pop changed each generation" );
	
	try {
		parser.parse(_argc, _argv);		// Do the parsing
		_p = parser.getInt("-p");		
		_r = parser.getInt("-r" );
		_x = parser.getFloat("-x");	
		_m = parser.getFloat("-m");	
		_z = parser.getFloat("-z");	
		_t = parser.getFloat("-t");
		_c = parser.getInt("-c");
		_l = parser.getInt("-l");
		_secret = parser.getString("-s");
		_elite = parser.getFloat("-e");
	}
	catch (UException & e)
    {
		cout << e.what() << endl;
    }
	catch (exception & e)
    {
		cout << e.what() << endl;
    }
}
//@}


//_______________________________________________________________________________

int main( int argc, char* argv[], char *envp[] ) {
	
	const unsigned numJugadas= 40;
	
	// GA variables
	unsigned popSize = 500;
	float xOverPr=2, mutPr=1, zapPr=0, trPr=2, elite=0.5;
	bool showStats = true;
	
	// Mastermind variables
	unsigned posiciones=6, colores=6, resetLength=15;
	string secretCombination = "";
	
	
	// Check if it's been called as a CGI
	bool thisIsCGI = false;
	mmOutput out( thisIsCGI?mmOutput::CGI:mmOutput::STDOUT );
	unsigned maxPosiciones = 15;
	unsigned minColores = 6;
	out.printHeader();

	// Get parameters from the command line or implicit
	if ( getenv( "QUERY_STRING" ) ) {		
		secretCombination.insert(0,getenv( "QUERY_STRING" ));
		thisIsCGI = true;
		showStats = false;
		maxPosiciones = 8;
	} else // Obtain command line parameters or default values
		getParams( argc, argv, popSize, resetLength,
	       xOverPr, mutPr, zapPr, trPr, colores, posiciones, 
		   secretCombination, elite );
    
	
	// generate string
	string acertada;		 // string to search
	if ( secretCombination != "" ) {
		posiciones = secretCombination.length();
		char maxDigit = '0';
		for ( unsigned i = 0; i < posiciones; i++ ) {
			if (!isdigit(secretCombination[i]) || secretCombination[i] == '0' ){
				out.printError( "Incorrect combination: only digits 1-9 are valid" );
				exit(1);
			}
			if (  posiciones > maxPosiciones ) {
				out.printError( "Incorrect combination: max 8 digits are allowed");
				exit(1);
			}
			maxDigit = (secretCombination[i] > maxDigit )?secretCombination[i]:maxDigit;
		}
		colores = maxDigit - '0' ;
		colores = ( colores <= minColores)?minColores:colores;
		acertada = secretCombination;
	} else
		randomGen( acertada, posiciones, colores );
	
	mmMatchString myStr = acertada;
	match aciertos={0,0};
	rules myRules;
	string iStr = "You played " + acertada + "\0";
	out.printMsg( iStr );
	out.printMsg( "\nAnd here's what the computer is playing");
	mmEvalRules evaluator( myRules );
	
	// More Genetic Algorithm structures
	// Evaluate
	EOEvalAll<mmChrom > thisEval( evaluator );
	EOStat<mmChrom > graphEval( thisEval, showStats );
	
	// Go ahead to next generation
	EOSteadyState<mmChrom > coach( elite );
	
	// And now breed and replace
	EOCrowding< mmChrom > killSome( elite  );
	EORandomBreed<mmChrom > chaperon;
	
	EOXOver2<mmChrom > xOver( xOverPr ); 
	chaperon.addOp( &xOver );
	EOTranspose<mmChrom > transposer( trPr );
	chaperon.addOp( &transposer );
	CharCircMutate<mmChrom > circler('1','0'+colores, mutPr );
	chaperon.addOp( &circler );
	ZapMutate<mmChrom > zapper('1','0'+colores, zapPr );
	chaperon.addOp( &zapper );
	
	//Termination condition
	CoincidenceTerm termWhenValid( myRules );
	
	// First bet must be as diverse as possible
	// To compute initial string, we generate the order, and then submit 
	// to a random number of permutations
	mmChrom initStr;
	unsigned i;
	for ( i = 0; i < posiciones; i ++ ) {
		initStr += '1' + i % colores;
	}
	
	// Generate initial population
	EOPop< mmChrom > initPop; 
	ResetPop resetter( popSize, posiciones, colores);
	resetter( initPop );
	
	
	//And now the algorithm itself
	EOResetGen<mmChrom> resetGen( graphEval, coach, chaperon, killSome, false, resetLength, resetter );
	EOGenGA<mmChrom> thisAlgorithm( resetGen, termWhenValid );
	
	// Initial values
	aciertos =	myStr.check(initStr);
	myRules.addRule( mmMatchString( initStr ), aciertos );
	out.printCombination( initStr, aciertos.full, aciertos.color );
	unsigned totalEv = popSize;
	
	for ( i = 1; 
	(i < numJugadas) && (aciertos.full < posiciones); i ++ ) {
		try {
			
			resetGen.resetCounter();
			mmChrom checkStr;
			
			graphEval( initPop );       
			thisAlgorithm( initPop ); // Runs until coincidences met
			
			// Check the best
			checkStr= *(initPop.begin());
			aciertos =  myStr.check( checkStr);
			myRules.addRule( checkStr, aciertos );
			
			out.printCombination( checkStr, aciertos.full, aciertos.color );	
			
		} catch ( UException &e ) { 
			cout << e.what() << endl;
		} 
    }


	// total Evaluated = initial pop + evaluated after hypermutation + normally evaluated
	totalEv += resetGen.getTotalEvaluated() + resetGen.getEvaluated();
	if ( !thisIsCGI ) {
		cout << "\nCombinations played " << i << " and generated " 
			<< totalEv << endl;
	} else {
		cout << "\n<br><em>" << totalEv << "</em> combinations have been evaluated out of " << pow( colores, posiciones ) << " possible <br></body></html>";
	}
	
    return 0;
  }
