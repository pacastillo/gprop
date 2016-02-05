#ifndef qp_MLFF_H
#define qp_MLFF_H

//-----------------------------------------------------------------------------

#include <math.h>

#ifndef _WIN32
//#include <values.h>  // en XCode no compila con ese include...
#define  MAXDOUBLE  (double)1.79769313486231570e+308
#define  MAXINT     (unsigned)2147483647

#else
#define  MAXDOUBLE  (double)1.79769313486231570e+308
#define  MAXINT     (unsigned)2147483647
#endif

// STL includes
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
//#include <util/Random.h>
#include <random>

using namespace std;

// qp includes
#include <qp.h>
#include <qpVectorTS.h>
#include <qpUniform.h>
#include <Exception.h>


//type of Weights and Labels for examples
//typedef double   wgtType;
typedef unsigned labelType;
typedef qpVector exampleType;

//some usefull constants
const double  learning_default =  0.25;
const double  alpha_default    =  0.9;
const double  lambda_default   =  0.5;
const double  lambda0          =  0.1;
const double  backtrack_step   =  0.5;
const double  me_floor         =  0.0001;
const double  mw_floor         =  0.0001;
const double  learning_floor   =  0.0001;
const double  max_double      =  MAXDOUBLE;


//-----------------------------------------------------------------------------
// Class NActivation
//-----------------------------------------------------------------------------

class qpFActivation {
 public:     
  //ctor
  qpFActivation(){};

  /**
   * sigmoide
   */
  double sigmoide(double x) { 
    return (double)( 1.0 / ( 1.0 + exp(-(x))) ); 
  };
  

};



//-----------------------------------------------------------------------------


/// This class is used for each neuron in the MLP
struct PNeuron {
	
	/// Ctor: defaults to null ctor, because VC++ needs it
	PNeuron(unsigned _ne = 0, double _iWr = 0.05): dim(_ne),iWr(_iWr){ 
		//create the weight vectors
		for(unsigned i=0; i<dim; i++) {
			Wgt.push_back(0.0);
			dWgt1.push_back(0.0);
			dWgt2.push_back(0.0);
			DxO.push_back(0.0);
		}
		reset(); 
	};
    
	/// Copy ctor
	PNeuron(const PNeuron& _pn) {
		out = _pn.out;
		dlt = _pn.dlt;
		nDlt = _pn.nDlt;
		Bias = _pn.Bias;
		dBias1 = _pn.dBias1;
		dBias2 = _pn.dBias2;
		Wgt   = _pn.Wgt;
		dWgt1 = _pn.dWgt1;
		dWgt2 = _pn.dWgt2;
		DxO   = _pn.DxO;
		dim = _pn.dim;
		iWr = _pn.iWr;
	};
	
	/// Assignment operator
	const PNeuron& operator = (const PNeuron& _pn) {
		if ( &_pn != this ) {
			out = _pn.out;
			dlt = _pn.dlt;
			nDlt = _pn.nDlt;
			Bias = _pn.Bias;
			dBias1 = _pn.dBias1;
			dBias2 = _pn.dBias2;
			Wgt   = _pn.Wgt;
			dWgt1 = _pn.dWgt1;
			dWgt2 = _pn.dWgt2;
			DxO   = _pn.DxO;
			dim = _pn.dim;
			iWr = _pn.iWr;
		}
		return *this;
	};
    
	// dtor
	~PNeuron() {};
	
  ///initialize all fields
  void reset(void){
    out = dlt = nDlt = dBias1 = dBias2 = 0.0;

    double lower = (-1)*iWr;
    double higher= iWr;
    double diff  = higher - lower;

    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<double> dis(0, 1);
    double TMPRANDOM1 = dis(gen);

    Bias = lower + (diff * TMPRANDOM1);
    for (unsigned i=0; i<dim; i++) {
      double TMPRANDOM2 = dis(gen);
      Wgt[i] = lower + (diff * TMPRANDOM2);
      dWgt1[i] = dWgt2[i] = DxO[i] = 0.0;
    }

    /*
    Bias = rand()*1.0 / RAND_MAX;
    for (unsigned i=0; i<dim; i++) {
      Wgt[i] = rand()*10.0 / (double)RAND_MAX - 5;
      dWgt1[i] = dWgt2[i] = DxO[i] = 0.0;
    }
    */
  };
  
  
  double out;
  double dlt;
  double nDlt;
  double Bias;
  double dBias1;
  double dBias2;
  vector<double> Wgt;
  vector<double> dWgt1;
  vector<double> dWgt2;
  vector<double> DxO;
  unsigned dim;
  double iWr;
  
};


/**
 *  class qpMLFF
 */
template<class OutClass=labelType>
class qpMLFF: qp<exampleType, OutClass>
{
 public:
 //typedef qpTS<exampleType,OutClass> TrainingSet;    

  /**
   * ctor for N layer qpPerceptron
   * @param  _numN is a vector of sizes-of-layers : size_input_layer , size_1hidden_layer , size_2hidden_layer , ...
   */
  qpMLFF( vector<unsigned> _numN, double _iWr ) {
    //initialize layer-sizes
    numN = _numN;

    vector<PNeuron> ilayer; //empty layer, only to make easy the access to the vector of layers
    //add the empty input layer
    layers.push_back(ilayer);

    unsigned i,j;
    //create hidden layers and output layers
    for(j=1 ; j< _numN.size() ; j++) {
		vector<PNeuron> newlayer;

      //create the neurona vectors
      for(i=0; i<numN[j]; i++) {
		PNeuron n(numN[j-1], _iWr);
		newlayer.push_back(n);
      }

      layers.push_back(newlayer);
    }
  };

    
  /**
   * ctor for 4 layer (I,H1,H2,O) qpMLFF
   */
  qpMLFF( unsigned _ni, unsigned _nh1, unsigned _nh2, unsigned _no, double _iWr) {

  }
    
  /**
   * ctor for 3 layer qpMLFF
   */
  qpMLFF( unsigned _ni, unsigned _nh, unsigned _no, double _iWr) {
    //initialize layer-sizes
    numN.push_back(_ni);
    numN.push_back(_nh);
    numN.push_back(_no);

    unsigned i;
    vector<PNeuron> ilayer; //empty layer, only to make easy the access
    vector<PNeuron> hlayer;
    vector<PNeuron> olayer;
    //create the neurona vectors
    for(i=0; i<_nh; i++) {
      PNeuron n(_ni, _iWr);
      hlayer.push_back(n);
    }
    for(i=0; i<_no; i++) {
      PNeuron n(_nh, _iWr);
      olayer.push_back(n);
    }    

    //addthe layers:
    layers.push_back(ilayer);
    layers.push_back(hlayer);
    layers.push_back(olayer);
  };
  
  //copy ctor
  qpMLFF( const qpMLFF& _p):
    layers(_p.layers), numN(_p.numN)  
    {}


  /**
   * dtor
   */  
  virtual ~qpMLFF(void) {};

  //methods to manage the qpPerceptron

  /// assignment operator
  const qpMLFF& operator= ( const qpMLFF& _p ) {
    if( this != &_p ){
      numN = _p.numN;
      layers = _p.layers;
    }
    return (*this);
  }
  
  /**
   * return the number of neurons per layer
   * _layer: 0 == input ; 1 == first hidden layer ; 2 == second hidden layer ; ....
   */
  unsigned sizeLayer(unsigned _layer) const{
    return (unsigned)( numN[_layer] );
  }

  
  /**
   * return the number of layers
   *   if 3-layer-Perc => returns 2 (1 hidden  and  1 output)
   */
  unsigned numLayers(void) const {
    return (unsigned)( layers.size() );
  }


  
  /**
   * returns the _i-th neuron error
   * _layer: 0 == input ; 1 == first hidden layer ; 2 == second hidden layer ; ....
   */  
  double getNeuronError(unsigned _i, unsigned _layer) const {
    return (unsigned)( layers[_layer][_i].dlt );
  }


  /**
   * returns the _neuron-th neuron of _layer-th layer
   * _layer: 0 == input ; 1 == first hidden layer ; 2 == second hidden layer ; ....
   */  
  PNeuron *getNeuron(unsigned _neuron, unsigned _layer) const {
    return (PNeuron*)( &(layers[_layer][_neuron]) );
  }


  /**
   * adds a new neuron to hidden layer (at the end).
   * _layer: 0 == input ; 1 == first hidden layer ; 2 == second hidden layer ; ....
   */  
  void addNeuron(unsigned _layer) {
    //#*# excepcion si (_layer <= 0)
    PNeuron neuron( numN[_layer-1] ); 
    layers[_layer].push_back(neuron);
    numN[_layer] ++ ;
    
    //add wgts to the next layer (if it is not the output layer)
    if( _layer < numLayers()-1 ){
      for(unsigned i=0; i<numN[_layer+1]; i++){
	PNeuron *n = &(layers[_layer+1][i]);
	n->dim++;
	n->Wgt.push_back((double)rand() / (double)MAXINT / 5000.0);
	n->dWgt1.push_back(0.0);
	n->dWgt2.push_back(0.0);
	n->DxO.push_back(0.0);
      }
    }
  }
  

  /**
   * delete the _i-th neuron of hidden layer
   * _layer: 0 == input ; 1 == first hidden layer ; 2 == second hidden layer ; ....
   */  
  void deleteNeuron(unsigned _i, unsigned _layer) {
    vector<PNeuron>::iterator i = layers[_layer].begin()+_i;
    layers[_layer].erase( i );
    numN[_layer] --;
    //delete wgts of next layer
    if( _layer < numLayers()-1 ){
      for(unsigned i=0; i<numN[_layer+1]; i++){
	PNeuron *n = &(layers[_layer+1][i]);
	n->dim--;
	vector<double>::iterator i_wgt = n->Wgt.begin()+_i;   n->Wgt.erase(i_wgt);
	vector<double>::iterator i_wgt1 = n->dWgt1.begin()+_i; n->dWgt1.erase(i_wgt1);
	vector<double>::iterator i_wgt2 = n->dWgt2.begin()+_i; n->dWgt2.erase(i_wgt2);
	vector<double>::iterator i_dxo = n->DxO.begin()+_i;   n->DxO.erase(i_dxo);
      }
    }
  }

 private:
  //data structs for qpPerceptrons
  vector< vector<PNeuron> > layers;
  vector<unsigned> numN;
};


//-----------------------------------------------------------------------------
#endif
