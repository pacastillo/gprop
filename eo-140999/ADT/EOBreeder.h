// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOBREEDER_H
#define _EOBREEDER_H

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


/** Takes those on the selection list and creates a list of new individuals
 *  Destroys the genetic pool
 * So many instances are necessary to be able to use EOops
 * EOT is a EO-type class
 */
template<class EOT>
class EOBreeder {
public:

  /// Needed virtual destructor
  virtual ~EOBreeder(){};
  
  /// Adds a genetic operator to the Breeder 
  virtual void addOp( EOOp<EOT>* _eop) = 0;
  
  /// Remove a genetic operator from the population
  virtual void deleteOp( const EOOp<EOT>* _eop ) = 0;

  /** Takes the genetic pool, and creates the next generation, destroying the
      genetic pool container, and creating a new vector with _pop 
      components.\\
      It´s not const since it might order the operator vector. You shouldn't assume
      it's ordered, though
  */
  virtual void operator() ( EOPop<EOT>& _pop, EOPop<EOT>& _newPop ) = 0;
};
//@}
#endif
