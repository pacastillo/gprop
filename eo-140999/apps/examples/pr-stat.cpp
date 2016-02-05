//-----------------------------------------------------------------------------
// pr-stat.cpp
//-----------------------------------------------------------------------------

#include <util/Uniform.h>
#include <util/EOStat.h>
#include <specific/EOBin/EOBin.h>
#include <generic/EOEvalAll.h>

//-----------------------------------------------------------------------------

typedef EOBin<float>  Chrom;
typedef EOPop<Chrom>  Pop;
typedef EOStat<Chrom> Stat;

//-----------------------------------------------------------------------------

const unsigned NUM_CHROM = 8;
const unsigned BITSxGEN = 4;
const unsigned MIN_GEN = 1;
const unsigned MAX_GEN = 4;

//-----------------------------------------------------------------------------

/* Implementation of an function evaluatio object. Adds all
values of the chromosome */
struct eval: public EOEvalFunc< Chrom > {
  virtual float operator()( Chrom & _eo ) const {
    Uniform< float > on_off( -1, 1 );
    float oo = on_off( );
    _eo.fitness( oo );
    return oo;
  };
};


//-----------------------------------------------------------------------------

main()
{

  Uniform< float > on_off( -1, 1);
  Uniform< unsigned > rndLen(MIN_GEN, MAX_GEN + 1);

  Pop pop;
  unsigned i;
  for ( i = 0; i < NUM_CHROM; i++ ) {
      Chrom chrom( "", rndLen(), BITSxGEN );
      
      for ( unsigned j = 0; j < chrom.length(); j++ )	{
		  float oo = on_off( );
		  chrom.writeGene( j, (oo > 0) ? true: false );
      }
      pop.push_back( chrom );
    }
  
  eval thisEvalFunc;
  EOEvalAll< Chrom > thisEval( thisEvalFunc );
  Stat stat( thisEval );
  stat( pop );
  
  // show population
  for ( i = 0; i < pop.size(); i++) {
    Chrom& chrom = pop[i];
    cout << chrom << " ";
    for (unsigned i = 0; i < BITSxGEN * MAX_GEN - chrom.size(); i++)
      cout << ".";
    cout << "... " << chrom.fitness() << endl;
    }

  // show statistics
  cout << endl
       << "minFitness() ....... " << stat.minFitness() << endl
       << "maxFitness() ....... " << stat.maxFitness() << endl
       << "averageFitness() ... " << stat.averageFitness() << endl
       << "minGenes() ......... " << stat.minGenes() / BITSxGEN << endl
       << "maxGenes() ......... " << stat.maxGenes() / BITSxGEN << endl
       << "averageGenes() ..... " << stat.averageGenes() / BITSxGEN << endl;

  return 0;
}
