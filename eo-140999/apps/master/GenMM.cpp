// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-
/** @name Genetic Mastermind
\begin{center} Genetic Mastermind \end{center}

Program to play MasterMind using Genetic Algorithms. It uses an standard
genetic algorithm, with integer representation, plus an "circle mutation" 
operator.

@author J. J. Merelo
@version 0.1

*/
//@{
    //@Include:CharCircMut.h


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
#include <generic/EOEasyGA.h>
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOselect/EOReplacers.h>

#include <specific/EOString.h>	// for EOString

#include <util/EOStat.h>		// For printing stats
#include <util/Parser.h>		// For parser

#include "CharCircMut.h"		// Local mutation operator


/// random generation of strings
void randomGen( string& _str, unsigned _len, unsigned _colors ) {
  _str="";
  char lastChar = '1' + _colors -1;
  Uniform<char> uniform('1',lastChar);
  for ( unsigned j = 0; j < _len ; j ++ ){
    _str += uniform(  );
  }
}

/** Evaluation function object: it counts how many mastermind rules have been met
 */
struct EvalRules: public EOEvalFunc< EOString<float> > {
	
	EvalRules( const rules& _rules)
		: theseRules( _rules ) {};
	
	virtual ~EvalRules() {};
	
	virtual float operator()( EOString<float> & _eo ) const {
		unsigned numBlack;
		unsigned numWhite;
		unsigned coincidencias = 
			theseRules.numMatches( _eo, numBlack, numWhite ) ;
		float fitness = coincidencias + numBlack*100 + numWhite*10;
		return fitness;
	};
	
private:
	const rules& theseRules;
};

/** Termination object for Genetic MasterMind \\ 
Terminates when the number of rules met coincides with the number of rules so far,
or if _maxCounter steps have happened without change
@param _rules number of rules that have to be met
@param _maxCounter maximum number of generations without finding a solutions
*/
class CoincidenceTerm: public EOTerm< EOString<float> > {
public:

	/// Ctors/dtors
	CoincidenceTerm( const rules& _rules, unsigned _maxCounter )
		: EOTerm< EOString<float> > (), 
		coincidences( 0 ), numGenerations(0), maxCounter( _maxCounter ), 
		counter( 0 ), theseRules( _rules ){};

	virtual ~CoincidenceTerm() {};

	/** Returns false if the training has to stop, true if it
	*  continues */
	virtual bool operator() ( const EOPop< EOString<float> >& _vEO ) {
		// get the best
	  const EOString<float> * minEO= _vEO.begin();
	  unsigned foo, bar;
	  unsigned thisScore;
	  numGenerations++;
	  if ( minEO->fitness() == lastFitness ) {
		counter++;
	  } else {
		counter = 1;
	  }
	  lastFitness = minEO->fitness();
	  // reboot after a number of generations
	  if ( counter % maxCounter == 0 )
		  return false;

	  if ( minEO != NULL ) {
		thisScore = theseRules.numMatches( *minEO, foo, bar);
		return thisScore != coincidences;
	  } else {
		return true;
	  }
	}

	void setCoincidences( unsigned _c ) { coincidences = _c; };
	void resetCounter() { counter = 0; };
	unsigned getNumGenerations() const { return numGenerations;};

private:
	unsigned coincidences, numGenerations, maxCounter, counter;
	float lastFitness;
	const rules& theseRules;
};


/// Parses the program´s command line, by creating a parsing object
void getParams( int _argc, char** _argv, 
		unsigned& _p, unsigned& _ngen, unsigned& _r,
		float& _x, float& _m, float& _t,
		unsigned& _c, unsigned& _l, string& _secretComb ) {

  // Create the command-line parser
  Parser parser( "Mastermind", "Genetic Mastermind; solve mastermind using GAs");
  
  parser.addParam("-g", "--generations", "20", Param::INT, "Number of generations the GA will be run" );
  parser.addParam("-p", "--popsize", "1000", Param::INT, "Number of EOs in the population");
  parser.addParam("-x", "--xover", "2", Param::FLOAT, "Crossover rate or priority" );
  parser.addParam("-m", "--mutation", "1", Param::FLOAT, "Mutation rate or priority"  );
  parser.addParam("-t", "--transpose", "2", Param::FLOAT, 
		  "Transpose rate or priority"  );
  parser.addParam("-c", "--colors", "6", Param::INT, "Number of colors" );
  parser.addParam("-l", "--length", "6", Param::INT, "Combination length" );
  parser.addParam("-r", "--reset", "15", Param::INT, "Generations before reset" );
  parser.addParam("-s", "--secret", "", Param::INT, "Secret combination, using numbers as colors" );
  
  try {
    parser.parse(_argc, _argv);		// Do the parsing
    _p = parser.getInt("-p");	
    _ngen = parser.getInt("-g");	
    _r = parser.getInt("-r" );
    _x = parser.getFloat("-x");	
    _m = parser.getFloat("-m");	
    _t = parser.getFloat("-t");
    _c = parser.getInt("-c");
    _l = parser.getInt("-l");
    _secretComb = parser.getString("-s");
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

int main( int argc, char* argv[]) {
	
	const unsigned numJugadas= 40;

	// GA variables
	unsigned popSize, generations;
	float xOverPr, mutPr, trPr;

	// Mastermind variables
	unsigned posiciones, colores, resetLength;
	string secretCombination;
	
	// Obtain command line parameters or default values
	getParams( argc, argv, popSize, generations, resetLength,
		   xOverPr, mutPr, trPr, colores, posiciones, 
		   secretCombination);
	
	// Check if a combination has been set by the user
	string acertada;		 // string to search
	matchString myStr;
	if ( secretCombination != "" ) {
	  // Check that it's correct
	  char highest = '1';// highest digit
	  for ( unsigned i = 0; i < secretCombination.length(); i++ ) {
	    if (!isdigit( secretCombination[i] ) ) {
	      cerr << "Incorrect secret combination: You must use digits";
	      exit(1);
	    }
	    highest= 
	      (secretCombination[i]>highest)?secretCombination[i]:highest;
	  }
	  // Assign values to length and number of colors from it
	  posiciones = secretCombination.length();
	  colores = highest - '0';
	  acertada = secretCombination;
	  string str = "";
	  for ( unsigned j = 0; j < secretCombination.length() ; j ++ ){
	    str += secretCombination[j];
	  }
	  myStr = str;
	} else {
	  // generate string
	  randomGen( acertada, posiciones, colores );
	  myStr = acertada;
	}

	match aciertos={0,0};
	rules myRules;
	cout << "Cadena inicial " << acertada << "\n";
	EvalRules evaluator( myRules );
	
	// More Genetic Algorithm structures
	// Evaluate
	EvalAll<EOString<float> > thisEval( evaluator );
	EOStat<EOString<float> > graphEval( thisEval, true );
	
	// Go ahead to next generation
	float select = (float)0.3;
	EOSteadyState<EOString<float> > coach( select );
	
	// And now breed and replace
	unsigned subPop = unsigned( select*popSize );
	EOCrowding< EOString<float> > killSome( subPop );
	EORank<EOString<float> > chaperon( subPop );
	
	EOXOver2<EOString<float> > xOver( xOverPr ); 
	chaperon.addOp( &xOver );
	EOTranspose<EOString<float> > transposer( trPr );
	chaperon.addOp( &transposer );
	CharCircMutate<EOString<float> > circler('1','0'+colores, mutPr );
	chaperon.addOp( &circler );
	
	//Termination condition
	CoincidenceTerm termWhenValid( myRules, resetLength );
	
	// First bet must be as diverse as possible
	// To compute initial string, we generate the order, and then submit 
	// to a random number of permutations
	EOString<float> initStr;
	unsigned i;
	for ( i = 0; i < posiciones; i ++ ) {
		initStr += '1' + i % colores;
	}
	
	// Generate initial population
	EOPop< EOString<float> > initPop; 
	for ( unsigned j = 0; j < popSize; j ++ ) {
		EOString<float> newStr;
		randomGen( newStr, posiciones, colores );
		initPop.push_back( newStr );
	}
	
	//And now the population itself
	EOEasyGA<EOString<float> > 
		thisAlgorithm( graphEval, coach, chaperon, 
		termWhenValid, killSome, false);
	
	// Initial values
	aciertos =	myStr.check(initStr);
	myRules.addRule( matchString( initStr ), aciertos );
	cout << "Cadena " << acertada << "\nCadena " << initStr <<
		" Plenos " << aciertos.full <<
		" Parciales " << aciertos.color << endl;

	// Evaluate the initial population
	graphEval( initPop );       

	unsigned lastGen = 1;
	unsigned totalEv = popSize;
	
	for ( i = 1; 
	      (i < numJugadas) && (aciertos.full < posiciones); i ++ ) {
	  try {
	    termWhenValid.setCoincidences( i );
	    termWhenValid.resetCounter();
	    unsigned thisScore;
	    EOString<float>* checkStr;
	    do {
	      thisAlgorithm( initPop ); // Runs until coincidences met
			
	      // Check the best
	      checkStr= initPop.begin();
	      unsigned foo, bar;
	      thisScore = myRules.numMatches( *checkStr, foo, bar);
	      // Reboot population
	      if ( thisScore < i ) {
		initPop.erase( initPop.begin(), initPop.end() );
		for ( unsigned j = 0; j < popSize; j ++ ) {
		  EOString<float> newStr;
		  randomGen( newStr, posiciones, colores );
		  initPop.push_back( newStr );
		}
		totalEv += popSize;
		termWhenValid.resetCounter();
		graphEval( initPop );  // Evaluate again
	      }
	    } while ( thisScore < i );

	    aciertos =  myStr.check( *checkStr);
	    myRules.addRule( *checkStr, aciertos );
	    cout << "Cadena " << acertada << "\nCadena " << *checkStr <<
	      " Plenos " << aciertos.full <<
	      " Parciales " << aciertos.color << endl;

	    totalEv += (termWhenValid.getNumGenerations()-lastGen)*unsigned(popSize*select) ;
	    lastGen = termWhenValid.getNumGenerations();

	  } catch ( UException &e ) { 
	    cout << e.what() << endl;
	  } 
	}
	
	cout << "Numero de tiradas " << i << " Combinaciones generadas " 
	     << totalEv << endl;	  

	return 0;
}
