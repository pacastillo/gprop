
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <util/Parser.h>

main(int argc, char ** argv)
{
  // parser's constructor needs the name of the program and a little description
  Parser parser( "pr-parsing", "Program to test the parsers.\n\tThis is only a description" );

  // Adding flags is very easy: just type a short name, a long name and a description
  parser.addFlag("-v", "--verbose",  "Verbosely print all things during evolution");
  parser.addFlag("-h", "--help", "Shows this help");

  // Adding params is a little more complicated: type a short name, a long name, a value by default,
  // a type for the param (one of these: PARAM_INT, PARAM_FLOAT or PARAM_STRING), a description and if
  // required or not
  parser.addParam("-v1", "--value1", "10", Param::INT,"Value 1", true);
  parser.addParam("-v2", "--value2", "1.0", Param::FLOAT,"Value 2");
  parser.addParam("-v3", "--value3", "myfile.dat", Param::STRING,"Value 3",true );

  try
  {
    parser.parse(argc, argv);
    cout << "-v " << parser.isEnabled("-v") << endl;
    cout << "--verbose " << parser.isEnabled("--verbose") << endl;
    cout << "-h " << parser.isEnabled("-h") << endl;
    cout << "--help " << parser.isEnabled("--help") << endl;

    cout << "-v1 " << parser.getInt("-v1") << endl;
    cout << "--value1 " << parser.getInt("--value1") << endl;
    cout << "-v2 " << parser.getFloat("-v2") << endl;
    cout << "--value2 " << parser.getFloat("--value2") << endl;

    if (parser.isEnabled("-h") || parser.isEnabled("--help") ) {
      parser.printHelp();
    }
  }
  catch (UException& e)
  {
    cout << e.what() << endl;
    parser.printHelp();
  }

  return 0;
}
	
