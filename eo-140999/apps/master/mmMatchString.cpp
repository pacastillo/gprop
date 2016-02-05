// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35 -*-
//-----------------------------------------------------------------------------

/* mmMatchString.cpp
   Class of strings that are going to be matched
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



#include "mmMatchString.h"

#include <algorithm>

const unsigned NUMCOLORS=20;

//--------------------------------------------------------------------
void mmMatchString::colorVector(const string& _str, 
			      vector<unsigned>& _colorVec ) const {
// -- Has as input a string with digits, and as output a Color Vector,
//    ie, the number of times each digit appears. Comp 0 is meaningless
//--------------------------------------------------------------------

    // init vector; only components > 1 have any meaning
  fill( _colorVec.begin(), _colorVec.end(), 0 );

    // now, create color vector
  for (unsigned i= 0; i < _str.size(); i++ )
    _colorVec[ _str[i] - '0']++;

}

//--------------------------------------------------------------------
match mmMatchString::check( const mmMatchString& _nmStr ) const {
// -- check incoming string and returns number of matches
//    caches result in matches
//--------------------------------------------------------------------

    match tmpMatch= {0,0};
    if (_nmStr.size() != size()) 
      return tmpMatch;               // to avoid going past the end
  
    string inStr( _nmStr );           // Local copies of strings
    string innerStr( *this );        

    // first: check number of position/color matches
    for (register unsigned i = 0; i < size(); i ++ )  {
		// cout << (*this)[i] << " " << inStr[i] << endl;
		if ((*this)[i]==inStr[i]) {
			inStr[i] = innerStr[i] = '0';	// Mark for already scanned
			tmpMatch.full++;
		}
	}
    
    // second: check number of color coincidences; 1st create the color vector
    // for the incoming vector
    vector<unsigned> matches(NUMCOLORS);	
    vector<unsigned> inColorVec(NUMCOLORS);
    // now, create color vector
    colorVector( innerStr, matches);
    colorVector( inStr, inColorVec);

    // now compares color vectors
    tmpMatch.color = 0;
	// 0 color means no color, that is why it starts by 1
    for ( register unsigned j = 1; j < NUMCOLORS; j ++ ) 
      if ( matches[j] > 0 )
		tmpMatch.color += (inColorVec[j]>matches[j] )?matches[j]:inColorVec[j]; 
//    cout << "Comparando "<< inStr << " con " << innerStr << " con resultados " << tmpMatch.full << " blancas " << tmpMatch.color << endl;

    return tmpMatch;
}
