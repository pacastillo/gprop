// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOSELECTOR_H
#define _EOSELECTOR_H

#include <ADT/EOOp.h>
#include <ADT/EOPop.h>

/** @name {\em Selection and reproduction algorithms}

Selection algorithms are used to select the EO that are going to reproduce, and put them on a 
pool; breeders take the genetic pool and apply genetic operators on them to create the new Generation.\\

These classes will usually be used inside a EOAlgorithm.
@author J. J. Merelo
@version 0.0
@see EOAlgorithm
*/
//@{

/** Selects those who are going to reproduce, eliminates those which
 * are not fit */ 
template<class EOT>
class EOSelector{
public:
  /// Needed virtual destructor
  virtual ~EOSelector(){};
  
  /// Creates and returns the genetic pool of the reproducers
  virtual void  operator() ( EOPop<EOT>& _old, EOPop<EOT>& _new ) const = 0;
};

/** A generalized population operator, takes a population and changes it
 */
template<class EOT>
class EOPopOp{
public:

  /// Needed virtual destructor
  virtual ~EOPopOp(){};
  
  /** Changes the population
  */
  virtual void operator() ( EOPop<EOT>& _pop ) = 0;
};

//@}
#endif
