//-----------------------------------------------------------------------------
// EOBinOp.h
//-----------------------------------------------------------------------------

#ifndef _EOBINOP_H
#define _EOBINOP_H

//-----------------------------------------------------------------------------

#ifdef _MSC_VER
#define min _MIN
#endif

#include <ADT.h>
#include <util/Uniform.h>
#include <specific/EOBin/EOBin.h>
#include <functional>
#include <algorithm>

//-----------------------------------------------------------------------------
/** 
 * EOBitFlip: operator for binary chromosomes 
 * Changes one bit in the chromosome every time that is aplied
 */
//-----------------------------------------------------------------------------

template<class fitness = float> class EOBitFlip: public MonOp< EOBin<fitness> >
{
 public:
  typedef EOBin<fitness> Chrom;

  ///
  EOBitFlip( float _priority = 1 ): 
    MonOp< Chrom > ( _priority ) {}
  
  ///
  void operator()( Chrom& chrom ) const
    {
      Uniform<unsigned> uniform( 0, chrom.size() - 1);
      
      applyAt( chrom, uniform( ) );
    }
  
  ///
  void applyAt( Chrom& chrom, unsigned bit, unsigned = 0 ) const
    {
      if (chrom[bit])
	chrom[bit] = false;
      else
	chrom[bit] = true;
    }
};

//-----------------------------------------------------------------------------
/** 
 * EORandom: operator for binary chromosomes 
 * random change for all bits in the chromosome
 */
//-----------------------------------------------------------------------------

template<class fitness = float> class EORandom: public MonOp< EOBin<fitness> >
{
 public:
  typedef EOBin<fitness> Chrom;

  ///
  EORandom( float _priority = 1 ): 
    MonOp< Chrom > ( _priority ) {}

  ///  
  void operator()( Chrom& chrom ) const
    {
      for (unsigned bit = 0; bit < chrom.size(); bit++)
	applyAt( chrom, bit );
    }

  ///
  void applyAt( Chrom& chrom, unsigned bit, unsigned = 0 ) const {
    Uniform<float> uniform(-1,1);
	if (uniform() > 0)
	chrom[bit] = true;
      else
	chrom[bit] = false;
    }

};


//-----------------------------------------------------------------------------
/** 
 * EOMutation: operator for binary chromosomes 
 * Mutation change for all bits in the chromosome
 */
//-----------------------------------------------------------------------------

template<class fitness = float> 
class EOMutation: public MonOp< EOBin<fitness> >
{
 public:
  typedef EOBin<fitness> Chrom;

  ///
  EOMutation( float _rate, float _priority = 1 ): 
    MonOp< Chrom > ( _priority ), rate( _rate ) {}
  
  ///
  void operator()( Chrom& chrom ) const
    {
      for (unsigned bit = 0; bit < chrom.size(); bit++)
	applyAt( chrom, bit );
    }

  ///
  void applyAt( Chrom& chrom, unsigned bit, unsigned = 0 ) const {   
    Uniform<float> rnd(0, 1);
      if (rnd() < rate)
	chrom[bit] = !chrom[bit];
  }

 private:
  float rate; /// rate of mutation
};


#endif _EOBINOP_H
