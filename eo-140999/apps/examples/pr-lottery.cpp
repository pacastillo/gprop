//-----------------------------------------------------------------------------
// pr-lottery.cpp
//-----------------------------------------------------------------------------

#ifndef _MSC_VER
#include <iostream.h>
#else
#include <iostream>
#endif

using namespace std;

#include <util/uniform.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>
#include <generic/EOselect/EOGSLottery.h>
#include <generic/EOselect/EOReplacers.h>

//-----------------------------------------------------------------------------

typedef EOBin<float> Chrom;
typedef EOPop<Chrom> Pop;

//-----------------------------------------------------------------------------

const unsigned GENES = 2;
const unsigned BITSxGEN = 3;
const unsigned POPSIZE = 8;

//-----------------------------------------------------------------------------

main()
{

  Pop pop;
  EORandom<float> random;
  Uniform<float> uniform(0, 1);
  
  cout << "pop:" << endl;
  for(unsigned i = 0; i < POPSIZE; i++) {
      Chrom chrom("", GENES, BITSxGEN);
      random(chrom);      
      chrom.fitness(uniform());
      pop.push_back(chrom);
      
      cout << chrom << "\t" << chrom.fitness() << endl;
    }

  EOLottery<Chrom> lottery;

  Pop pop2;
  lottery(pop,pop2);

  cout << "lottery(pop):" << endl;
  copy(pop2.begin(), pop2.end(), ostream_iterator<Chrom>(cout, "\n"));

  return 0;
}
	
