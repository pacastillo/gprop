//-----------------------------------------------------------------------------
// pr-age.h
//-----------------------------------------------------------------------------

#include <EOGeneric.h>   // NxOver, EOAgeGA
#include <EOSpecific.h>  // EOBin
#include <EOutil.h>      // Uniform

//-----------------------------------------------------------------------------

const unsigned NUM_GENES = 2;
const unsigned BITSxGEN = 4;
const unsigned POP_SIZE = 8;

//-----------------------------------------------------------------------------

class Chrom: public EOBin<float>, public EOAge
{
public:
  Chrom(const unsigned num_genes = 1, const unsigned bits_x_gen = 1, 
	const string id = ""):
    EOBin<float>(id, num_genes, bits_x_gen), EOAge() {}

  Chrom(const Chrom& chrom): 
    EOBin<float>(chrom), EOAge(chrom) {}

  void swap(Chrom& other)
    {
      EOBin<float>::swap(other);
      EOAge::swap(other);
    }

  friend ostream& operator<<(ostream& os, const Chrom& chrom)
    {
      const EOBin<float>& eobin = chrom;
      const EOAge& eoage = chrom;

      return os << eobin << "  fit: " << chrom.fitness() << "  age: " << eoage;
    }
};

//-----------------------------------------------------------------------------

typedef EOPop<Chrom> Pop;

//-----------------------------------------------------------------------------

struct Bin: public EOEvalFunc< Chrom > 
{
  float operator()(Chrom& chrom) const
    {
      return accumulate(chrom.begin(), chrom.end(), 0);
    }
};

//-----------------------------------------------------------------------------

main()
{
  Pop pop;
  EORandom<float> random;
  Chrom chrom(NUM_GENES, BITSxGEN);
  for (unsigned i = 0; i < POP_SIZE; i++)
    {
      random(chrom);
      chrom.fitness(i);
      pop.push_back(chrom);
    }

  Bin fitness;
  EOEvalAll<Chrom> eval(fitness);
  EOStat<Chrom> graphEval( eval, true );  // True is for verbose
  
  EORank<Chrom> transform(1);
  //  EOMutation<Chrom> mutation(0.05);
  //  transform.addOp((EOOp<Chrom>*)&mutation);
  NxOver<Chrom> nxover(2);
  transform.addOp(&nxover);
  
  EOGenTerm<Chrom> term(100);
  
  EOAgeGA<Chrom> agega(graphEval, transform, term);
  
  try
    {
      agega(pop); 
    }
  catch(UException& e)
    {
      cout << e.what() << endl;
    }
 
  cout << "the best: " << pop.front() << endl;

  return 0;
}

//-----------------------------------------------------------------------------
