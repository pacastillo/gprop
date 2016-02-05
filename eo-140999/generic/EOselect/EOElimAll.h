// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOELIMALL_H
#define _EOELIMALL_H

// STL includes
#include <ADT/EOPop.h>
#include <ADT/EOReplace.h>


/** Selects those who are going to reproduce, eliminates those which
 * are not fit */ 
template<class EOT>
class EOElimAll: public EOReplace<EOT>{
public:

  /// Void ctor
  EOElimAll() {};

  /// Needed virtual destructor
  virtual ~EOElimAll(){};
  
  /// Creates and returns the genetic pool of the reproducers
  virtual void operator() ( EOPop<EOT>&  _in, EOPop<EOT>&  _out ) const {
   _out = _in;
  }
};


#endif
