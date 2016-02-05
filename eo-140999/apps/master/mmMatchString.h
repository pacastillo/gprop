// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8;  -*-

/* mmMatchString.h
   DESCRIPTION: Container class that contains a string with digits, that
                will represent colors, with a certain length, and returns
                a vector with the number of full position matches and
                digit matches

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

#ifndef __MATCHSTR_HPP
#define __MATCHSTR_HPP

#include <vector>
#include <string>

using namespace std;

struct match {
	unsigned full;               // full position matches
	unsigned color;              // matches in color, but not in position
};

/*
// To avoid problems with VC++
bool operator < ( match _m1, match _m2 ) {
	return _m1.full < _m2.full;
}

// To avoid problems with VC++
bool operator == ( match _m1, match _m2 ) {
	return _m1.full == _m2.full;
}
*/

/** Class used to store strings that are going to be matched, and 
    to match them against the secret combination */
class mmMatchString: public string {

public:

  ///
  mmMatchString() {};				// Dummy to allow vectorization

  ///
  mmMatchString( const string& _str )
    :string(_str) {};

  ///
  mmMatchString( const mmMatchString& _nmString )
		:string(_nmString ) {};
	
  ///
    ~mmMatchString( void ) {};
	
  ///
  match check( const mmMatchString& _nmString ) const;
	
private:

  void colorVector(const string& _str, 
		   vector<unsigned>& _colorVec ) const;		
	
};

#endif
