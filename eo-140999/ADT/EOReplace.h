// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOREPLACE_H
#define _EOREPLACE_H

#include <ADT/EOPop.h>

/** Abstract data type for Replacement algorithms, which will be implemented elsewhere */ 
template<class EOT>
class EOReplace{
public:
  /// Needed virtual destructor
  virtual ~EOReplace(){};
  
  /** Creates and returns the genetic pool of the reproducers
      @_in the population that is going to be included, the new generation
      @_out the population that remains
  */
  virtual void operator() ( EOPop<EOT>&  _in, EOPop<EOT>& _out ) const = 0;
};

#endif
