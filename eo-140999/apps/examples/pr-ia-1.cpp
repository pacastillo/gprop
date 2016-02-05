/** @name Testing EO's chromosomes and operators
\begin{center} Program to test EO features \end{center}

pr-ia-1 stands for pr (prueba) - ia  for Integer Array, and 1 for 1. This is a testbed for
basic genetic operators on an integer-array-turned-EO.
@see EO
@see EOOp
*/

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <string>
#include <iostream>
#include <iterator>

using namespace std;

#include <generic/EOGOps.h>
#include <generic/EOVector.h>
#include <specific/EOSIntOps.h>

#include <util/Uniform.h>

main() {
	const unsigned size = 10;
	const int range = 100;
	unsigned i;
	typedef EOVector<int,float> eoIntFloatT;
	
	// Define initial and print
	eoIntFloatT EOIA, EOIA2;
	Uniform<int> rnd( -range/2, range/2 ); 
	for ( i = 0; i < size; i ++ ) {
	  EOIA.push_back( rnd() );
	  EOIA2.push_back( -EOIA[i] );
	}
	cout << "EOIA " << EOIA << endl << "EOIA2 " << EOIA2 << endl;

	// apply mutation
	EOIntArrayMutate<eoIntFloatT> mutator;
	eoIntFloatT ia1 = EOIA;
	mutator( ia1 );
	cout << "ia1 " << ia1 << endl;

	// apply transposer
	EOTranspose<eoIntFloatT> transposer;
	eoIntFloatT ia2 = EOIA;
	transposer( ia2 );
	cout << "ia2 " << ia2 << endl;
	
	EOOp<eoIntFloatT>& opRef = transposer, opRef2;
	MonOp<eoIntFloatT>& mopRef = dynamic_cast< MonOp<eoIntFloatT>& > ( opRef );
	mopRef( ia2 );
	cout << "ia2 again " << ia2 << endl;

	// apply xOver
	EOXOver2<eoIntFloatT> xOver; 
	eoIntFloatT ia3 = EOIA;
	xOver( ia3, EOIA2 );
	cout << "ia3 , EOIA " << endl << ia3 << endl << EOIA2 << endl;

	// Multiple xOver
	ScanXOver<eoIntFloatT> sxOver;
	EOPop< eoIntFloatT> thisPop;
	thisPop.push_back( ia3);
	thisPop.push_back( EOIA);
	thisPop.push_back( EOIA2);
	eoIntFloatT ia4 = EOIA;
	thisPop.push_back( ia4 );

	sxOver( thisPop );
	cout << "ia4, ia3, EOIA, EOIA2" << endl << ia4 << endl << ia3 << endl 
	     << EOIA << endl << EOIA2 << endl;

	// Assignment operator
	EOIA = EOIA2;
	cout << "Operator = EOIA, EOIA2 " << endl << EOIA << endl << EOIA2 << endl;

	// Test exceptions
	try {
	  cout << EOIA.readGene( size + 1 );
	} catch ( exception& _e ) {
	  cout << _e.what() << endl;
	}
	return 0;
}
	
