#ifndef _EOVECTOR_H
#define _EOVECTOR_H

// STL libraries
#include <vector>		// For vector<int>
#include <stdexcept>
#include <strstream>

#include <ADT/EO.h>

/** Adaptor that turns an STL vector into an EO
 with the same gene type as the type with which
 the vector has been instantiated
*/
template <class T, class fitnessT>
class EOVector: public EO<T, fitnessT>, public vector<T> {
public:
	/// ctor. Follows convention that ID should be first
	EOVector( const string& _ID = "",
				unsigned _size = 0, const T _val = 0)
		: EO<T, fitnessT>( _ID ), vector<T>( _size, _val ){ };

	/// copy ctor
	EOVector( const EOVector & _eo )
	  : EO<T, fitnessT>( _eo ), vector<T>( _eo ){ };

	/// Assignment operator
	const EOVector& operator =( const EOVector & _eo ) {
		if ( this != &_eo ){
			EO<T, fitnessT>::operator=( _eo );
			vector<T>::operator=( _eo );
		}
		return *this;
	}

	/// dtor
	virtual ~EOVector() {};

	/** methods that implement the EO <em>protocol</em>
	    @exception out_of_range if _i is larger than EO큦 size
	*/
	virtual T readGene( unsigned _i ) const {
		if ( _i > length() ) 
			throw out_of_range( "out_of_range when reading gene");
		return (*this)[_i];
	};
	
	/** methods that implement the EO <em>protocol</em>
	    @exception out_of_range if _i is larger than EO큦 size
	*/
	virtual void writeGene( unsigned _i, T _val )  {
	  if ( _i > length() )
	    throw out_of_range( "out_of_range when writing a gene");
	  (*this)[_i] = _val;
	};

	/** methods that implement the EO <em>protocol</em>
	    @exception out_of_range if _i is larger than EO큦 size
	 */
	virtual void insertGene( unsigned _i, T _val ) {
	  if (_i <= this->size() ) {
	    vector<T>::iterator i = this->begin()+_i;
	    this->insert( i, _val );
	  } else {
	    throw out_of_range( "out_of_range when inserting a gene");
	  }
	};

	/** Eliminates the gene at position _i
	@exception out_of_range if _i is larger than EO큦 size
	*/
	virtual void deleteGene( unsigned _i ) { 
		if (_i < this->size() ) {
			vector<T>::iterator i = this->begin()+_i;
			this->erase( i );
		} else {
			throw out_of_range( "out_of_range when deleting a gene");
		};
	};

	/// methods that implement the EO <em>protocol</em>
	virtual unsigned length() const { return this->size(); };

	///
	virtual void printSelf( ostream& _s ) const {
	  copy( this->begin(), this->end(), ostream_iterator<T>( _s, " ") );
	  _s << " ID -> " << ID();
	}
	
};

/// Standard output
template<class T, class fitnessT >
ostream & operator << ( ostream& _s, const EOVector<T,fitnessT>& _eo ) {
	_eo.printSelf( _s ) ; // User virtual method
	return _s;
}

#include <ADT/EOFactory.h>		// For base class
#include <util/Random.h>

/// Factory method
template<class T, class fitT = float>
class EOVectorFactory: public EOFactory< EOVector<T,fitT> >{
 public:
	 typedef EOVector<T,fitT> EOV;

  /// constructor
  EOVectorFactory(  const string& _prefix, unsigned _numElements,
						T _sRange, T _eRange):
    prefix( _prefix ), 
    numElements( _numElements ), 
	counter(0),
	eRange( _eRange ), sRange( _sRange ){};

  /// destructor
  ~EOVectorFactory() {}
  
  ///
  virtual unsigned length() const { return numElements;}

  /// One of the factory methods: creates a random EO
  virtual EOV* make() {
    string thisID= newID();
    EOV* newEO = new EOV( thisID );
    float range = eRange - sRange;
    for ( unsigned i = 0; i < numElements; i ++ ) {
	  newEO->push_back( T( sRange + range*Random::unit() ) );
    }
    return newEO;
  };
    

  /** One of the factory methods: creates an EOBin from another
	with an unary operator
	*/
  EOV* make( const EOV & _eo, 				
	  const MonOp<EOV>* _eop = 0) {
    string thisID= newID();
    EOV* newEO = new EOV( _eo );
	if ( _eop ) {	// Applies genetic operator
		(*_eop)(*newEO );
	}
	newEO->ID( thisID );
    return newEO;
  };

 
private:
	 /// Instance variables
	string prefix;	
	unsigned numElements;
	unsigned long counter;
	T eRange, sRange;


	/// Real function: creates a new ID
	string newID (){
	  //	  char IDbuf[IDBUFLEN] = "";// strstreams don't work properly
	  // sprintf( IDbuf, "%lu", counter++ );
	  strstream s;
	  s << counter++ << "\0" ;
	  string tmp;
	  s >> tmp;
	  string thisID= prefix + tmp;
	  return thisID;
	}
};



#endif
