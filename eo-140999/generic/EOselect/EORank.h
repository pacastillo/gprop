#ifndef _EORANK_H
#define _EORANK_H

#include <math.h>                       // for floor
#include <ADT/EOBreeder.h>
#include <ADT/EOOp.h>


/**
 * Takes those on the selection list and creates a list of new individuals
 * Destroys the genetic pool. There's no requisite on EOT, other than the 
 * genetic operators can be instantiated with it, so it fully depends on 
 * the genetic operators used. If generic genetic operators are used, then 
 * EOT must be an EO 
 */
template<class EOT>
class EORank: public EOBreeder<EOT>{
 public:
  
  typedef vector< EOOp<EOT >*  > vecOpT;
  
  /// Ctor
  EORank( float _select = 1.0):vecOp(),repSelect( _select ) {};
  
  /** Copy ctor
   * Needs a copy ctor for the EO operators */
  EORank( const EORank&  _rankBreeder)
    :vecOp(), repSelect( _rankBreeder.repSelect ) {
    copy( _rankBreeder.vecOp.begin(), 
	  _rankBreeder.vecOp.end(), 
	  vecOp.begin() );
  };
  
  /// Dtor
  virtual ~EORank() {};
  
  /// Adds a genetic operator to the Breeder with a rate
  virtual void addOp( EOOp<EOT>* _eop )  {	
    vecOp.push_back( _eop);
  };
  
  virtual void deleteOp( const EOOp<EOT>* _eop)  {
    typename vecOpT::iterator i;
    for ( i = vecOp.begin(); i != vecOp.end(); i++ ) {
      if ( *i == _eop ) {
	vecOp.erase( i );
      }
    }
  };
  
  /** Takes the genetic pool, and returns next generation, destroying the
   * genetic pool container
   * Non-const because it might order the operator vector*/
  virtual void operator() (EOPop< EOT >& _ptVeo, EOPop< EOT >& _newPop )  { 
    
    sort( vecOp.begin(), vecOp.end(), SortEOpPt<EOT>() );
    
    unsigned i;
    float totalPriority = 0;
    for ( i = 0; i < vecOp.size(); i ++ ) {
      totalPriority += vecOp[i]->Priority();
    }
    
    unsigned inLen = _ptVeo.size(); // size of subPop
    if ( !inLen ) 
      throw EOPop< EOT >::ZeroPopulation( "in EORank");

    for ( i = 0; i < repSelect*inLen; i ++ ) {
      // Create a copy of a random input EO with copy ctor
      EOT newEO =  _ptVeo[ i ];
      
      // Choose operator
      float randomDraw = totalPriority *(rand() % 1000) /1000.0;
      
      typename vecOpT::const_iterator j;
      float accumulated = 0;
      for ( j = vecOp.begin(); 
	    ( j != vecOp.end() ) && ( accumulated < randomDraw); 
	    j ++ ) {
	accumulated+= (*j)->Priority(); // the priority
      }
      
      if ( j != vecOp.begin() )
	j--;          // previous one
      EOOp<EOT >* thisOp = *j;
      if ( thisOp->readArity() == unary ) {
	MonOp<EOT>* mopPt = static_cast< MonOp<EOT>* > ( thisOp );
	(*mopPt)( newEO );
      } else {
	unsigned chosenMatch = rand() % inLen;
	BinOp<EOT>* bopPt = static_cast< BinOp<EOT>* > ( thisOp );
	(*bopPt)( newEO, _ptVeo[chosenMatch] );
      }      

      //change the chrom... it has mutated
      //_ptVeo[ i ] = newEO;
      _newPop.push_back( newEO );
    }
  };

  /** This is a _new_ function, non defined in the parent class
   * It´s used to set the selection rate */
  void select( float _select ) {
    repSelect = _select;
  }
  
private:
  vecOpT vecOp;
  float repSelect;
  
};

#endif
