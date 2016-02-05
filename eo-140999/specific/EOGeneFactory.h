#ifndef _EO_GENE_FACTORY_H
#define _EO_GENE_FACTORY_H

#include <string>
#include <strstream>

#include <util/Uniform.h>            // for on_off
#include <generic/EOVector.h>
#include <specific/EOString.h>
#include <specific/EOBin/EOBin.h>

/// Creates random EOs under request
template< class fitT >
class EOGeneFactory {
public:
  /// Ctor
  EOGeneFactory( string _p, unsigned _size ): prefix( _p ), counter(0), 
    size( _size){};

  /// Creates a EOVector
  EOVector<float,fitT>* makeVector( float _sRange, float _eRange) {
    string thisID = newID();

    EOVector<float,fitT>* newEO = new EOVector<float,fitT>( thisID );
    float range = _eRange - _sRange;
    for ( unsigned i = 0; i < size; i ++ ) {
	  newEO->push_back( _eRange + (rand()*10000/10000.0) / range );
    }
    return newEO;
  };

  /// Creates a EOBin
  EOBin<fitT>* makeBin() {
    string thisID= newID();
    Random rnd;
    Uniform< float > on_off( -1, 1 );
    EOBin<fitT>* newEO = new EOBin<fitT>( thisID, size );
    // Assigns values randomly
    for ( unsigned j = 0; j < newEO->length(); j++ )	{
      float oo = on_off();
      newEO->writeGene( j, (oo > 0) ? true: false );
    }
    return newEO;
  };

  /// Creates a EOStr
  EOString<fitT>* makeStr( char _eRange, char _sRange ) {
    string thisID= newID();
    EOString<fitT>* newEO = new EOString<fitT>( thisID );
    *newEO = string("");
    char range = _eRange - _sRange;
    for ( unsigned i = 0; i < size; i ++ ) {
      *newEO +=  _eRange + rand() % range ;
    }
    return newEO;
  }

private:
  string prefix;
  unsigned long counter;
  unsigned size;

  string newID (){
    strstream s;
    s << counter++;
    string thisID= prefix +s.str();
    return thisID;
  }
    
};

#endif
