//-----------------------------------------------------------------------------
// EOStat.h
//-----------------------------------------------------------------------------

#ifndef EOSTAT_H
#define EOSTAT_H

//-----------------------------------------------------------------------------
#if ! (defined(WIN32) || defined( _WIN32 ) )
#include <values.h>  // MAXINT, MAXFLOAT
#else
#define MAXINT 0xFFFFFFFF
#define MAXFLOAT 1e31f
#endif


#include <ADT/EOPop.h>
#include <ADT/EOFitness.h>

//-----------------------------------------------------------------------------

/**
 * class to get statistic from populations
 * @author GeNeura Team
 */

template<class EOT> 
class EOStat: public EOEvaluator<EOT>{
 public:
  /// Evaluator to get statistics from
  EOStat( EOEvaluator<EOT>& _thisEval, bool _verbose = false)
    :thisEval( _thisEval ), verbose( _verbose ), gen_count( 0 ) {};

  /// forwards to EOEvaluator setFunc
  virtual void Ef( const EOEvalFunc< EOT> & _ef ) {
    thisEval.Ef( _ef );
  }

    
  /** This is decorator: adds functionality to EOEvaluators; that is why
  * it starts calling the object it has inside, and then performs whatever
  * it is intended to. In the case of EOGrapher, it just computes max, min and
  * average fitness. Not very efficient (runs over the list 3 times) but quite
  * elegant
  */
  virtual void operator() ( EOPop< EOT >& _pop ) {
     
    thisEval( _pop );// Calls evaluator
    gen_count++;	// Number of generations;

    min_fitness = max_fitness = _pop[0].fitness();
    min_genes = max_genes = _pop[0].length();
    average_fitness = average_genes = 0;

    for (unsigned i = 0; i < _pop.size(); i++)
      {
	float fitness = _pop[i].fitness();
	unsigned genes = _pop[i].length();
	
	if (fitness < min_fitness)
	  min_fitness = fitness;
	else if (fitness > max_fitness)
	  max_fitness = fitness;
	
	average_fitness += fitness;
	
	if (genes < min_genes)
	  min_genes = genes;
	else if (genes > max_genes)
	  max_genes = genes;
	
	average_genes += genes;
      }
    
    average_fitness /= _pop.size();
	if ( gen_count > 1 ) {
		online_fitness = 
			( online_fitness + average_fitness/(gen_count-1))
				*((gen_count-1.0)/gen_count);
	} else {
		online_fitness = average_fitness;
	}
    average_genes /= _pop.size();

	if ( verbose ) {
		cout << " minF " << minFitness() << endl
       << " maxF " << maxFitness() << endl
       << " avgF " << averageFitness() << endl
	   << " onlF " << onlineFitness() << endl
       << " minG " << minGenes() << endl
       << " maxG " << maxGenes() << endl
       << " avgG " << averageGenes() << endl << endl;
	}

  }

  ///
  float minFitness() const { return min_fitness; }
  
  ///
  float maxFitness() const { return max_fitness; }
  
  ///
  float averageFitness() const { return average_fitness; }

  ///
  float onlineFitness() const { return online_fitness; };
  
  /// 
  unsigned minGenes() const { return min_genes; };

  /// 
  unsigned maxGenes() const { return max_genes; };
  
  /// 
  float averageGenes() const { return average_genes; };
  
 private: 
  
  EOEvaluator<EOT>& thisEval;
  bool verbose;

  float min_fitness, max_fitness, average_fitness, 
	  online_fitness,average_genes;
  unsigned min_genes, max_genes, gen_count;  
};

//-----------------------------------------------------------------------------

/**
 * a simpler class to get fitness statistics from a population
 */

class FitStat
{
 public:
  template<class Chrom> FitStat(const EOPop<Chrom>& pop)
    {
      Min = MAXFLOAT;
      Max = -MAXFLOAT;
      Average = 0;

      for (unsigned i = 0; i < pop.size(); i++)
	{
	  float fitness = pop[i].fitness();

	  if (fitness < Min)
	    Min = fitness;
	  else if (fitness > Max)
	    Max = fitness;

	  Average += fitness;
	}
      
      Average /= pop.size();
    }
  
  float min() const { return Min; }
  float max() const { return Max; }
  float average() const { return Average; }
   
  friend ostream& operator<<(ostream& os, const FitStat& stat)
    {
      return os << stat.max() << " " << stat.average() << " " << stat.min();
    }
  
 private:
  float Min, Max, Average;
};
//-----------------------------------------------------------------------------

/**
 * a simpler class to get length statistics from a population
 */

class LenStat
{
 public:
  template<class Chrom> LenStat(const EOPop<Chrom>& pop)
    {
      Min = MAXINT;
      Max = 0;
      Average = 0;

      for (unsigned i = 0; i < pop.size(); i++)
	{
	  unsigned len = pop[i].length();

	  if (len < Min)
	    Min = len;
	  else if (len > Max)
	    Max = len;

	  Average += len;
	}
      
      Average /= pop.size();
    }
  
  unsigned min() const { return Min; }
  unsigned max() const { return Max; }
  float average() const { return Average; }

  friend ostream& operator<<(ostream& os, const LenStat& stat)
    {
      return os << stat.max() << " " << stat.average() << " " << stat.min();
    }

 private:
  unsigned Min, Max;
  float Average;
};

//-----------------------------------------------------------------------------

/**
 * class to get age statistics from a population
 */

class AgeStat
{
 public:
  template<class Chrom> AgeStat(const EOPop<Chrom>& pop)
    {
      Min = MAXINT;
      Max = 0;
      Average = 0;
      
      for (unsigned i = 0; i < pop.size(); i++)
	{
	  unsigned life_time = pop[i].lifeTime();
	  
	  if (life_time < Min)
	    Min = life_time;
	  else if (life_time > Max)
	    Max = life_time;
	  
	  Average += life_time;
	}
      
      Average /= pop.size();
    }
  
  unsigned min() const { return Min; }
  unsigned max() const { return Max; }
  float average() const { return Average; }

  friend ostream& operator<<(ostream& os, const AgeStat& stat)
    {
      return os << stat.max() << " " << stat.average() << " " << stat.min();
    }

 private: 
  unsigned Min, Max;
  float Average;
};

//-----------------------------------------------------------------------------

#endif EOSTAT_H
