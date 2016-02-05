//---------------------------------------------------------------------------
// pr-view.cpp
//---------------------------------------------------------------------------

#include <iostream>

#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>
#include <specific/EOBin/EOView.h>

//-----------------------------------------------------------------------------

typedef EOBin<float> Chrom;
typedef EOView<float> View;

//-----------------------------------------------------------------------------

unsigned GENES = 2;
unsigned BITSxGEN = BITS(float);

//-----------------------------------------------------------------------------

main()
{
  Chrom chrom("",GENES, BITSxGEN);    // First one is ID
  View view(chrom);
  
  cout << "a binary chromosome with " 
       << GENES << " genes and " 
       << BITSxGEN <<" bits per gene:\n\t"
       << chrom << "\n\t"
       << view << endl;
  
  // testing random
  EORandom< float > random;
  random( chrom );
  View view2(chrom);
  cout << "after random: \n\t" 
       << chrom << "\n\t"
       << view2 << endl;
  
  // testing BitFlip
  EOBitFlip< float > bitflip;
  bitflip( chrom );
  View view3(chrom, -1, 1);
  cout << "after bitflip:\n\t" 
       << chrom << "\n\t"
       << view3 << endl;


  // testing view(chrom, min, max)
  GENES = 3;
  BITSxGEN = 8;

  Chrom chrom2("", GENES, BITSxGEN);
  View view4(chrom2, -1, 1);
  
  cout << "a binary chromosome with " 
       << GENES << " genes and " 
       << BITSxGEN <<" bits per gene:\n\t"
       << chrom2 << "\n\t"
       << view4 << endl;
  
  // testing random
  random( chrom2 );
  View view5(chrom2, -1, 1);
  cout << "after random: \n\t" 
       << chrom2 << "\n\t"
       << view5 << endl;
  
  // testing BitFlip
  bitflip( chrom2 );
  View view6(chrom2, -1, 1);
  cout << "after bitflip:\n\t" 
       << chrom2 << "\n\t"
       << view6 << endl;

  return 0;
}
	
