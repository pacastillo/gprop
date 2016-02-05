#ifndef _EOOP_H
#define _EOOP_H

#include <vector>
#include <ADT/EOPop.h>

/** @name Genetic operators

What is a genetic algorithm without genetic operators? There is a genetic operator hierarchy, with 
EOOp as father and MonOp (monary or unary operator) and BinOp (binary operator) as siblings. Nobody
should subclass EOOp, you should subclass BinOp or MonOp, those are the ones actually used here.
@author GeNeura Team
@version 0.0
*/
//@{

///
enum Arity { unary, binary, Nary };

/** Abstract data types for EO operators.
 * Genetic operators act on chromosomes, changing them 
 */
template<class EOType>
class EOOp {
public:

  /// Ctor
  EOOp( Arity _arity = unary, float _priority = 0 )
    :arity( _arity ), priority ( _priority ) {};

  /// Copy Ctor
  EOOp( const EOOp& _eop )
    :arity( _eop.arity ), priority ( _eop.priority ) {};

  /// Needed virtual destructor
  virtual ~EOOp(){};

  /// Arity: number of operands
  Arity readArity() const {return arity;};
	
  /// Priority: relative rate at which it is applied. This one returns priority
  float Priority() const {return priority;};

  /// Write priority
  void  Priority( unsigned _priority ) { priority = _priority;};

private:
  /// arity is the number of operator it takes
  Arity arity;

  /// priority of the operator, or rate
  float priority;

};

/// for sorting EO Ops
template <class EOType> 
struct SortEOp {
  SortEOp() {};
  bool operator () ( const EOOp<EOType> & _eop1, 
		     const EOOp<EOType> & _eop2) {
    return ( _eop2.Priority()< _eop1.Priority());
  };
};

/// For sorting EOOp pointers, can be used later on
template <class EOType> 
struct SortEOpPt {
  SortEOpPt() {};
  bool operator () ( const EOOp<EOType> * _eop1, 
		     const EOOp<EOType> * _eop2) {
    return ( _eop2->Priority()< _eop1->Priority());
  };
};

/** Binary genetic operator: subclasses EOOp, and defines
basically the operator() with two operands 
*/
template<class EOType>
class BinOp: public EOOp<EOType> {
public:

  /// Ctor
  BinOp( float _priority = 0 )
    :EOOp<EOType>( binary , _priority ) {};

  /// Copy Ctor
  BinOp( const BinOp& _ebop )
    : EOOp<EOType>( _ebop ){};

  /// Dtor
  ~BinOp () {};

  /** applies operator, to the object. If arity is more than 1,
   * keeps a copy of the operand in a cache.
   */
  virtual void operator()( EOType& _eo1, EOType& _eo2 ) const = 0;

  /// Two-EO version
  virtual void applyAt( EOType& _eo1, EOType& _eo2, 
			unsigned _i, unsigned _j) const = 0;
};

/** MonOp is the monary operator: genetic operator that takes
 only one EO
*/
template <class EOType>
class MonOp: public EOOp<EOType> {
public:

  /// Ctor
  MonOp( float _priority = 0 )
    :EOOp<EOType>( unary , _priority ) {};

  /// Copy Ctor
  MonOp( const MonOp& _emop )
    : EOOp<EOType>( _emop ){};

  /// Dtor
  ~MonOp() {};

  /** applies randomly operator, to the object. If arity is more than 1,
   * keeps a copy of the operand in a cache.
   */
  virtual void operator()( EOType& _eo1) const = 0;

  /** Apply at is used for "surgery": applies the operator to 
   * the part of the chromosome one wants.
   */
  virtual void applyAt( EOType& _eo1, 
			unsigned _i, unsigned _j ) const = 0;
};

/** NaryOp is the N-ary operator: genetic operator that takes
 several EOs. It could be called an {\em orgy} operator
*/
template <class EOType>
class NaryOp: public EOOp<EOType> {
public:

  /// Ctor
  NaryOp( float _priority = 0 )
    :EOOp<EOType>( Nary , _priority ) {};

  /// Copy Ctor
  NaryOp( const NaryOp& _emop )
    : EOOp<EOType>( _emop ){};

  /// Dtor
  ~NaryOp() {};

  /** applies randomly operator, to the object. If arity is more than 1,
   * keeps a copy of the operand in a cache.
   */
  virtual void operator()( EOPop<EOType> & _eop) const = 0;

  /** Apply at is used for "surgery": applies the operator to 
   * the part of the chromosome one wants. In this case, it
   * will need many points
   */
  virtual void applyAt( EOPop<EOType>& _eo1, 
			vector<unsigned>& _v ) const = 0;
};
//@}
#endif
