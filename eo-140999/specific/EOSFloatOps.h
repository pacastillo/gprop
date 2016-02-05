#ifndef _FLOATEO_H
#define _FLOATEO_H

// STL libraries
#include <vector>		// For vector<float>

#include <ADT/EO.h>
#include <ADT/EOOp.h>
#include <util/Uniform.h>

/** @name Operators for floating-point EOs
 */
//@{

/// Average crossover for arrays of floats
template< class EOTfloat >
class EOFloatArrayXoverAverage: public BinOp<EOTfloat> {
 public:
  ///
  EOFloatArrayXoverAverage( float _priority = 0):
    BinOp<EOTfloat>( _priority ){};
  
  /// needed virtual dtor
  virtual ~EOFloatArrayXoverAverage() {};
  
  virtual void operator()( EOTfloat& _eo,
			   EOTfloat& _eo2  ) const {
    // Change only the minimun length
    unsigned min=(_eo.length()<_eo2.length())?_eo.length():_eo2.length();
    for ( unsigned i=0; i<min; i++ ) { 
      applyAt( _eo, _eo2, i,i );
    } 
  }
  
  /// applies operator to one gene in the EO
  virtual void applyAt( EOTfloat& _eo, 
			EOTfloat& _eo2, 
			unsigned _i, unsigned _j ) const {
    
    float tmpC1 = _eo.readGene( _i );
    float tmpC2 = _eo2.readGene( _j );
    float average=(tmpC1+tmpC2)/2;
    _eo.writeGene( _i,average );
    
  }

}; //class EOFloatArrayXoverAverage

/** Every gen is mutated adding or substracting a percentage
  multipliying by a value between 0 a 1
*/
template< class EOTfloat >
class EOFloatArrayMutateByPercentage: public MonOp<EOTfloat>{
 public:
  ///
  EOFloatArrayMutateByPercentage( float _percentage,
				  float _priority = 1):
  MonOp<EOTfloat>( _priority ), percentage(_percentage) {};
  
  /// needed virtual dtor
  virtual ~EOFloatArrayMutateByPercentage() {};
  
  virtual void operator()( EOTfloat& _eo ) const {
    unsigned length=_eo.length();
    for ( unsigned i=0; i<length; i++ ) { 
      applyAt( _eo, i );
    } 
  }
  
  /// applies operator to one gene in the EO
  virtual void applyAt( EOTfloat& _eo, 
			unsigned _i, unsigned _j=0 ) const {
    
    float tmpC1 = _eo.readGene( _i );
    Uniform<unsigned> uniform(0, 2);
    unsigned aleatorio = uniform();

    tmpC1*=(aleatorio)?(1+percentage):(1-percentage);
    _eo.writeGene( _i, tmpC1 );
    
  }
 private:
  float percentage;

}; 

/** Mutates by adding or substracting a random number with a uniform distribution */
template< class EOTfloat >
class EOFloatArrayRndMutate: public MonOp<EOTfloat>{
 public:
  ///
  EOFloatArrayRndMutate( float _range,
			 float _priority = 1):
    MonOp<EOTfloat>( _priority ), range( _range ) {};
  
  /// needed virtual dtor
  virtual ~EOFloatArrayRndMutate() {};
  
  virtual void operator()( EOTfloat& _eo ) const {
    unsigned length=_eo.length();
    for ( unsigned i=0; i<length; i++ ) { 
      applyAt( _eo, i );
    } 
  }
  
  /// applies operator to one gene in the EO
  virtual void applyAt( EOTfloat& _eo, 
			unsigned _i, unsigned _j=0 ) const {
    
    float tmpC1 = _eo.readGene( _i );
    Uniform<float> uniform( 0, range*2);
    _eo.writeGene( _i, tmpC1-range + uniform() );
    
  }
 private:
  float range;


}; 


/** IntermXOver computes an arithmetic average of the two chromosomes. It´s used
in Evolution Strategies. Can be applied to any EO with arithmetic operations, but
is designed for float.
*/
template< class EOTfloat >
class EOFloatIntermXOver: public BinOp<EOTfloat>{
 public:
  ///
  EOFloatIntermXOver( float _priority = 0):
    BinOp<EOTfloat>( _priority ){};
  
  /// needed virtual dtor
  virtual ~EOFloatIntermXOver() {};
  
  /// Leaves the second operator unchanged
  virtual void operator()( EOTfloat& _eo, EOTfloat& _eo2) const {
	  unsigned len1 = _eo.length(), len2 = _eo2.length(), 
	    len= (len1 > len2)?len2:len1;
	  for ( unsigned i = 0; i < len; i ++ ) {
		_eo.writeGene( i, ( _eo.readGene(i ) + _eo.readGene(i) ) /2 );
	  }
  }
  
	/// Directed application of intermediate xOver... makes no sense, actually.
	virtual void applyAt( EOTfloat& _eo, EOTfloat& _eo2, 
							unsigned , unsigned ) const {
		(*this)( _eo, _eo2 );
	};

}; 
//@}
#endif
