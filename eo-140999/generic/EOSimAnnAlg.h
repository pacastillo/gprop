#ifndef _EO_SIM_ANN_ALG_H_
#define _EO_SIM_ANN_ALG_H_

#include <math.h> // For exp
#include <ADT.h>
#include <util/Uniform.h>

/**
 * Abstrac implementation of a freezer.
 */
class EOSimAnnFreezer
{
  public:

    /**
     * Destructor must be virtual
     */
    virtual ~EOSimAnnFreezer() {};

    /**
     * Freezes the temperature.
     * @param _t Temperature
     */
    virtual void operator () ( float &_t ) = 0;
};

/**
 * Implementation of a Simulated annealing algorithm using EO.
 */
template<class EOT>
class EOSimAnnAlg: public EOAlgo<EOT>
{
#ifdef _MSC_VER
        typedef EOT::EOFitT fitT;
#else
        typedef typename EOT::EOFitT fitT;
#endif  
  public:

    /** 
     * Constructs the algorithm.
     * @param _evalFunc     Evaluation function to be minimized during the simulated annealing process
     * @param _op           Makes changes in the configuration to get elements in the neighbourhood of the current one
     * @param _numChanges   Number of random changes allowed a a temperature
     * @param _initTemp     Initial temperature
     * @param _minTemp      Minimal temperature allowed
     * @param _freezer      Freezes the temperature
     * @param _verbose      If true sets verbose mode on
     */
    EOSimAnnAlg( EOEvalFunc<EOT>&  _evalFunc,
                 MonOp<EOT>& _op,
                 unsigned _numChanges,
                 float _initTemp,
                 float _minTemp,
                 EOSimAnnFreezer& _freezer,
                 bool _verbose = false )
      :EOAlgo<EOT>(), repEvalFunc(_evalFunc), repOp(_op), repNumChanges(_numChanges),
       repInitTemp(_initTemp), repMinTemp(_minTemp), repFreezer(_freezer),repVerbose(_verbose){};

    /**
     * Destructor must be virtual
     */
    virtual ~EOSimAnnAlg() {};

    /**
     * Sets the Evaluation Function
     * @param _evalFunc Evaluation to be minimized
     */
    virtual void evalFunc( const EOEvalFunc<EOT>& _evalFunc )
    {
      repEvalFunc = _evalFunc;
    };
  
    /**
     * Sets the operator to generate elements in the neighbourhood of the current one
     * @param _op The operator
     */
    virtual void op( const MonOp<EOT>& _op )
    {
      repOp = _op;
    };
  
    /**
     * Sets the number of random changes to be made in a temperature
     * @param _numChanges The number of changes
     */
    virtual void numChanges ( unsigned _numChanges )
    {
      repNumChanges = _numChanges;
    };
  
    /**
     * Sets the initial temperature
     * @param _initTemp Initial temperature
     */
    virtual void initTemp ( float _initTemp )
    {
      repInitTemp = _initTemp;
    };

    /**
     * Sets the minimun temperature
     * @param _minTemp Minimun temperature
     */
    virtual void minTemp ( float _minTemp )
    {
      repMinTemp = _minTemp;
    };

    /**
     * Sets the freezer
     * @param _freezer
     */
    virtual void freezer ( EOSimAnnFreezer& _freezer )
    {
      repFreezer = _freezer;
    };

    /**
     * Returns the Evaluation Function
     */
    virtual const EOEvalFunc<EOT>& evalFunc( void )
    {
      return repEvalFunc;
    };
  
    /**
     * Returns the operator to generate elements in the neighbourhood of the current one
     */
    virtual const MonOp<EOT>& op( void )
    {
      return repOp;
    };
  
    /**
     * Returns the number of random changes to be made in a temperature
     */
    virtual unsigned numChanges ( void )
    {
      return repNumChanges;
    };
  
    /**
     * Returns the initial temperature
     */
    virtual float initTemp ( void )
    {
      return repInitTemp;
    };

    /**
     * Returns the minimun temperature allowed
     */
    virtual float minTemp ( void )
    {
      return repMinTemp;
    };

    /**
     * Returns the freezer
     */
    virtual const EOSimAnnFreezer& freezer ( void )
    {
      return repFreezer;
    };

    /**
     * Run the algorithm. Applies the simulated Annealing process to each element of the population separately.
     * @param _pop Population
     */
    virtual void operator () ( EOPop<EOT>& _pop ) 
      {
	const unsigned popSize = _pop.size();
	float t = repInitTemp;

	while (t > repMinTemp) { 
	  if (repVerbose) cout << "\nt = " << t << "\n---------------------------------------------"<<endl;
	  
	  for (unsigned i=0 ; i<repNumChanges ; i++) {
	    //if (repVerbose) cout << i << ":\n";	    
	    // generate a new configuration for each element of the population
	    for (unsigned j=0 ; j<popSize ; j++) {
	      EOT newOne = _pop[j];
	      
	      // generate a new configuration in the neighbourhood
	      repOp(newOne);
	      
	      // calculate the increment of energy
	      fitT delta = repEvalFunc(newOne) - repEvalFunc(_pop[j]);
	      
	      Uniform<float> uniform( 0, 1);
	      if ( delta < 0 || ( uniform() < exp((-delta)/t) ) ) {
		_pop[j] = newOne;
		cout << " [ old <= new ] "<<endl;
	      }
	      
	      /*if (repVerbose) {
		cout << "    ";
		for ( unsigned k = 0 ; k < _pop[j].length() ; k++ ) cout << _pop[j].readGene( k ) << " ";
		cout << endl;
		}*/
	    }
	  }
	  
	  // reduce the temperature
	  repFreezer(t);
	}
      };

  private:
    EOEvalFunc<EOT>& repEvalFunc;
    MonOp<EOT>& repOp;
    unsigned repNumChanges;
    float repInitTemp;
    float repMinTemp;
    EOSimAnnFreezer& repFreezer;
    bool repVerbose;
};

#endif
