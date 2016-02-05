//-----------------------------------------------------------------------------
// EOAgeReplace.h
//-----------------------------------------------------------------------------

#ifndef EOAGEREPLACE_H
#define EOAGEREPLACE_H

//-----------------------------------------------------------------------------

#include <algorithm>                 // sort, merge, min

#include "../../ADT/EOPop.h"      // EOPop
#include "../../ADT/EOReplace.h"  // EOReplace
#include "../EOAge.h"             // EOAge
#include "../../util/EOStat.h"    // FitStat

//-----------------------------------------------------------------------------

template<class Chrom> class EOAgeReplace: public EOReplace<Chrom>
{
 public:
  enum Strategy {Lineal, Proportional, Bilineal};

  EOAgeReplace(const Strategy _strategy = Lineal, 
	       const unsigned _minLifeTime = 1, 
	       const unsigned _maxLifeTime = 7): 
    strategy(_strategy),
    minLifeTime(_minLifeTime),
    difLifeTime((_maxLifeTime - _minLifeTime ) / 2),
    maxLifeTime(_maxLifeTime) {}

  unsigned min() const { return minLifeTime; }
  unsigned dif() const { return difLifeTime; }
  unsigned max() const { return maxLifeTime; }
  
  void min(const unsigned _min) 
    { 
      minLifeTime = _min; 
      difLifeTime = (maxLifeTime - minLifeTime) / 2;
    }
  void max(const unsigned _max) 
    {
      maxLifeTime = _max; 
      difLifeTime = (maxLifeTime - minLifeTime) / 2;
    }

  void operator()(EOPop<Chrom>& breeders, EOPop<Chrom>& pop) const
    {  
      // just put them all together
      copy(breeders.begin(), breeders.end(), 
	   back_insert_iterator<EOPop<Chrom> >(pop));
      
      // some figures are needed
      FitStat stat(pop);
      
      for (unsigned chrom = 0; chrom != pop.size(); chrom++)
        {
          switch(pop[chrom].lifeTime())
            {
            case Chrom::baby:
              // set life time if the chrom is just born
              setAge(pop[chrom], 
		     stat);
              break;
            case Chrom::aged:
              // ... but some day life ends
              pop[chrom].swap(pop.back());
              pop.pop_back();
              chrom--;
              break;
            default:
              // life goes on ...
              pop[chrom].birthday(); 
              break;
            }
	}
    }
  
 private:
  void setAge(Chrom& chrom, FitStat& stat) const
    {
      switch(strategy)
	{
	case Lineal:
          chrom.lifeTime(minLifeTime + 2.0 * difLifeTime * (chrom.fitness() - stat.min()) / (stat.max() - stat.min()));
	  break;
        case Proportional:
          chrom.lifeTime(std::min((unsigned)maxLifeTime, (unsigned)(minLifeTime + difLifeTime * chrom.fitness() / stat.average())));
	  break;
	case Bilineal:
          if (chrom.fitness() < stat.average())
            chrom.lifeTime(minLifeTime + difLifeTime * (chrom.fitness() - stat.min()) / (stat.average() - stat.min()));
	  else chrom.lifeTime(0.5 * (minLifeTime + maxLifeTime) + difLifeTime * (chrom.fitness() - stat.average()) / (stat.max() - stat.average())); 
          break;
	}
    }

  Strategy strategy;
  unsigned minLifeTime, difLifeTime, maxLifeTime;
};

//-----------------------------------------------------------------------------

#endif EOAGEREPLACE_H
