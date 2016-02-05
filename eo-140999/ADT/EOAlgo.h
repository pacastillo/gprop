#ifndef _EOALGO_H
#define _EOALGO_H

#include <ADT/EOPop.h>                   // for population

/** Algorithm that performs the evolutionary computation
 * Might be genetic or not 
 @author GeNeura Team
 @version 0.0
*/
template< class EOT >
class EOAlgo {
public:

  /// Dtor
  virtual ~EOAlgo() {};
  
  /** Run the algorithm on a population
   * @param _pop is the population that the algorithm is acting on
   */
  virtual void operator() ( EOPop<EOT>& _pop ) = 0;
  
};
	

#endif
