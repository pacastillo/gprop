//-----------------------------------------------------------------------------
// tsp.cpp
//-----------------------------------------------------------------------------

#include <math.h>    // sqrt
#include <iostream>  // cout 
#include <fstream>   // ifstream
#include <stl.h>     //
#include <EO++.h>    // 
#include <EOGeneric.h>

//-----------------------------------------------------------------------------

#define FILE "eil51.tsp"

//-----------------------------------------------------------------------------

const unsigned POP_SIZE = 256;
const unsigned CHROM_SIZE = 51;
const unsigned GENERATIONS = 1000;

//-----------------------------------------------------------------------------
  
struct City 
{ 
  unsigned name;
  float x, y;

  friend ostream& operator<<(ostream& os, const City& city)
    {
      return os << city.name << " " << city.x << " " << city.y;
    }
};

typedef EOVector<unsigned, float> TSPChrom;
typedef EOPop<TSPChrom> TSPPop;

//-----------------------------------------------------------------------------

ostream& operator<<(ostream& os, const TSPChrom& chrom)
{
  copy(chrom.begin(), chrom.end(), ostream_iterator<unsigned>(os, " "));
  return os << " fit: " << 1 / chrom.fitness();
}

/// Fitness for travellins salesman problem
class TSPFitness: public EOEvalFunc<TSPChrom> 
{
public:
  TSPFitness(): cities(CHROM_SIZE + 1)
    {
      ifstream file(FILE);
      if (!file)
	{
	  cerr << "where is " << FILE << endl;
	  exit(EXIT_FAILURE);
	}
      
      for (unsigned i = 1; i < cities.size(); i++)
	{
	  file >> cities[i].name >>  cities[i].x >>  cities[i].y;
	  
	  if (!file)
	    { 
	      cerr << FILE << " seems to be wrong" << endl;
	      exit(EXIT_FAILURE);
	    } 
	}
    }
  
  virtual ~TSPFitness() {}

  float operator()(TSPChrom& chrom) const
    {
      float sum = 0;
    
      for (TSPChrom::iterator i = chrom.begin(); i != chrom.end(); i++)
	{
	  TSPChrom::iterator j = i + 1;
	  if (j == chrom.end())
	    j = chrom.begin();

	  float diffx = cities[*i].x - cities[*j].x;
	  float diffy = cities[*i].y - cities[*j].y;
	  sum += sqrt(diffx * diffx + diffy * diffy);
	}
      sum = 1 / sum;
      chrom.fitness(sum);
      return sum;
    }

  void allCities(TSPChrom& chrom) const
    {
      chrom.clear();
      
      for (unsigned i = 1; i < cities.size(); i++)
	chrom.push_back(cities[i].name);
    }

private:
  vector<City> cities;
};

//-----------------------------------------------------------------------------

void InitPop(TSPPop& pop, const TSPFitness& fitness, 
	     const unsigned& pop_size, const unsigned& chrom_size)
{
  TSPChrom chrom;
  fitness.allCities(chrom);
  
  while (pop.size() < pop_size)
    {
      TSPChrom tmp = chrom;
      random_shuffle(tmp.begin(), tmp.end());
      pop.push_back(tmp);
    }
}

//-----------------------------------------------------------------------------
/// Michalewicz's inver-over operator
class InverOver: public MonOp<TSPChrom> 
{
public:
  InverOver(const float& _prob, const TSPPop& _pop, TSPFitness& _fitness):
    MonOp<TSPChrom>( 1 ), prob( _prob ), pop( _pop ), fitness(_fitness) {}
    
  void operator()(TSPChrom& chrom) const
    {
      TSPChrom temp = chrom;
   
      Uniform<unsigned> select_city(0, temp.length());
      Uniform<unsigned> select_chrom(0, pop.size());
      Uniform<float> uniform01(0, 1);
      
      unsigned city = temp[select_city()], city2;
      
      for(;;)
	{
	  // find the second city
	  if (uniform01() < prob)
	    {
	      do 
		city2 = temp[select_city()];
	      while (city == city2);
	    }
	  else
	    { 
	      const TSPChrom& chrom2 = pop[select_chrom()];
	      TSPChrom::const_iterator i = 
		find(chrom2.begin(), chrom2.end(), city);
	      if (++i == chrom2.end())
		i = chrom.begin();
	      city2 = *i;
	    }
	  
	  // apply inversion
	  TSPChrom::iterator pcity = find(temp.begin(), temp.end(), city);
	  TSPChrom::iterator pcity2 = find(temp.begin(), temp.end(), city2);
	  if (pcity == temp.end())
	    {
	      cout << "no encuentro " << city << " en " << temp << endl;
	      exit(EXIT_FAILURE);
	    }
	  if (pcity2 == temp.end())
	    {
	      cout << "no encuentro " << city2 << " en " << temp << endl;
	      exit(EXIT_FAILURE);
	    }

	   // stop condition
	  TSPChrom::iterator prev, next;
	  if (pcity == temp.begin())
	    prev = temp.end() - 1;
	  else
	    prev = pcity - 1;
	  if (pcity == temp.end())
	    next = temp.begin();
	  else
	    next = pcity + 1;
	  if (city2 == *prev || city2 == *next)
	    break;
	  	  
	  int num_swaps = ((pcity2 - pcity + temp.length()) 
			   % temp.length()) / 2;

	  while(num_swaps--)
	    {
	      if (++pcity == temp.end())
		pcity = temp.begin();
	      
	      swap(*pcity, *pcity2);
	     
	      if (pcity2 == temp.begin())
		pcity2 = temp.end() - 1;
	    }
	
	  // going on with the next city
	  city = city2;
	}
      
      // is better the new one?
      fitness(temp);
      //if (temp.fitness() > chrom.fitness())
      chrom.swap(temp);
    } 
  
  void applyAt(TSPChrom& _eo, unsigned _i, unsigned _j ) const
    {
    }

private:
  float prob;
  const TSPPop& pop;
  TSPFitness fitness;
};

//-----------------------------------------------------------------------------

main() {

  TSPPop pop;  
  TSPFitness fitness; 
  
  InitPop(pop, fitness, POP_SIZE, CHROM_SIZE);
  
  EOEvalAll<TSPChrom> eval(fitness);
  eval(pop);

  // Go ahead to next generation
  float select = (float)0.2;
  EOSteadyState<TSPChrom> coach( select );

  // And now breed 
  EORank<TSPChrom> chaperon( select*pop.size() );

  // inverOver operator
  InverOver inverover(0.2, pop, fitness);
  chaperon.addOp( &inverover );

  // Eliminate the worst
  EOElimWorst<TSPChrom> popKiller( select );
  
  //Termination condition
  EOGenTerm<TSPChrom> ngenerations( GENERATIONS );
	
  //And now the population itself
  EOEasyGA<TSPChrom> ga( eval, coach, chaperon, ngenerations, popKiller, true );
  
  // Evaluate the initial population
  eval( pop );
  
  // Apply the genetic algorithm, and generate evolvedPop
  ga( pop );
  
  // show me the best
  cout << "best: " << endl << pop[0] << endl; 

  return 0;
}

//-----------------------------------------------------------------------------
