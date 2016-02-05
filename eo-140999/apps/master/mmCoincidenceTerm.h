// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* mmCoincidenceTerm.h
 New termination operator used in MasterMind with	EO
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


#ifndef _mmCoincidenceTerm_H
#define _mmCoincidenceTerm_H

/** Termination object for Genetic MasterMind \\ 
Terminates when the number of rules met coincides with the number of rules so far,
or if _maxCounter steps have happened without change
@param _rules number of rules that have to be met
@param _maxCounter maximum number of generations without finding a solutions
*/
class CoincidenceTerm: public EOTerm< EOString<int> > {
public:
	
	/// Ctors/dtors
	CoincidenceTerm( const rules& _rules )
		: EOTerm< EOString<int> > (), theseRules( _rules ){};
	
	virtual ~CoincidenceTerm() {};
	
	/** Returns false if the training has to stop, true if it
	*  continues */
	virtual bool operator() ( const EOPop< EOString<int> >& _vEO ) {
		unsigned thisScore = -theseRules.dist( _vEO[0] );
		return thisScore != 0; // true if thisScore == 0
	}
	
	
private:
	const rules& theseRules;
};

#endif
