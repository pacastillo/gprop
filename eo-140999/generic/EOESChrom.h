#ifndef _EOESCHROM_H
#define _EOESCHROM_H

// STL libraries
#include <vector>		// For vector<>
#include <stdexcept>
#include <strstream>
#include <iostream>		// for ostream

// EO includes
#include <generic/EOVector.h>

/** Each gene in an Evolution Strategies is composed of a value plus an standard
deviation, sigma, used for mutation*/
struct EOESGene {
	double val, sigma;
	EOESGene( double _val = 0, double _sigma = 0 ): val( _val ), sigma( _sigma ) {};
};

/// Tricky operator to avoid errors in some VC++ systems
bool operator < ( EOESGene _e1, EOESGene _e2 ) {
	return _e1.val < _e2.val;
}

/// Tricky operator to avoid errors in some VC++ systems
bool operator == ( EOESGene _e1, EOESGene _e2 ) {
	return ( _e1.val == _e2.val ) && ( _e1.sigma == _e2.sigma ) ;
}

///
ostream & operator << ( ostream& _s, const EOESGene& _e ) {
	_s << _e.val << ", " << _e.sigma << " | ";
	return _s;
}

/** Basic chromosome for evolution strategies (ES), as defined by Rechenberg and
Schwefel. Each chromosomes is composed of "genes"; each one of then is an EOESGene
@see EOESGene
*/
template <typename fitT = float >
class EOESChrom: public EOVector<EOESGene, fitT> {	
public:
	///
	EOESChrom( string _ID=""):EOVector<EOESGene, fitT>( _ID ) {};

	///
	~EOESChrom() {};
};

/// Standard output
template<class fitnessT>
ostream & operator << ( ostream& _s, EOESChrom<fitnessT>& _eo ) {
	_eo.printSelf( _s ) ; // User virtual method
	return _s;
};


#endif
