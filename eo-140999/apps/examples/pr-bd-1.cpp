
/** @name Testing bidimensional chromosomes
\begin{center} Program to test EO Bidimensional Chromosomes \end{center}

pr-bd-1 stands for pr (prueba) - bd  for BiDimensional, and 1 for 1. This is a testbed for
bidimensional chromosomes, and the features that compose it: writing and reading genes.
@see EO
@see EOBidim.h
*/

#include <math.h>  // for fabs

// Disable long names warning
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

// STL includes
#include <string>
#include <iostream>

#include <numeric> // for accumulate
#include <iterator>

using namespace std;

// Standard C libraries for rand
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <ADT/EOBidim.h>
#include <generic/EOEvalAll.h>
#include <EOGeneric.h>

struct Peak: public EOEvalFunc< EOBidim<float,float> > {

  Peak( const vector<vector <float> >& _target )
    : target( _target ){};

  ///
  virtual float  operator() ( EOBidim<float,float> & _eo ) const {
    float dist = 0;
    for ( unsigned i=0; i<_eo.NumRow(); i++ ) {
      for ( unsigned j=0; j<_eo.NumCol(); j++ ) {
	dist+=fabs( _eo.readGene( i, j ) - target[i][j]);
      }
    }
    _eo.fitness( dist );
    return dist;
  };

  const vector<vector< float> >& target;
};

//Utils
//#include <util/Parser.h>

main( int argc, char** argv) {
  // start the algorithm
  srand( (unsigned)time( NULL ) );
  unsigned i,j;
  const unsigned numRow = 3, numCol = 4;

  vector< vector<float> > target(numRow );
  vector< vector<float> >::iterator v;

  for ( v=target.begin(); v!=target.end(); v++ ) {
    v->resize( numCol, 0.0  );
  }

  target[1][2] = 7.5;

  // Creating chromosomes
  EOBidim<float,float> newChr( "NewChr", numRow,numCol);
  cout << "A new chromosome: "<< endl << newChr<< endl;

  EOBidim<float,float> copyChr( newChr );
  cout << "A copy chromosome: "<< endl << copyChr<< endl;
	
  // Modifyin he values
  for ( i=0; i<newChr.NumCol() && i<newChr.NumRow(); i++ ) {
    newChr.writeGene( i,i,1.0 );
  } // for i
  cout << "Changing values [0,0],[1,1]... in NewChr"<< endl << newChr<< endl;
  
  
  // Sum of the values
  float sum=0.0;
  for ( i=0; i<newChr.NumRow(); i++ ) {
    for ( j=0; j<newChr.NumRow(); j++ ) {
      sum+=newChr.readGene(i,j);
    }
  }
  cout << "Sum="<<sum<<endl<< endl;
  
  // Inserting a row
  newChr.insertRow( 2,5.0 );
  cout << newChr<< endl;
  
  // Deleting a row
  newChr.deleteRow( 1 );
  cout << newChr<< endl;
  
  // Testing that copyCrh has not changed
  cout << "The copy chromosome: "<< endl << copyChr;

  // Testing higher-level classes

  // Evaluate
  Peak thisEvalFunc( target );
  cout << "Eval " << thisEvalFunc( copyChr ) << endl;
  EOEvalAll<EOBidim<float,float> > thisEval( thisEvalFunc );
  
  // Go ahead to next generation
  float select = (float)0.3;
  EOSteadyState< EOBidim< float, float > > coach( select );

  // And now breed
  unsigned popSize = 20;
  EORank< EOBidim<float,float> > chaperon( (unsigned) floor( select*popSize ) );

  
  //Termination condition
  unsigned numGenerations = 20;
  EOGenTerm< EOBidim<float,float> > ngenerations( numGenerations );

  // Replacement
  EOElimAll< EOBidim<float,float> > popKiller;	

  //And now the population itself
  EOEasyGA< EOBidim<float,float> > 
    thisAlgorithm( thisEval, coach, chaperon, ngenerations, popKiller, true);

  // Apply the genetic algorithm, and generate evolvedPop
  EOPop< EOBidim<float,float> > pop;
  pop.push_back( newChr );
  pop.push_back( copyChr );
  pop.push_back( copyChr );
  try {
    thisEval( pop );
    thisAlgorithm( pop );
  } catch ( UException& _e  ) {
    cout << "Exception : " << _e.what() << endl;
    exit(1);
  } catch ( exception& _e  ) {
    cout << "Exception : " << _e.what() << endl;
    exit(1);
  }

  return 0;
}
