#ifndef _EOPOP_H
#define _EOPOP_H

#include <vector>
using namespace std;

#include <util/UException.h>

/** Subpopulation: it is used to move parts of population
 from one algorithm to another and one population to another. It is safer
to declare it as a separate object. I have no idea if a population can be
some other thing that a vector, but if somebody thinks of it, this concrete
implementation will be moved to "generic" and an abstract Population 
interface will be provided
@author Geneura Team
@version 0.0
*/

template<class EOT>
class EOPop: public vector<EOT> {

  /** This exception should be thrown when the population is 0; even in methods 
      outside the EOPop class.
  */
  class ZeroPopulation : public UException {
      public:
      /**
       * Constructor
       */
      ZeroPopulation( const string& _msg)
      : UException("Some population seems to have 0 EOs in" + _msg) { };

      virtual ~ZeroPopulation() throw()  {};
  };

 public:
  ///
  EOPop()
    :vector<EOT>() {};
  ///
  ~EOPop() {};
};
#endif
