//-----------------------------------------------------------------------------
// EOGSLottery.h
//-----------------------------------------------------------------------------

#ifndef EOLOTTERY_H
#define EOLOTTERY_H

//-----------------------------------------------------------------------------

#include <algorithm>
#include <numeric> // for accumulate
#include <functional>

#include <ADT/EOSelector.h>
#include <util/Uniform.h>

//-----------------------------------------------------------------------------

/** 
 * EOLottery: an selection operator, which get the fitness from every 
 * chrommosome (fitness must have a standard typecast to float), and them 
 * creates a new population with fitness-proportionate selection 
 */

template<class EOT> class  EOLottery: public EOSelector<EOT>
{
 public:
  ///
  EOLottery(){};
  
  ///
  virtual ~EOLottery() {};
  
  /// 
  virtual void operator() ( EOPop<EOT>& _pop, EOPop<EOT>& _breeders ) const {

    // fitness of chromosomes
    vector<float> fitness(_pop.size());
		
    // get the fitness from every chrommosome
    // Fitness must have a standard typecast to float
    unsigned i;
    for ( i = 0; i < _pop.size(); i++)
      fitness[i] = _pop[i].fitness();
    
    // calculates the sum of the scores
    float sum = accumulate(fitness.begin(), fitness.end(), 0.0);
		
    // divides every fitness by the sum
    for ( i = 0; i < fitness.size(); i++) {
      fitness[i] /= sum;
    }
    
    // acumutales scores
    partial_sum(fitness.begin(), fitness.end(), fitness.begin());
    
    // generates random numbers
    vector<float> random(_pop.size());
    generate(random.begin(), random.end(), Uniform<float>(0, 1) );
    sort(random.begin(), random.end(), less<float>());
		
    // selection of chromosomes
    unsigned fpos = 0;                   // position in fitness vector
    unsigned rpos = 0;                   // position in random vector
    do {
      if(random[rpos] < fitness[fpos]) {
	_breeders.push_back(_pop[fpos]);
	rpos++;
      }
      else {
	fpos++;
      }
    } while ((_breeders.size() < _pop.size()) && (fpos < _pop.size()));
  }
};

//-----------------------------------------------------------------------------

#endif EOLOTTERY_H
