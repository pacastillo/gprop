// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* mmEvalRules.h
 An evaluation class to evaluate combinations for genetic mastermind
 (c) J. J. Merelo, 1999

	This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
-----------------------------------------------------------------------------*/


#ifndef _mmEvalRules_H
#define _mmEvalRules_H

#include "rules.h"

/** Evaluation function object: it computes the number of changes the combination
must undergo to become a "correct" combination, that is, one that meets all the rules
*/
struct mmEvalRules: public EOEvalFunc< mmChrom > {
	
  mmEvalRules( const rules& _rules)
    : theseRules( _rules ) {};
  
  virtual ~mmEvalRules() {};
  
  /** This operator returns a negative int distance, that is the distance to a "correct"
      , that is, possibly the secret, combination. 0 distance means that it could be correct
  */ 
  virtual int operator()( mmChrom & _eo ) const {
    int dist = 
      theseRules.dist( _eo ) ;
    return - dist ;
  };
  
private:
  const rules& theseRules;
};

#endif
