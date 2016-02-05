// Abstract data types for EO class

#ifndef _EO_H
#define _EO_H

#include <iostream>				// for ostream

// EO Includes
#include <ADT/EOBase.h>

using namespace std;

/** @name EO class 
* Randomly accesible evolvable object, with variable length.
 * Wrapper for any object that is to be evolved
* @author J. J. Merelo
* @version 0.0
*/

//@{

/** EO: Base class for the whole hierarchy
#T# is the type it will be instantiated with; this type must have, at
least, a copy ctor, assignment operators, 
*/
template<class T, class fitnessT = float>
class EO: public EOBase< fitnessT > {
  public:
  
  ///
  typedef T EOType;
  
  /** Can be used as default ctor; should be called from derived classes. Fitness should be
      at birth
      @param _ID An ID string, preferably unique
  */
  EO( const string& _ID = "" )
  :EOBase<fitnessT> ( _ID ) {};
  
  /// Copy ctor
  EO( const EO& _eo )
  :EOBase<fitnessT> ( _eo ) {};
  
  /// Assignment operator
  const EO& operator= ( const EO& _eo ) {
    EOBase<fitnessT>::operator = ( _eo );
    return *this;
  }
  
  /// Needed virtual dtor
  virtual ~EO(){};
  
  /** Reads and returns a copy of the gene in position _i
      This implies that T must have a copy ctor 
      @param _i index of the gene, which is the minimal unit. Must be an unsigned 
      less than #length()# 
  */
  virtual T readGene( unsigned _i ) const = 0;
  
  /** Overwrites the gene placed in position _i with a
   * new value. This means that the assignment operator
   * for T must be defined 
   @param _i index
   @param _val the value that will overwite the element at _i 
  */
  virtual void writeGene( unsigned _i, T _val ) = 0;
  
  /** Inserts a gene, moving the rest to the right. If
   * _i = length(), it should insert it at the end.
   * Obviously, changes length 
   @param _i index
   @param _val new value
  */
  virtual void insertGene( unsigned _i, T _val ) = 0;
  
  /** Eliminates the gene at position _i; all the other genes will
      be shifted left
      @param _i index
  */
  virtual void deleteGene( unsigned _i ) = 0;
  
  /// Returns the number of genes in the EO
  virtual unsigned length() const = 0;
  
  /** Print itself: default implementation. Declared virtual so that 
      it can be reimplemented anywhere */
  virtual void printSelf( ostream& _s ) {
    for ( unsigned i = 0; i < length(); i ++ ) {
      _s << this->readGene( i ) << " ";
    }
    _s << "Fitness = " << this->fitness() << " " << this->ID() << endl;
  }
  
};

//@}
#endif
