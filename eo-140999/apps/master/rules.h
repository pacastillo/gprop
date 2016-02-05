// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8;  c-comment-column: 35; -*-

// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* rules.h

 DESCRIPTION: class that contains a set of draws of the MasterMind game.
             It contains an array of couples 
             draw --- matches
             The interface contains functions to add rules, to compare
             a draw with all the rules, and so on.
 
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
// 				


#ifndef __RULES_HPP
#define __RULES_HPP

#include "mmMatchString.h"

#include <string>
#include <vector>

using namespace std;

class rules {

public:

	rules():rulesArr(), matchesArr() {};
	~rules() {};

	void addRule( const mmMatchString& _nwString, match _result );
	/** Returns 0 if it is different to all the rules already in 
		the array 
		@param _inStr the string to match */
	bool isRule( const string& _inStr ) const; 
				
	/** Computes the distance to the rules in the base, that is, the
		absolute value difference between the actual number of white and
		blacks and the number of w&b obtained matching the string with the rules
		@param _mString String to match
		@return distance to the actual rules; 0 if all rules met
		*/
	int dist( const mmMatchString& _mString ) const;

	/// returns the number of rules it meets
	unsigned numMatches(const  mmMatchString& _mString,
			    unsigned& _numBlacks,
			    unsigned& _numWhites) const;

	/// Total number of black tokens.			
	unsigned numBlacks(const mmMatchString& _mString, 
			   unsigned& _rules ) const;

  /// Number of rules (guesses made so far)
  unsigned numRules() const { return rulesArr.size();};

private:
	vector<mmMatchString> rulesArr;			// color strings
	vector<match> matchesArr;			// number of matches

};




#endif
