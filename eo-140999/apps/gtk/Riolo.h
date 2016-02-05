#ifndef _RIOLO_H
#define _RIOLO_H

#include <math.h>
#include <ADT/EOEvalFunc.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOView.h>

// Some useful typedefs
typedef EOBin<float> Chrom;
typedef EOView<float> View;

/**
 riolo = x + |sin(32 * 3.1415927 * x)| max = 1.98442447185516357421875

*/

class Riolo: public EOEvalFunc< Chrom >
{
  float operator()( Chrom& _chrom) const {      
    View view(_chrom, 0, 1);
    float val = view[0];
    //    cout <<  val + fabs(sin(32 * 3.1415927 * val)) << endl;
    return val + fabs(sin(32 * 3.1415927 * val));
  }
};

#endif
