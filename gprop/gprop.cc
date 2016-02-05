/*
  g++ -O3 -I../eo-140999 -I../qp -Wall -g  gprop.cc -o gprop

  time ./gprop  100   50  0.05   0.5   1 1 1 1 1   0.3 0.005 0.005   500   90 6   g.trn g.tst g.tst   glass.mlp
*/

// Standard C libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// STL includes
#include <iostream>
#include <fstream>
#include <string>

//QP's includes
#include <qpPerceptron.h>

// EO's includes
#include "opsGNN.h"
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOEvalAll.h>
#include <generic/EOEasyGA.h>
#include <generic/EOGenTerm.h>
#include <generic/EOselect/EOReplacers.h>
#include <generic/EOselect/EORank.h>
#include <generic/EOselect/EORandomBreed.h>
#include "EOP.h"
#include <util/Uniform.h>


// useful typedefs
typedef vector<double> geneType;
typedef EOP<geneType, EOPFitness, unsigned> Chrom;
typedef qpTS<vector<double>, unsigned> TrainingSet;


// Standard output
ostream & operator << ( ostream& _s, geneType& ) {
  _s << " ";
  return _s;
};



struct eval: public EOEvalFunc< Chrom > {
  eval(const TrainingSet &_cbTrn , const TrainingSet &_cbVal, const bool _verbose=false ):
    cbTrn(_cbTrn),cbVal(_cbVal), verbose(_verbose)
    {};

  virtual ~eval(){};

  virtual EOPFitness operator() ( Chrom &_eop ) const {
    Chrom auxEOP( _eop );
    unsigned esperado,obtenido,cont=0;

    auxEOP.train( cbTrn );

    cont=0;
    for(unsigned i=0; i< cbVal.size(); i++) {
      esperado =  cbVal.targetAt(i);
      obtenido = auxEOP.apply_example( cbVal.exampleAt(i) );
      if( esperado == obtenido ){ cont++; }
    }
    double max = (double)cont / (double)cbVal.size() ;

    unsigned total_length = _eop.length();
    total_length=0;
    for(unsigned l=1; l< _eop.lengthT(); l++){
      total_length += ( _eop.length(l-1)*_eop.length(l) + _eop.length(l) );
    }

    double criterio1 = (double) max;
    double criterio2 = (double) 0.0;
    double criterio3 = (double) 0.0;
    double criterio4 = (double) total_length;
    EOPFitness f( criterio1, criterio2, criterio3, criterio4 );
    _eop.fitness(f);

    cout <<"."<<flush;
    if(verbose) {
      cout.precision(10);
      cout << "\tfitness:  c1(max)=" << criterio1 <<" ("<<cont<<"/"<<cbVal.size()<< ") \t c4(length)=" << criterio4 << endl;
    }

    return (EOPFitness)f;
  };
  
  const TrainingSet &cbTrn;
  const TrainingSet &cbVal;
  bool verbose;
};



int main(int argc, char **argv) {
  cout << "\n(c)2001 - Pedro Angel Castillo Valdivieso\n" ;
  if( argc < 20 ){
    cout << "SINTAXIS: " << argv[0] << "  "
	 << "numberGenerations  " // fijo = ¿300?
	 << "populationSize initialWeightRange  " // 100,500,1000 ; 0.05,0.1,0.2,0.3,0.4,0.5
	 << "selectionRate  " // 0.1,0.5,0.9
	 << "prioM prioX prioA prioE prioT  " // 0,0.5,1,2,4
	 << "mutationProbability weightMutationRange learningConstantMutationRange  " // 0.1,0.3,0.6,0.9 ; 0.001,0.005,0.01,0.05,0.1 ; 0.001,0.005,0.01,0.05,0.1
	 << "trnEpochs  " // 10,50,100,300
	 << "maxHiddenLayerSize outputLayerSize  " // fijo=90 ; segun_el_problema
	 << "fileTrn fileVal fileTst  " // segun_el_problema
	 << "fileOutMLP  " // segun_el_problema
	 << endl << endl ;

    return 0;
  }

  unsigned numGenerations = (unsigned)atoi(argv[1]);
  unsigned popSize = (unsigned)atoi(argv[2]);
  double initWeightRange = (double)atof(argv[3]);
  double select= (double)atof(argv[4]);

  double prioM=(double)atof(argv[5]);
  double prioX=(double)atof(argv[6]);
  double prioA=(double)atof(argv[7]);
  double prioE=(double)atof(argv[8]);
  double prioT=(double)atof(argv[9]);

  double mutatProb=(double)atof(argv[10]);
  double weightMutatRange=(double)atof(argv[11]);
  double learningMutatRange=(double)atof(argv[12]);

  unsigned epochs=(unsigned)atoi(argv[13]);

  unsigned NH=(unsigned)atoi(argv[14]);
  unsigned NO=(unsigned)atoi(argv[15]);

  string fileTrn=argv[16];
  string fileVal=argv[17];
  string fileTst=argv[18];

  string fileTmpMLP = argv[19];
  
  //create the trn, val and tst sets
  ifstream streamTrn(fileTrn.c_str());
  TrainingSet cbTrn(streamTrn);

  double tolerance=0.0000005;  
  unsigned Xpoint= 2;
  double targetError=0;
  unsigned NI= cbTrn.exampleAt(0).size();

  ifstream streamVal(fileVal.c_str());
  TrainingSet cbVal(streamVal);

  ifstream streamTst(fileTst.c_str());
  TrainingSet cbTst(streamTst);


  //***************************************************//
  
  cout <<"\n<INITIALIZE>";
  cout << "\n\t numberGenerations = " << numGenerations
       << "\n\t populationSize = " << popSize
       << "\n\t initialWeightRange = " << initWeightRange
       << "\n\t selectionRate = " << select
       << "\n\t prioM = " << prioM
       << "\n\t prioX = " << prioX
       << "\n\t prioA = " << prioA
       << "\n\t prioE = " << prioE
       << "\n\t prioT = " << prioT
       << "\n\t mutationProbability = " << mutatProb
       << "\n\t weightMutationRange = " << weightMutatRange
       << "\n\t learningConstantMutationRange = " << learningMutatRange
       << "\n\t trnEpochs = " << epochs
       << "\n\t inputLayerSize = " << NI
       << "\n\t maxHiddenLayerSize = " << NH
       << "\n\t outputLayerSize = " << NO
       << "\n\t fileTrn = " << fileTrn
       << "\n\t fileVal = " << fileVal
       << "\n\t fileTst = " << fileTst
       << "\n\t fileOutMLP = " << fileTmpMLP
       << endl << endl ;
  
  srand( (unsigned)time( NULL ) );
  
  EOPop<Chrom> thisPop;
  Uniform<unsigned> u(2,NH);
  Uniform<double> a(0.05, 0.075);
  for ( unsigned j=0 ; j<popSize ; j++ ) {
    //char cad[10]; sprintf(cad,"%d",j);
    //string id(cad);
    string id("0");
    NH = u();
    double tmp_aprend = a();
    cout << " {"<<NH<<" , "<<tmp_aprend<<"} " << flush;
    Chrom *bicho = new Chrom(NI,NH,NO, id, initWeightRange);
    bicho->initTraining( epochs, targetError, tolerance, tmp_aprend , false );
    thisPop.push_back( *bicho );
  }
  cout <<endl;

  // Evaluate
  eval evalFunc( cbTrn, cbVal, false);
  EOEvalAll<Chrom> thisEval( evalFunc );
  
  // Go ahead to next generation
  EOSteadyState<Chrom> coach( select );

  // Eliminate the worst
  EOElimWorst<Chrom> popKiller;
  
  // And now breed, add operators
  EORandomBreed<Chrom> chaperon;


  //si no va a haber ops.gen. => todos a 0.01, para que se apliquen por igual, ya que no puede ser que no haya ni un operador
  if(prioM==0 && prioX==0 && prioA==0 && prioE==0 && prioT==0) {
    prioM=prioX=prioA=prioE=prioT= 0.01;
  }

  GNNMutation<Chrom, geneType > mutation(mutatProb,weightMutatRange,learningMutatRange,  prioM);
  if(prioM!=0.0) 
    chaperon.addOp( &mutation );
  
  GNNCrossover<Chrom, geneType > xOver(Xpoint,  prioX);
  if(prioX!=0.0) 
    chaperon.addOp( &xOver ); 
  
  GNNAdition<Chrom, geneType > adition(prioA);
  if(prioA!=0.0) 
    chaperon.addOp( &adition ); 
  
  GNNElimination<Chrom, geneType > elim(prioE);
  if(prioE!=0.0) 
    chaperon.addOp( &elim ); 
  
  GNNTraining<Chrom, geneType , unsigned> trainOp(cbTrn,prioT);
  if(prioT!=0.0) 
    chaperon.addOp( &trainOp ); 
  
  //Termination condition
  EOGenTerm<Chrom> terminator( numGenerations );
  
  //create and initialice the algorithm
  EOEasyGA<Chrom> thisAlgorithm( thisEval, coach, chaperon, terminator, popKiller, false );
  cout <<"</INITIALIZE>"<<endl;

//***************************************************//
  cout <<"\n<RUN>"<<endl;
  try{
    thisEval( thisPop );
    thisAlgorithm( thisPop );
  }catch(UException e){
    cout << e.what() << endl;
  }
  cout <<"\n</RUN>"<<endl;
//***************************************************//
  
  cout <<"\n<POP>"<<endl;
  for (unsigned j = 0; j < thisPop.size(); j ++ ) {
    cout << "\tpop:  " << thisPop[j].fitness() << "\tID="<< thisPop[j].ID() << endl;
  }
  cout <<"</POP>\n"<<endl;

//***************************************************//
  double tmp_learning = thisPop[0].getLearning() ;
  thisPop[0].train( cbTrn );
  
  unsigned esperado,obtenido,cont=0;
  for(unsigned i=0; i< cbTst.size(); i++) {
    esperado =  cbTst.targetAt(i);
    obtenido = thisPop[0].apply_example( cbTst.exampleAt(i) );
    //cout << " y= "<<esperado<< "\to= "<<obtenido <<endl;
    if( esperado == obtenido ){ cont++; }
  }

  unsigned total_length=0;
  for(unsigned l=1; l< thisPop[0].lengthT(); l++){
    total_length += ( thisPop[0].length(l-1)*thisPop[0].length(l) + thisPop[0].length(l) );
  }

  cout.precision(10);
  cout << endl;
  cout <<"  E= " << (100-(100*(double)cont/(double)cbTst.size()))
       <<"\tS= " << total_length
       <<"\tID= "<< thisPop[0].ID()
       <<"\tL= " << tmp_learning
       <<"\tverbose="<<cont<<"/"<<cbTst.size()<<"="<<((double)cont/(double)cbTst.size())
       <<"\tfile= "<< fileTmpMLP << endl;

  char tmpcad[10]; sprintf(tmpcad,"%d",0);
  string tmpid(tmpcad);
  tmpid = "z_"+tmpid+"_"+fileTmpMLP;
  thisPop[0].save( tmpid );
//***************************************************//

  return 0;
}
