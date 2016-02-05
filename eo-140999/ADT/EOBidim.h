//-----------------------------------------------------------------------------
// EOBin.h
//-----------------------------------------------------------------------------

#ifndef _EOBIDIM_H
#define _EOBIDIM_H

//-----------------------------------------------------------------------------


#include <iostream>		// for ostream
#include <strstream>    // for strstream
#include <iterator>
#include <stdexcept>    // for out_of_range
#include <vector>

using namespace std;

#include <ADT.h>

#include <util/Random.h>
//-----------------------------------------------------------------------------

/** 
  Bidimensional Chromosomes
 @author Geneura Team
 */
template<class T, class fitnessT = float>
class EOBidim: public EOBase< fitnessT >, public vector< vector< T > > {
 public:
  /// constructor
  EOBidim( const string& _ID = "", unsigned _numRow = 0, unsigned _numCol = 0 ):
    EOBase<fitnessT>( _ID ), 
    vector< vector<T> >(_numRow )
    {
      vector< vector<T> >::iterator i;

      for ( i=begin(); i!=end(); i++ ) {
	i->resize( _numCol,0 );
      }
    };

  /// copy constructor
  EOBidim( const EOBidim& _eobidim ):
    EOBase<fitnessT>( _eobidim ), 
    vector< vector<T> >(_eobidim ) {};

  /// Operator =
  const EOBidim& operator = ( const EOBidim& _eobidim ) { 
    if ( &_eobidim != this ) {
      EOBase<fitnessT>:: operator = ( _eobidim );
      copy( _eobidim.begin(), _eobidim.end(), this->begin());
    }
    return *this;
  }

  /// destructor
  ~EOBidim() {}
  
  /// 

  T readGene( unsigned _i, unsigned _j ) const {
    if ( _i >= NumRow() || _j>=NumCol() ) 
      throw out_of_range( "out_of_range when reading gene");
    return ( *this )[ _i ][ _j ];
  }
  
  ///

  void writeGene( unsigned _i, unsigned _j, T _val ) {
    if ( _i < NumRow() && _j<NumCol() ) {
      (*this)[_i][_j] = _val;
    } else 
      throw out_of_range( "out_of_range when writing gene");
  }
  
  ///

  void insertRow( unsigned _i, T _val ) {
    if (_i < NumRow() ) {
      vector<T> newVector( NumCol(),_val );
      insert( begin() + _i, vector<T>( NumCol(),_val ) );
    } else 
      throw out_of_range( "out_of_range when inserting gene");
  };

  ///
  void insertCol( unsigned _j, T _val ) {
    vector< vector<T> >::iterator i;
    for ( i=begin(); i!=end(); i++ ) {
      if (_j < i->size() ) {
	i->insert( i->begin() + _j, _val );
      } else 
	throw out_of_range( "out_of_range when deleting gene");
    }
  };

  ///
  void deleteRow( unsigned _i ) {
    if (_i < NumRow() ) {
      erase( begin() + _i );
    } else 
      throw out_of_range( "out_of_range when deleting gene");
  }; 

  ///
  void deleteCol( unsigned _j ) {
    vector< vector<T> >::iterator i;
    for ( i=begin(); i!=end(); i++ ) {
      if (_j < i->size() ) {
	i->erase( i->begin() + _j );
      } else 
	throw out_of_range( "out_of_range when deleting gene");
    }
  };
  
  /// 
  unsigned NumRow() const { return size(); }

  ///
  unsigned NumCol() const { return front().size(); }

  ///

  unsigned length( unsigned _dimension ) const { 
    return( _dimension==0)?NumRow():NumCol();
  }

 private:

};

///

template< class T, class fitT>
ostream& operator<<( ostream& os, const EOBidim<T,fitT> & chrom )  {

  // unsigned i,j, maxI=chrom.NumRow(),maxJ=chrom.NumCol();
  vector <vector <T> >::const_iterator row;

  os << "Bidimensional Chromosome: "<<chrom.ID()<<endl;

  for ( row=chrom.begin(); row!=chrom.end(); row++ ) {
    copy( row->begin(), row->end(), ostream_iterator<T>( os,"\t" ));
    os << endl;
  }
  
  return os;
};


/** Evaluator takes a vector of EOs and evaluates its fitness
* returning void. Template instances must be of fitness and EO type
*/
template<class EOT >
class EOBidimEvaluator: public EOEvaluator<EOT> {
public:
	/// Needed virtual destructor
	virtual ~EOBidimEvaluator() {};

	/// Sets the evaluation function
	void Ef( const EOEvalFunc< EOT> & _ef ) { ef=_ef;};

	/** This is the actual function: it takes a vector of pointers to eo
	 * @param _vEO is a vector of pointers to eo, that will be evaluated
	 */
	void operator()( EOPop< EOT >& _vEO ) {
	  _vEo.fitness=10;
	};
	
};
#endif EOBIDIM_H
