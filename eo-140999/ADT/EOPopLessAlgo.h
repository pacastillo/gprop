#ifndef _EOPOPLESSALGO_H
#define _EOPOPLESSALGO_H

/** This is an algorithm with stupid base class name, that could be
    used for any algorithm that does not take a population as an
    argument. The only thing it intends is to give a common interface
    to those kind of algorithms
 @author GeNeura Team
 @version 0.0
*/
template< class EOT >
class EOPopLessAlgo {
public:

  /// Dtor
  virtual ~EOPopLessAlgo() {};
  
  /** Run the algorithm, until it finishes
      @return 0 for algorithm finished properly, anything else for
      fubar'ed algorithm
   */
  virtual int run( ) = 0;
  
};
	

#endif
