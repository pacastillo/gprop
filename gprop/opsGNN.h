#ifndef GNNops_H
#define GNNops_H

/*
   TODO:
      - un op para SimAnn que lo que haga sea crear un op de mut. o add. para poder aplicar varios ops

      - un op Xover que haga el promedio de los pesos entre redes
      - un op Xover que haga el cruce de redes de 2 capas: la primera tal cual, meter las ocultas de la segunda, Peh de la segunda, Phh a 0, Pho normalizados junto con los de la primera.

      - un op de Mutacion para redes de 2 capas
      - el op de train esta bien.
      - no usar el de sustitucion (en principio)
 */


/** @name genetic operators for MLPs
 * crossover of two chromosomes (each gene is a hidden neuron)
 * mutation of a gene (a hidden neuron)
 * addition of 1 gene
 * elimination of 1 gene
 * substitution of 1 gene
*/
//@{

#include <ADT/EOOp.h>
#include <util/Uniform.h>

//-----------------------------------------------------------------------------
//  GNNCrossover
//-----------------------------------------------------------------------------

template <class EOT, class geneType>
class GNNCrossover: public BinOp<EOT> {
 public:
  //        
  GNNCrossover( unsigned _num_points = 2, float _priority = 1 ):
    BinOp< EOT >( _priority ), num_points(_num_points) {};
  
  //
  virtual ~GNNCrossover() {};
  
  //
  virtual void operator()( EOT& _eo1, EOT& _eo2 ) const {
    unsigned max_genes = min(_eo1.length(), _eo2.length());
    unsigned cut_genes = min(max_genes, num_points);

    vector<bool> points(max_genes, false);
    Uniform<unsigned> uniform(0, max_genes);
    
    // selects genes to swap
    do {
      unsigned bit = uniform();
      if (points[bit])
        continue;
      else
        {
          points[bit] = true;
          cut_genes--;
        }
    } while (cut_genes);
    
    for (unsigned i = 0; i < points.size(); i++)
      if (points[i]){
	//swap two genes between EOs
	geneType tmp = _eo1.readGene(i);
	_eo1.writeGene(i, _eo2.readGene(i));
	_eo2.writeGene(i, tmp);
      }


    //ya ha inter-cambiado los pesos de la red... ahora a inter-cambiar el param de aprendizaje:
    double oldLearning1 = _eo1.getLearning();
    double oldLearning2 = _eo2.getLearning();
    _eo1.setLearning( oldLearning2 );
    _eo2.setLearning( oldLearning1 );

    //string op("("+_eo1.ID()+")x("+_eo2.ID()+")");
    //_eo1.ID( op );
    //_eo2.ID( op );
    //cout << "C("<<_eo1.ID()<<","<<_eo2.ID()<<")-";
    //cout << "C";
  };
  
 private:  
  /// applies operator to one gene in the EO
  virtual void applyAt( EOT&, EOT&, unsigned, unsigned) const {};
  
  unsigned num_points; // number of cut points
};



//-----------------------------------------------------------------------------
//  GNNMutation
//-----------------------------------------------------------------------------

/** Every gen is mutated adding or substracting an Uniform [-0.1,0.1]
*/
template< class EOT, class geneType > 
class GNNMutation: public MonOp< EOT > {
 public:
  //
  GNNMutation( float _rate, float _weightMut, float _learningMut, float _priority = 1 ): 
    MonOp< EOT > ( _priority ), rate( _rate ), weightMut( _weightMut ), learningMut( _learningMut ) {};
  
  virtual ~GNNMutation() {};

  //
  virtual void operator()( EOT& _chrom ) const {
    Uniform<float> rnd(0, 1);

    /*TODO: doble bucle FOR para recorrer las neuronas de ambas capas; poniendolo todo en el mismo bucle, sin llamar al applyAt...*/
    for (unsigned g = 0 ; g < _chrom.length() ; g++){
      if (rnd() < rate){
        //mutate the g-th neuron
        applyAt( _chrom, g );
      }
    }

    //ya ha cambiado los pesos de la red... ahora a ver si cambia el param de aprendizaje:
    if (rnd() < rate){
      //mutate the learning-rate
      double oldLearning = _chrom.getLearning();

      Uniform<float> mut( (-1)*learningMut , learningMut);
      float tmp_mut = mut() ;
      float tmp_l = oldLearning + tmp_mut ;

      if( tmp_l <= 0  ||  tmp_l > 1.0 ) {
	tmp_l = oldLearning - tmp_mut ;
      }

      //cout << "cambia  "<<_chrom.ID()<<"  learning en mut: "<<tmp_l<<" \n";
      _chrom.setLearning( tmp_l );
    }

    //string op("["+_chrom.ID()+"]");
    //_chrom.ID( op );
    //cout << "M["<<_chrom.ID()<<"]-";
    //cout << "M";
  };         
  
 private:
  
  //
  void applyAt( EOT& _chrom, unsigned _g, unsigned = 0 ) const { 
    Uniform<float> u( (-1)*weightMut , weightMut );
    geneType tmp = _chrom.readGene(_g) ;
    for(unsigned i=0; i< tmp.size(); i++){
      tmp[i] += u();
    }
    _chrom.writeGene( _g, tmp );
  };
  
  float rate; //rate of mutation
  float weightMut;
  float learningMut;
};



//-----------------------------------------------------------------------------
//  GNNAdition
//-----------------------------------------------------------------------------

/** adds 1 gene to the chrom
*/
template< class EOT, class geneType > 
class GNNAdition: public MonOp< EOT > {
 public:
  //
  GNNAdition( float _priority = 1 ): 
    MonOp< EOT > ( _priority ) {};
  
  virtual ~GNNAdition(){};

  //
  virtual void operator()( EOT& _chrom ) const {
    unsigned i=0;
    geneType gen;

    /*TODO: decidir en que capa se introduce la nueva neurona*/
    _chrom.insertGene(i, gen);

    //string op("{"+_chrom.ID()+"}");
    //_chrom.ID( op );
    //cout << "A{"<<_chrom.ID()<<"}-";
    //cout << "A";
  };
  
 private:
  //
  virtual void applyAt( EOT&, unsigned, unsigned) const {};
};



//-----------------------------------------------------------------------------
//  GNNSubstitution
//-----------------------------------------------------------------------------

/** substitution of 1 gene from chrom
*/
template< class EOT, class geneType > 
class GNNSubstitution: public MonOp< EOT > {
 public:
  //
  GNNSubstitution( float _priority = 1 ): 
    MonOp< EOT > ( _priority ) {};
  
  virtual ~GNNSubstitution(){};
  //
  virtual void operator()( EOT& _chrom ) const {
    if ( _chrom.length() == 1 ) {
      return;
    }
    unsigned which = _chrom.worseGene();

    Uniform<float> u(-0.1, 0.1);
    geneType tmp = _chrom.readGene(which) ;
    for(unsigned i=0; i< tmp.size(); i++){
      tmp[i] = u();
    }
    _chrom.writeGene( which, tmp );

    //string op("<"+_chrom.ID()+">");
    //_chrom.ID( op );
    //cout << "S<"<<_chrom.ID()<<">-";
  };
  
 private:
  //
  virtual void applyAt( EOT&, unsigned, unsigned) const {};
};


//-----------------------------------------------------------------------------
//  GNNElimination
//-----------------------------------------------------------------------------

/** removes 1 gene (hidden neuron) from chrom
*/
template< class EOT, class geneType > 
class GNNElimination: public MonOp< EOT > {
 public:
  //
  GNNElimination( float _priority = 1 ): 
    MonOp< EOT > ( _priority ) {};
  
  virtual ~GNNElimination(){};
  //
  virtual void operator()( EOT& _chrom ) const {
     if ( _chrom.length() == 1 ) {
      return;
    }
    unsigned which = _chrom.worseGene();

    _chrom.deleteGene( which );

    //string op("/"+_chrom.ID()+"/");
    //_chrom.ID( op );
    //cout << "E/"<<_chrom.ID()<<"/-";
    //cout << "E";
  };         
  
 private:
  //
  virtual void applyAt( EOT&, unsigned, unsigned) const {};
};


//-----------------------------------------------------------------------------
//  GNNTraining
//-----------------------------------------------------------------------------

/** trains the net for a certain number of epochs
*/
template< class EOT, class geneType , class OutClass > 
class GNNTraining: public MonOp< EOT > {
 public:

  typedef qpTS<vector<double>, OutClass> TrainingSet;

  //
  GNNTraining( const TrainingSet &_cbTrn , float _priority = 1 ): MonOp< EOT > ( _priority ) , cbTrn(_cbTrn) {};
  
  virtual ~GNNTraining(){};
  //
  virtual void operator()( EOT& _chrom ) const {
    _chrom.train( cbTrn );
    
    //string op(";"+_chrom.ID()+";");
    //_chrom.ID( op );
    //cout << "T;"<<_chrom.ID()<<";-";
    //cout << "T";
  };         
  
 private:
  //
  virtual void applyAt( EOT&, unsigned, unsigned) const {};
  
  const TrainingSet &cbTrn;
};


//@}
#endif
