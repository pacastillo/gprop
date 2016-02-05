// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c-comment-column:35; -*-

//-----------------------------------------------------------------------------
/* rules.cpp

 Implementation of rules class

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

#include "rules.h"

//-------------------------------------------------------------
void rules::addRule( const mmMatchString& _nwString, match _result ) {
// -- Adds another rule to the array
//--------------------------------------------------------------
  rulesArr.push_back( _nwString );    
  matchesArr.push_back( _result );
}
  
//-------------------------------------------------------------
bool rules::isRule( const string& _inStr ) const {
// -- Checks if the string is already in the database, returns
//    TRUE if it does
//-------------------------------------------------------------
    bool isIn = false;
    
    for ( unsigned i = 0; (i < rulesArr.size() ) && !isIn; i ++ ) {
		match tmpMatch = rulesArr[i].check( _inStr );
		if ( tmpMatch.full == _inStr.size() )
			isIn = true;
	}
    return isIn;

}

/*---------------------------------------------------------------------------
 Computes the distance to the rules in the base, that is, the
	absolute value difference between the actual number of white and
	blacks and the number of w&b obtained matching the string with the rules
	@param _mString String to match
		@return distance to the actual rules; 0 if all rules met
-----------------------------------------------------------------------------*/
int rules::dist( const mmMatchString& _mString ) const {
	int tmpDist = 0;
	for ( unsigned i = 0; i < rulesArr.size(); i ++ ) {
      match tmpMatch = rulesArr[i].check( _mString );
      //tmpDist += abs(tmpMatch.full - matchesArr[i].full);
	  //tmpDist += abs(tmpMatch.color - matchesArr[i].color );
	  tmpDist += abs(tmpMatch.full - matchesArr[i].full);
	  tmpDist += abs(tmpMatch.full + tmpMatch.color 
			- matchesArr[i].color- matchesArr[i].full);

	}
	return tmpDist;
}

//-------------------------------------------------------------
unsigned rules::numMatches( const mmMatchString& _mString,
			    unsigned& _numBlacks,
			    unsigned& _numWhites ) const {
// returns the number of rules it meets
//-------------------------------------------------------------

  unsigned tmpNMatches = 0;
  _numBlacks = _numWhites = 0;	// Do not forget initialisation
  for ( unsigned i = 0; i < rulesArr.size(); i ++ ) {
      match tmpMatch = rulesArr[i].check( _mString );
//      cout << "tmp " << tmpMatch.full << " F " << tmpMatch.color << endl;
      if ( (tmpMatch.full == matchesArr[i].full) 
	   && (tmpMatch.color == matchesArr[i].color ) ) {
		tmpNMatches++;
		_numBlacks += tmpMatch.full;
		_numWhites += tmpMatch.color;
      }
//      cout << "Otro " << matchesArr[i].full << " F " << matchesArr[i].color << endl;
  }
  return tmpNMatches;

}

