#ifndef _EOSTRING_H
#define _EOSTRING_H

// STL libraries
#include <string>		
#include <stdexcept>

using namespace std;

// EO headers
#include <ADT/EO.h>

/** Adaptor that turns an STL string into an EO */
template <class fitnessT >
class EOString: public EO<char, fitnessT>, public string {
public:
	/// ctor
	EOString( const string& _str ="", 
			  const string& _ID = "",
			  fitnessT _fit = 0 )
		: EO<char, fitnessT>( _ID ), string( _str ) {};

	/// copy ctor
	EOString( const EOString<fitnessT>& _eoStr )
		:EO<char, fitnessT>( static_cast<const EO<char, fitnessT> & > ( _eoStr ) ), 
	  string( _eoStr ){};

	/// Assignment operator
	const EOString& operator =( const EOString& _eoStr ) {
		if ( this != & _eoStr ) {
			EO<char, fitnessT>::operator = ( _eoStr );
			string::operator = ( _eoStr );
		}
		return *this;
	}

	/// dtor
	virtual ~EOString() {};

	/** methods that implement the EO <em>protocol</em>
	 @exception out_of_range if _i is larger than EO큦 size
	 */
	virtual char readGene( unsigned _i ) const {
		if ( _i >= length() ) 
			throw out_of_range( "out_of_range when reading gene");
		return (*this)[_i];
	};
	
	/** Writes over gene _i, that is, the _i'eth element of the EO 
	 @exception out_of_range if _i is larger than EO큦 size
	 */
	virtual void writeGene( unsigned _i, char _val )  {
		if ( _i < length() ) {
			(*this)[_i] = _val;
		} else 
			throw out_of_range( "out_of_range when writing gene");
	};

	/** Inserts a value after _i, displacing anything to the right
	 @exception out_of_range if _i is larger than EO큦 size
	 */
	virtual void insertGene( unsigned _i, char _val ) {
		if (_i <= this->size() ) {
			string::iterator i = this->begin()+_i;
			this->insert( i, _val );
		} else 
			throw out_of_range( "out_of_range when inserting gene");
	};

	/** Eliminates the gene at position _i
	 @exception out_of_range if _i is larger than EO큦 size
	 */
	virtual void deleteGene( unsigned _i ) { 
	  if (_i < this->size() ) {
	    string::iterator i = this->begin()+_i;
	    this->erase( i );
	  } else 
	    throw out_of_range( "out_of_range when deleting gene");
	};
	
	/// methods that implement the EO <em>protocol</em>
	virtual unsigned length() const { return this->size(); };

	/// Print itself 
	virtual void printSelf( ostream& _s ) {
	  _s << *this << " ID -> " << ID();
	}

};

#endif
