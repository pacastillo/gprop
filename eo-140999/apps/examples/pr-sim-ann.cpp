#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <generic/EOSimAnnAlg.h>
#include <specific/EOLinearFreezer.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>
#include <util/Parser.h>


/**
 * Evaluation Function to minimize the number of 1's in an EO
 */
struct MinOnes : public EOEvalFunc<EOBin<float> >
{
  virtual float operator() (EOBin<float>& _eo) const
  {
    int numZeros = 0;
    for ( unsigned k = 0; k < _eo.length(); k ++ )
      if (!_eo.readGene (k))
        numZeros++;

    return numZeros;
  }
};

main(int argc, char ** argv)
{

  // parser's constructor needs the name of the program and a little description
  Parser parser( "pr-sim-ann", "Program to test the Simulated Annealing Algorithm" );

  // Adding flags is very easy: just type a short name, a long name and a description
  parser.addFlag("-v", "--verbose",  "Verbosely print all things during evolution");
  parser.addFlag("-h", "--help", "Shows this help");

  try
  {
    parser.parse(argc, argv);

    if (parser.isEnabled("-h"))
    {
      parser.printHelp();
      return 0;
    }

    unsigned size = 15;
    EOBin<float> eo;

    srand( (unsigned)time( NULL ) );

    // the initial eo contains only zeros
    for ( unsigned i = 0; i < size; i ++ )
      eo.push_back( false );

    // Print initial 
    cout << "initial eo: ";
    copy( eo.begin(), eo.end(), ostream_iterator<bool>( cout, " ") );
    cout << endl;

    EOPop<EOBin<float> > pop;
    pop.push_back(eo);

    // apply simulated annealing to find the eo with more 1's
    MinOnes func;
    EOBitFlip< float > op;
    EOLinearFreezer lf(0.9f);

    EOSimAnnAlg<EOBin<float> > alg(func, op, 10, (float) 0.9, (float) 0.1, lf, parser.isEnabled("-v"));
  
    alg(pop);

    // print the best
    cout << "final   eo: ";
    copy( pop[0].begin(), pop[0].end(), ostream_iterator<bool>( cout, " ") );
    cout << endl;

  }
  catch (UException& e)
  {
    cout << e.what() << endl;
    parser.printHelp();
  }
  
  return 0;
}
	
