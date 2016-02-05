//-----------------------------------------------------------------------------
// pr-eobin.cpp
//-----------------------------------------------------------------------------


#include <iostream>

using namespace std;

#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>
#include <generic/EOGOps.h>

//-----------------------------------------------------------------------------

const unsigned GENES = 4;
const unsigned BITSxGEN = 2;

//-----------------------------------------------------------------------------

typedef EOBin<float> Chrom;

//-----------------------------------------------------------------------------

main()
{
  Chrom chrom("", GENES, BITSxGEN);

  cout << "a binary chromosome with " 
       << GENES << " genes and " 
       << BITSxGEN <<" bits per gene" << endl
       << "chrom ............ " << chrom << endl;

  // testing random
  EORandom<float> random;
  random(chrom);
  cout << "random ........... " << chrom << endl;

  // testing BitFlip
  EOBitFlip<float> bitflip;
  bitflip(chrom);
  cout << "bitflip .......... " << chrom << endl;

  // testing Mutation
  EOMutation<float> mutation(0.25);
  mutation(chrom);
  cout << "mutation(0.25) ... " << chrom << endl;
  
  // testing UniformCrossover
  Chrom chrom0("",GENES, BITSxGEN), chrom1("",GENES, BITSxGEN);
  fill(chrom0.begin(), chrom0.end(), false);
  fill(chrom1.begin(), chrom1.end(), true);
  EOUniformCrossover<Chrom> uxover;
  uxover(chrom0, chrom1);
  cout << "UniformCrossover ... " 
       << chrom0 << "  " << chrom1 << endl;
  
  // testing NxOver
  unsigned i;
  for (i = 1; i < chrom.length(); i++)
    {
      fill(chrom0.begin(), chrom0.end(), false);
      fill(chrom1.begin(), chrom1.end(), true);
      NxOver<Chrom> nxover(i);
      nxover(chrom0, chrom1);
      cout << "NxOver(" << i << ") .......... " 
	   << chrom0 << "  " << chrom1 << endl;
    }
  
  // testing GeneCrossover
  for (i = BITSxGEN; i <= GENES * BITSxGEN; i += BITSxGEN)
    for (unsigned j = 1; j <= GENES * BITSxGEN / i; j++) {
	fill(chrom0.begin(), chrom0.end(), false);
	fill(chrom1.begin(), chrom1.end(), true);
	GeneCrossover<Chrom> gxover(i, j);
	gxover(chrom0, chrom1);
	cout << "GxOver(" << i << ", " << j << ") ....... " 
	     << chrom0 << "  " << chrom1 << endl;
    }
  return 0;
}

