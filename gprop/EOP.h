#ifndef EOP_H
#define EOP_H

#include <string>				// for the ID string

#ifdef _MSC_VER
#include <iostream>				// for ostream
#include <ostream>				// for ostream
#else
#include <sstream>			// for ostream
#endif

using namespace std;

#include <ADT/EO.h>
// #pragma warning(disable:4786)
#include <time.h>
#include <iostream>
#include <fstream>
//QP's includes
#include <qpPerceptron.h>

// typedef qpTS<vector<double>, OutClass> TrainingSet;


/** @name EOPFitness
 * class to define a complex fitness
 * @author P.A. Castillo Valdivieso
 */
//@{

class EOPFitness : public EOFitness {
  public:

  EOPFitness( double _criterio1=0.0, double _criterio2=0.0 , double _criterio3=0.0 , double _criterio4=0.0): 
    criterio1(_criterio1), criterio2(_criterio2), criterio3(_criterio3), criterio4(_criterio4) {}

  virtual ~EOPFitness() {};

  //using the default copy ctor.

  // Assignment operator
  const EOPFitness& operator= ( const EOPFitness& _eop ) {
    if ( this != &_eop ) {
      this->criterio1 = _eop.criterio1;
      this->criterio2 = _eop.criterio2;
      this->criterio3 = _eop.criterio3;
      this->criterio4 = _eop.criterio4;
    }
    return *this;
  }

  ///
  virtual bool operator < ( const EOFitness & _f ) const {
    EOPFitness &tmp_f = (EOPFitness &)_f;

    if( this->criterio1 < tmp_f.criterio1 ) {
      return true;
    }
    if( this->criterio1 > tmp_f.criterio1 ) {
      return false;
    }

    if( (this->criterio1 == tmp_f.criterio1) && (this->criterio2 < tmp_f.criterio2) ) {
      return true;
    }
    if( (this->criterio1 == tmp_f.criterio1) && (this->criterio2 > tmp_f.criterio2) ) {
      return false;
    }

    if( (this->criterio1 == tmp_f.criterio1) && (this->criterio2 == tmp_f.criterio2) && (this->criterio3 < tmp_f.criterio3) ) {
      return true;
    }
    if( (this->criterio1 == tmp_f.criterio1) && (this->criterio2 == tmp_f.criterio2) && (this->criterio3 > tmp_f.criterio3) ) {
      return false;
    }

    if( (this->criterio1 == tmp_f.criterio1) && (this->criterio2 == tmp_f.criterio2) && (this->criterio3 == tmp_f.criterio3) && 
	(this->criterio4 > tmp_f.criterio4) ) {
      return true;
    }

    return false;
  }

  ///
  virtual operator double() const {
    return this->criterio1 ;
  }

  double print_criterio1(void) {
    return criterio1;
  }

  double print_criterio2(void) {
    return criterio2;
  }

  double print_criterio3(void) {
    return criterio3;
  }

  double print_criterio4(void) {
    return criterio4;
  }

  friend ostream& operator<<(ostream& os, const EOPFitness& _eop) {
    os <<"c1="<<_eop.criterio1<<"\tc2="<<_eop.criterio2<<"\tc3="<<_eop.criterio3<<"\tc4="<<_eop.criterio4;
    return os;
  }

 private:
  double criterio1;
  double criterio2;
  double criterio3;
  double criterio4;
};
//@}


/** @name EOP class 
 * class to use the qpPerceptron an individual in an Evolutionary Algorithm
 * @author P. A. Castillo Valdivieso
 */
//@{

//type T is a neuron (NI+NO double weights)   ==>   vector<double>
template<class T, class fitnessT , class OutClass>
class EOP : public EO<T,fitnessT>, public qpPerceptron<OutClass> {
public:
 
  //needs the sizes of input,hidden,output layers for qpPerceptron and ID for EO
  EOP( const unsigned _ni, const unsigned _nh, const unsigned _no, const string& _ID = "" , const double& _iWr=0.05)
    :EO<T,fitnessT>(_ID), qpPerceptron<OutClass>(_ni, _nh, _no, _iWr) {};

  // Copy ctor
  EOP( const EOP& _eo )
    :EO<T,fitnessT>(_eo), qpPerceptron<OutClass>(_eo){};

  // Assignment operator
  const EOP& operator= ( const EOP& _eo ) {
    if ( this != &_eo ) {
      EO<T,fitnessT>::operator=(_eo);
      qpPerceptron<OutClass>::operator=(_eo);
    }
    return *this;
  };
  
  virtual ~EOP(){};
  
  //returns the _i-th neuron of HIDDEN-LAYER (a vector of weights)
  T readGene( unsigned _i ) const {
    unsigned i;
    PNeuron *n;
    T val;

    n = this->getNeuron(_i, 1);
    for(i=0; i< n->dim; i++){
      val.push_back(n->Wgt[i]);
    }

    for(i=0; i< this->sizeLayer(2); i++){
      n = this->getNeuron(i, 2);
      val.push_back(n->Wgt[_i]);
    }

    return ( (T)val );
  };

  //writes on the _i-th neuron of HIDDEN-LAYER the _val neuron
  void writeGene( unsigned _i, T _val ) {
    unsigned i,j=0;
    PNeuron *n;

    n = this->getNeuron(_i, 1);
    for(i=0; i< n->dim; i++,j++){
      n->Wgt[i] = _val[j];
    }

    for(i=0; i< this->sizeLayer(2); i++,j++){
      n = this->getNeuron(i, 2);
      n->Wgt[_i] = _val[j];
    }
  };

  //adds a neuron to the HIDDEN-LAYER
  void insertGene( unsigned  , T ) {
    this->addNeuron(1);  //1 == primera capa oculta ; 2 == segunda capa oculta
  };

  //deletes the _i-th neuron of HIDDEN-LAYER
  void deleteGene( unsigned _i ) {
    if( length() > 1 ){
      this->deleteNeuron(_i, 1);  //1 == primera capa oculta ; 2 == segunda capa oculta
    }
  };

  //returns the number of neurons of HIDDEN-LAYER
  unsigned lengthT() const {
    return ( this->numLayers() );
  };
  unsigned length() const {
    return ( this->sizeLayer(1) );
  };
  unsigned length(unsigned _l) const {
    return ( this->sizeLayer(_l) );
  };

  void setLearning(double _l) {
    qpPerceptron<OutClass>::setLearning(_l);
  };
  double getLearning(void) {
    return (double) qpPerceptron<OutClass>::getLearning();
  };

  //return the current fitness
  fitnessT fitness() const {
    return EO<T,fitnessT>::fitness();
  };
  
  //sets the fitness value
  void fitness( const fitnessT &_fitness ) { 
    EO<T,fitnessT>::fitness(_fitness);
  };
  
  const string& ID() const { 
    return ( EO<T,fitnessT>::ID() );
  };
  
  void ID( const string& _id ) { 
    EO<T,fitnessT>::ID(_id);
  };
  
  // returns the number of the worst hidden neuron in the MLP.
  unsigned worseGene(void) {
    // para seleccionarla segun el error acumulado en todo el conjunto de datos
    double worse = -100000.0;
    unsigned w = 0;  
    PNeuron *n;

    for(unsigned i=0; i<length(); i++){
      n = this->getNeuron(i, 1);
      if( worse < n->nDlt ) {
	worse = n->nDlt;
	w = i;
      }
    }
    return w;   
  };
  
  /**
   *  Sets epochs, targetError, tolerance and learning const... 
   */
  void initTraining( unsigned _epochs=100, double _targetError=0.0, double _tolerance=0.0000005, double _learning=0.25 , bool _verbose=false ) {
    qpPerceptron<OutClass>::initTrain( _epochs, _targetError, _tolerance, _learning , _verbose );
    //qpPerceptron::train( _trnSet );
  }
  

  /**
   *  obtains the error achieved by the EOP on the data set or the output on an example
   */
  double testing( const qpTS<vector<double>, OutClass> &_tstSet ) {
    return (double) ( qpPerceptron<OutClass>::classify( _tstSet ) );
  }

  unsigned apply_example( const qpVector &_ex ) {
    return (unsigned) ( qpPerceptron<OutClass>::apply( _ex ) );
  }

  /**
   *  obtains the error achieved by the EOP on the data set or the output on an example  FOR   FUNCT-APROX
   */
  double testingFA( const qpTS<vector<double>, OutClass> &_tstSet ) {
    return (double) ( qpPerceptron<OutClass>::classificationErrorFA( _tstSet ) );
  }

  double apply_exampleFA( const qpVector &_ex ) {
    return (double) ( qpPerceptron<OutClass>::applyFA( _ex ) );
  }

  /**
   *  obtains the error achieved by the EOP while training
   */
  fitnessT error(void) { 
    return ( qpPerceptron<OutClass>::error() ); 
  }
      

  //
  virtual void printSelf( ostream& _s ) {
    _s << length();
  }


  void save( string & fileMLP ) {
    ////////////////////////////////////// escribir la red en un fichero //////////////////////////////////////////////
    // FORMATO: 
    // no. entradas
    // no. ocultas
    // no. salidas
    // fitness
    // varias lineas para los pesos (y bias) de la primera neurona oculta
    // varias lineas para los pesos (y bias) de la segunda neurona oculta
    // .....
    // varias lineas para los pesos (y bias) de la primera neurona de salida
    // varias lineas para los pesos (y bias) de la segunda neurona de salida
    // .....
    //
    ofstream out( fileMLP.c_str() );
    out << length(0) << endl;
    out << length(1) << endl;
    out << length(2) << endl;
    out << (-1) << endl;
    
    for(unsigned h=0; h < length(1) ; h++) {
      PNeuron *n = this->getNeuron(h, 1);
      for(unsigned k=0; k< n->dim; k++) {
	out << n->Wgt[k] << endl;
      }
      out << n->Bias << endl;
    }
    
    for(unsigned o=0; o < length(2) ; o++) {
      PNeuron *n = this->getNeuron(o, 2);
      for(unsigned k=0; k< n->dim; k++) {
	out << n->Wgt[k] << endl;
      }
      out << n->Bias << endl;
    }
    
    out.close();
    ////////////////////////////////////// escribir la red en un fichero //////////////////////////////////////////////
  }

  void load( string & fileMLP ) {
    ////////////////////////////////////// leer la red de un fichero //////////////////////////////////////////////
    // FORMATO: 
    // no. entradas
    // no. ocultas
    // no. salidas
    // fitness
    // varias lineas para los pesos (y bias) de la primera neurona oculta
    // varias lineas para los pesos (y bias) de la segunda neurona oculta
    // .....
    // varias lineas para los pesos (y bias) de la primera neurona de salida
    // varias lineas para los pesos (y bias) de la segunda neurona de salida
    // .....
    //
    unsigned NI,NH,NO;
    double fitness;

    ifstream in( fileMLP.c_str() );
    in >> NI;
    in >> NH;
    in >> NO;
    in >> fitness;
    
    for(unsigned h=0; h < NH ; h++) {
      PNeuron *n = this->getNeuron(h, 1);
      for(unsigned k=0; k< n->dim; k++) {
	in >> n->Wgt[k] ;
      }
      in >> n->Bias ;
    }
    
    for(unsigned o=0; o < NO ; o++) {
      PNeuron *n = this->getNeuron(o, 2);
      for(unsigned k=0; k< n->dim; k++) {
	in >> n->Wgt[k] ;
      }
      in >> n->Bias ;
    }
    
    in.close();
    ////////////////////////////////////// leer la red de un fichero //////////////////////////////////////////////
  }

  
 private:
  /* repID , repFitness , validFitness   [de EO]*/
  /* HLayer , OLayer , ni , nh , no      [de qpPerceptron]*/
  
};


// Standard output
template<class T, class fitnessT, class OutClass>
ostream & operator << ( ostream& _s, EOP<T,fitnessT,OutClass>& _eop ) {
        _eop.printSelf( _s ) ; // User virtual method
        return _s;
};


//@}
#endif
