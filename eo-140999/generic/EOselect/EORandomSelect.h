//-----------------------------------------------------------------------------
// EOGSRandomSelect.h
//-----------------------------------------------------------------------------

#ifndef EORANDOMSELECT_H
#define EORANDOMSELECT_H

//-----------------------------------------------------------------------------

#include <algorithm>
#include <numeric>    // for accumulate
#include <functional>

#include <ADT/EOSelector.h>
#include <util/Uniform.h>

//-----------------------------------------------------------------------------

/** 
 * EORandomSelect: an selection operator, which selects random chromosomes
 */
template<class EOT> class EORandomSelect: public EOSelector<EOT>
{
 public:
  ///
  EORandomSelect(const float& _rate = 0.4)
    : EOSelector<EOT>(), rate(_rate) {};
  
  ///
  virtual ~EORandomSelect() {};
  
  /// 
  virtual void operator() ( EOPop<EOT>& _pop, EOPop<EOT>& _breeders ) const {
    // generates random numbers
    Uniform<unsigned> uniform(0, _pop.size());
    unsigned num_chroms = (unsigned)(rate * _pop.size());

    // selection of chromosomes
    do {
      _breeders.push_back(_pop[uniform()]);
    } while (_breeders.size() < num_chroms);
  }
 private:
  float rate;
};

//-----------------------------------------------------------------------------

#endif EOGSRANDOMSELECT_H
