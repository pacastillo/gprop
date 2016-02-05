// This file generated automagically by genParser.pl from IslandGA
// genParser (c) JJ Merelo, 1999
#include "IslandGA-getParams.h"
#include <util/Parser.h>

/// Parses the program´s command line, by creating a parsing object
void getParams( int _argc, char** _argv, 
		float & _mutPr,
		unsigned & _subPopulations,
		unsigned & _tournamentSize,
		unsigned & _popSize,
		float & _migrationPerc,
		bool & _verbose,
		float & _xOverPr,
		unsigned & _genToMigration,
		unsigned & _numGenerations){
// Create the command-line parser
  Parser parser( _argv[0], "Program that tests an IslandGA ");
  parser.addFlag("-h", "--help", "Shows this help");
  parser.addParam("-m", "--mutPr", "0.2", Param::FLOAT,"Mutation rate, from 0 to 1");
  parser.addParam("-s", "--subPopulations", "2", Param::INT,"Number of subpopulations");
  parser.addParam("-t", "--tournamentSize", "2", Param::INT,"Tournament size");
  parser.addParam("-p", "--popSize", "20", Param::INT,"Population size");
  parser.addParam("-i", "--migrationPerc", "0.1", Param::FLOAT,"Percentage of migrators ");
  parser.addFlag("-v", "--verbose", "true if you want many things printed");
  parser.addParam("-x", "--xOverPr", "0.8", Param::FLOAT,"Crossover rate, from 0 to 1");
  parser.addParam("-g", "--genToMigration", "10", Param::INT,"Generations until migration");
  parser.addParam("-n", "--numGenerations", "20", Param::INT,"Number of generations");
  try {
    parser.parse(_argc, _argv);		// Do the parsing
    _mutPr = parser.getFloat("-m");
    _subPopulations = parser.getInt("-s");
    _tournamentSize = parser.getInt("-t");
    _popSize = parser.getInt("-p");
    _migrationPerc = parser.getFloat("-i");
    _verbose = parser.isEnabled("-v");
    _xOverPr = parser.getFloat("-x");
    _genToMigration = parser.getInt("-g");
    _numGenerations = parser.getInt("-n");
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
