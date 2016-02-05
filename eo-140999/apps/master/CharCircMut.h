// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* CharCircMutate.h
 Circular mutation operator to be used with genetic mastermind and EO
 (c) GeNeura Team, 1999

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


#ifndef _CHARCIRCMUTATE_H
#define _CHARCIRCMUTATE_H

// Standard C libraries for rand
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// STL libraries
#include <vector>		// For vector<int>

#include <EO++.h>

#include <util/Uniform.h>

/// These are the genetic operators
template< class EOTchar >
class CharCircMutate: public MonOp<EOTchar> {
public:
	///
	CharCircMutate( char _lb, char _ub, float _priority = 0):
		MonOp<EOTchar>( _priority ), lb( _lb), ub( _ub) {};

	/// needed virtual dtor
	virtual ~CharCircMutate() {};

	///
	virtual void operator()( EOTchar& _eo) const {
	  Uniform<unsigned> uniform(0, _eo.length());
	  applyAt( _eo, uniform() );
	}

	/** Adds one to the gene; if it´s bigger or equal than end,
	* change to start
	*/
	virtual void applyAt( EOTchar& _eo, 
						  unsigned _i, unsigned _j = 0) const {
		int tmpC = _eo.readGene( _i );
		tmpC++;
		if (tmpC > ub)
			tmpC = lb;
		_eo.writeGene( _i, tmpC );
	}

private:
	char lb, ub;

};

#endif
