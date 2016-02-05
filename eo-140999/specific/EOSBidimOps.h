#ifndef _EO_BIDIM_OPS_H
#define _EO_BIDIM_OPS_H
/** @name EO Bidimemsional Operators
\begin{center} EO Bidimensional Operators \end{center}

Some operators for the Bidimensional Chromosmes
@author Victor Rivas - Geneura Team
@version 0.1

*/

// STL libraries
#include <vector>		// For vector<float>

#include <ADT/EO.h>
#include <ADT/EOOp.h>
#include <util/Random.h>
#include <ADT/EOBidim.h>

/** @name Operators for Bidimensional EOs
 */
//@{


/** Every gen is mutated adding or substracting a percentage
  multipliying by a value between 0 a 1
*/

template <class T, class fitnessT>
class EOBidimMutation: public MonOp< EOBidim<T,fitnessT> >{
 public:
  typedef EOBidim<T,fitnessT> chrom;
  ///
  EOBidimMutation( float _min,
		   float _max,
		   float _priority = 0):
    MonOp< chrom>( _priority) , min( _min ), max(_max){};
  
  /// needed virtual dtor
  virtual ~EOBidimMutation() {};
  
  virtual void operator()( chrom & _chrom ) const {
    
    applyAt( _chrom, 
	     (unsigned) Random::uniformInt( 0,_chrom.NumRow()+1), 
	     (unsigned) Random::uniformInt( 0,_chrom.NumCol()+1) );
  }
  
  /// applies operator to one gene in the EO
  virtual void applyAt( chrom & _chrom, 
			unsigned _i, unsigned _j ) const {
    
    Uniform<T> uniform(min, max);
    _chrom.writeGene( _i,_j, uniform() );
    
  }
 private:
  T min,max;

}; 

//@}
#endif
