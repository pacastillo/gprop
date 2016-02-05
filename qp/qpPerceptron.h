#ifndef qp_PERCEPTRON_H
#define qp_PERCEPTRON_H

//-----------------------------------------------------------------------------

#include <qpMLFF.h>


/**
 *  class qpPerceptron
 */
template<class OutClass>
class qpPerceptron: public qpMLFF<OutClass>
{
 public:
  
  typedef qpTS<exampleType,OutClass> TrainingSet;
  
  /**
   * ctor for N layer qpPerceptron
   *   _numN is a vector of sizes-of-layers : size_input_layer , size_1hidden_layer , size_2hidden_layer , ...
   */
  qpPerceptron( vector<unsigned> _numN, double _iWr ):qpMLFF<OutClass>(_numN,_iWr) {};
    
  /**
   * ctor for 3 layer qpPerceptron
   */
  qpPerceptron( unsigned _ni, unsigned _nh, unsigned _no, double _iWr):qpMLFF<OutClass>(_ni,_nh,_no,_iWr) {};

  ///copy ctor
  qpPerceptron( const qpPerceptron& _p): qpMLFF<OutClass>(_p) {
    repEpochs = _p.repEpochs;
    targetError = _p.targetError;
    tolerance = _p.tolerance;
    learning = _p.learning;
    repError = _p.repError;
    momentum = _p.momentum;
    lambda = _p.lambda;
    verbose = _p.verbose;
  };
  
  ///dtor
  virtual ~qpPerceptron(void) {};

  ///friend method to print the qpPerceptron
  friend ostream& operator<< ( ostream& os, qpPerceptron& _p ){
    os << " this prints the PPC "<< endl;
    return os;
  };


  /**
   * copy operator
   */  
  const qpPerceptron& operator= ( const qpPerceptron& _p ) {
    if( this != &_p ){
      qpMLFF<OutClass>::operator=(_p);
      repEpochs = _p.repEpochs;
      targetError = _p.targetError;
      tolerance = _p.tolerance;
      learning = _p.learning;
      repError = _p.repError;
      momentum = _p.momentum;
      lambda = _p.lambda;
      verbose = _p.verbose;
    }
    return (*this);
  };
  
  

  /// methods for Training and Test  
  
  /**
   *  Sets epochs, targetError, tolerance and learning const.
   */
  void initTrain( unsigned _epochs=100, double _targetError=0.0, double _tolerance=0.0000005, double _learning=0.25 , bool _verbose=false ) {
    repEpochs = _epochs;
    targetError = _targetError;
    tolerance = _tolerance;
    learning = _learning;
    verbose = _verbose;
    
    momentum = alpha_default;
    lambda = lambda_default;
    repError = max_double;
  } 

  void setEpochs( const unsigned _epochs ) {
    repEpochs = _epochs;
  }
  /**
   * trains the qpPerceptron whith a training set
   */
  virtual void train( const qpPerceptron<OutClass>::TrainingSet& _set ) {
    unsigned numEj = 0;
    double last_error = MAXDOUBLE;
    
    numEj = _set.size();

    while( (repEpochs>0)&&(last_error>targetError) ) {
      //init somethings
      initDlt();
      repError=0.0;
      
      for( unsigned i=0 ;  i<numEj ; i++ ) {
	forward( _set.exampleAt(i), _set.targetAt(i) );
	repError += backward( _set.exampleAt(i), _set.targetAt(i) );
      } 

      //update things...
      repError /= numEj;

      if( repError <= last_error + tolerance ){
	coeffAdapt();
	dWeightCalc( numEj , true );
	last_error = repError;
      } else {
	learning *= backtrack_step;
	learning = ( learning < learning_floor ? learning_floor : learning );
	momentum = learning * lambda;
	dWeightCalc( numEj , false );
      }
      
      //display some information...
      if( verbose ) {
	cout.precision(10); cout << "E:"<<repEpochs<<"\terror:"<<repError<<"\tl:"<<learning<<endl;
      }
      repEpochs--;
    }    
  }

  
  /**
   *  forward step in training the qpPerceptron
   */
  void forward( const exampleType &_ej, const OutClass &_label ) {
    unsigned j,k;
    qpFActivation fActiv;
    PNeuron *n;
    
    //first hidden layer
    for (j=0; j<this->sizeLayer(1); j++) {
      n = this->getNeuron(j,1);
      double net = n->Bias + n->dBias1;
      for (k=0; k< n->dim; k++)
	net += (n->Wgt[k] + n->dWgt1[k]) * _ej[k];
      
      n->out = fActiv.sigmoide(net);
    }
    
    //rest of layers
    for(unsigned l=2; l<this->numLayers() ; l++) {
      for (j=0; j<this->sizeLayer(l); j++) {
	n = this->getNeuron(j,l);
	double net = n->Bias + n->dBias1;
	for (k=0; k<n->dim; k++){
	  PNeuron *tmpN = this->getNeuron(k,l-1);
	  net += (n->Wgt[k] + n->dWgt1[k]) * tmpN->out;
	}
	n->out = fActiv.sigmoide(net);
      }
    }
    
    /*    
    //get the output
    unsigned ganadora=0;
    double maxim = -(max_double);
    for ( j=0; j<sizeLayer(numLayers()-1); j++) {
      n = getNeuron(j,numLayers()-1);
      double output = n->out;
      if( output > maxim ) {
	maxim = output;
	ganadora = j; 
      }
    }
    //return the output obtained
    return (OutClass)ganadora;
    */
  }
  

  /**
   *  backward step in training the qpPerceptron
   */
  double backward( const exampleType &_ej, const OutClass &_label ) {
    double bError, diff, sum;
    unsigned j, k;
    PNeuron *n, *nf;
    
    bError = 0.0;
    
    const unsigned sizeLayerOut = this->sizeLayer( this->numLayers()-1 );
    vector<double> sal(sizeLayerOut);
    fill( sal.begin(), sal.end(), 0.0 );
    
    // output layer
    for (j=0; j<sizeLayerOut; j++) {
      n = this->getNeuron(j, this->numLayers()-1 );
      diff = sal[j] - n->out;
      n->dlt = diff * n->out * (1.0 - n->out);
      n->nDlt += n->dlt;
      for (k=0; k<n->dim; k++){
	PNeuron *tmpN = this->getNeuron(k, this->numLayers()-2 );
	n->DxO[k] += n->dlt * tmpN->out;
      }
      bError += diff * diff;
      //cout << "bError:"<<bError<<"   ("<<sal[j]<<" , "<<n->out<<")"<<endl;
    }
    
    //if there are at least one hidden layer
    if( this->numLayers() > 2 ) {
      for(unsigned l=this->numLayers()-2; l>=1; l--) {
	
	for (j=0; j<this->sizeLayer(l); j++){
	  n = this->getNeuron(j,l);
	  sum = 0.0;
	  for (k=0; k<this->sizeLayer(l+1); k++){
	    nf = this->getNeuron(k,l+1);
	    sum += nf->dlt * (nf->Wgt[j] + nf->dWgt1[j]);
	  }
	  n->dlt = n->out * (1.0 - n->out) * sum;
	  n->nDlt += n->dlt;
	  for (k=0; k<n->dim; k++){
	    if( l == 1 ){ //last hidden layer => use example
	      n->DxO[k] += n->dlt * _ej[k];
	    }else{ //other hidden layers => use previous layer neuron outputs
	      PNeuron *tmpN = this->getNeuron(k, l-1 );
	      n->DxO[k] += n->dlt * tmpN->out;
	    }
	  }
	}
	
      }
    }
    
    return bError;
  }


  /**
   *  initialize delta values for all neurons
   */
  void initDlt(void) {
    unsigned l, i,j;
    
    for(l=1; l<this->numLayers(); l++){
      
      for (i=0; i<this->sizeLayer(l); i++) {
	PNeuron *n = this->getNeuron(i,l);
	n->nDlt = 0.0;
	for (j=0; j<n->dim; j++)
	  n->DxO[j] = 0.0;
      }
      
    }
  }


  /**
   *  update coefs: learning, momentum, lambda
   */
  void coeffAdapt(void) {
    unsigned j,k;
    double me, mw, ew, costh;
    
    me = mw = ew = 0.0;
    
    for(unsigned l=this->numLayers()-1; l>=1; l--){
      
      for (j=0; j<this->sizeLayer(l); j++) {
	PNeuron *n = this->getNeuron(j,l);
	for (k=0; k<n->dim; k++) {
	  me += n->DxO[k] * n->DxO[k];
	  mw += n->dWgt1[k] * n->dWgt1[k];
	  ew += n->DxO[k] * n->dWgt1[k];
	}
      }
      
    }
    
    me = ( sqrt(me) < me_floor ? me_floor : sqrt(me) );
    mw = ( sqrt(mw) < mw_floor ? mw_floor : sqrt(mw) );
    
    costh = ew / (me * mw);
    
    learning *= (1.0 + 0.5 * costh);
    learning = ( learning < learning_floor ? learning_floor : learning );
    lambda = lambda0 * me / mw;
    momentum = learning * lambda;
  }
  
  /**
   *  compute  dWgt
   */
  void dWeightCalc( unsigned _numEj, bool _fire ) {
    unsigned j,k;
    
    for(unsigned l=this->numLayers()-1; l>=1; l--) {
      
      for (j=0; j<this->sizeLayer(l); j++) {
	PNeuron *n = this->getNeuron(j,l);
	n->nDlt /= _numEj;
	for (k=0; k<n->dim; k++) {
	  n->DxO[k] /= _numEj;
	  if (_fire){
	    n->Wgt[k] += n->dWgt1[k];
	    n->dWgt2[k] = n->dWgt1[k];
	  }
	  n->dWgt1[k] = learning * n->DxO[k] + momentum * n->dWgt2[k];
	}
	
	if (_fire){
	  n->Bias += n->dBias1;
	  n->dBias2 = n->dBias1;
	}
	n->dBias1 = learning * n->nDlt + momentum * n->dBias2;
      }
      
    }
  }


  //-----------------------------------------------------------------------------
  // tester/classifier for the qpPerceptron
  //-----------------------------------------------------------------------------
  
  /**
   *  receives the TrainingSet and returns the classify error
   */
  double classify( const qpPerceptron<OutClass>::TrainingSet& _set ) {
    unsigned numEj = 0;
    
    numEj = _set.size();
    repError=0.0;
    
    for( unsigned i=0 ;  i<numEj ; i++ ) {
      forward( _set.exampleAt(i), _set.targetAt(i) ); //###ponerle lo del try
      
      const unsigned sizeLayerOut = sizeLayer( this->numLayers()-1 );
      vector<double> sal(sizeLayerOut);
      fill( sal.begin(), sal.end(), 0.0 ); //initialize all either to [-1,1] or [0,1]
      sal[ _set.targetAt(i) ] = 1.0;
      
      // output layer
      double diff;
      for (unsigned j=0; j<sizeLayerOut; j++) {
	PNeuron *n = getNeuron(j, this->numLayers()-1 );
	diff = sal[j] - n->out;
	repError += diff * diff;
      }
      
      //cout << "\t(tst) E: "<<repError << endl;
    }
    
    repError /= numEj;
    
    return (double)repError;
  }
  

  void setLearning(double _l) {learning = _l;};
  double getLearning(void) {return learning;};

  void epochs(unsigned _epochs) {repEpochs = _epochs;};
  unsigned epochs(void) {return repEpochs;};
  double error(void) {return repError;};


  /**
   *  classify one example. 
   *  returns the label obtained.
   */
  virtual OutClass apply( const exampleType &_ej ) const {
    qpFActivation fActiv;
    
    unsigned j,k;
    PNeuron *n;
    vector<double> v1;
    vector<double> v2;
    
    //forward the [_ej,0]; first the hidden layer
    for (j=0; j<this->sizeLayer(1); j++) {
      n = this->getNeuron(j,1);
      double net = n->Bias + n->dBias1;
      for (k=0; k< n->dim; k++)
	net += (n->Wgt[k] + n->dWgt1[k]) * _ej[k];
      v1.push_back( fActiv.sigmoide(net) );
    }
    
    //rest of layers
    for(unsigned l=2; l<this->numLayers(); l++) {
      
      for (j=0; j<this->sizeLayer(l); j++) {
	n = this->getNeuron(j,l);
	double net = n->Bias + n->dBias1;
	for (k=0; k<n->dim; k++){
	  net += (n->Wgt[k] + n->dWgt1[k]) * v1[k];
	}
	v2.push_back( fActiv.sigmoide(net) );
	//v1 = v2;
      }
      v1 = v2;
    }
    
    //get the output
    unsigned ganadora=0;
    double maxim = -(max_double);
    
    for ( j=0; j<this->sizeLayer(this->numLayers()-1); j++) {  //dim output layer
      if( v1[j] > maxim ) {
	maxim = v1[j];
	ganadora = j; 
      }
    }
    
    //return the output obtained
    return (OutClass)ganadora;
  }


  virtual vector<double> vector_salidas( const exampleType &_ej ) const {
    qpFActivation fActiv;
    
    unsigned j,k;
    PNeuron *n;
    vector<double> v1;
    vector<double> v2;
    
    //forward the [_ej,0]; first the hidden layer
    for (j=0; j<this->sizeLayer(1); j++) {
      n = this->getNeuron(j,1);
      double net = n->Bias + n->dBias1;
      for (k=0; k< n->dim; k++)
	net += (n->Wgt[k] + n->dWgt1[k]) * _ej[k];
      v1.push_back( fActiv.sigmoide(net) );
    }
    
    //rest of layers
    for(unsigned l=2; l<this->numLayers(); l++) {
      
      for (j=0; j<this->sizeLayer(l); j++) {
	n = this->getNeuron(j,l);
	double net = n->Bias + n->dBias1;
	for (k=0; k<n->dim; k++){
	  net += (n->Wgt[k] + n->dWgt1[k]) * v1[k];
	}
	v2.push_back( fActiv.sigmoide(net) );
	//v1 = v2;
      }
      v1 = v2;
    }
    
    //return the output-vector obtained
    return (vector<double>)v1;
  }

  /******************************************** LAS VARIABLES DE CLASE ***************************************************************/

 private:
  //data structs for Training and Test
  unsigned repEpochs;    // contador de epocas
  double targetError;     // error que deseamos alcanzar
  double tolerance;       // tolerancia de la red
  double learning;        // coeficiente de aprendizaje
  double repError;           // error de la red
  double momentum;        // momento
  double lambda;          // necesario hacer evolucionar el momento
  bool verbose;
};


//-----------------------------------------------------------------------------
#endif
