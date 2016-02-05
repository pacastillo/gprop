
#include "ex2-getParams.h"
#include <util/Parser.h>

/// Parses the program´s command line, by creating a parsing object
void getParams( int _argc, char** _argv, 
		bool& _verbose, unsigned& _p, 
		unsigned& _ngen, float& _x, float& _m ) {
  // Create the command-line parser
  Parser parser( _argv[0], "Demo of a Simple Genetic algorithms a la Holland");
  
  parser.addFlag("-v", "--verbose", "Verbosely print all things during evolution");
  parser.addFlag("-h", "--help", "Shows this help");
  parser.addParam("-g", "--generations", "20", Param::INT, 
		  "Number of generations the GA will be run" );
  parser.addParam("-p", "--popsize", "20", Param::INT, 
		  "Number of EOs in the population");
  parser.addParam("-x", "--xover", "0.8", Param::FLOAT, 
		  "Crossover rate or priority" );
  parser.addParam("-m", "--mutation", "0.2", Param::FLOAT, 
		  "Mutation rate or priority"  );
  
  try {
    parser.parse(_argc, _argv);		// Do the parsing
    _verbose = parser.isEnabled("-v");
    _p = parser.getInt("-p");	
    _ngen = parser.getInt("-g");	
    _x = parser.getFloat("-x");	
    _m = parser.getFloat("-m");	

    if (parser.isEnabled("-h") || parser.isEnabled("--help") ) {
      parser.printHelp();
      exit(1);
    }
  }
  catch (UException & e){
    cout << e.what() << endl;
    parser.printHelp();
    exit(1);
  }
}
