// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-

/*   NumOnes.h
     
Implementation as an EO evaluation function of a function that adds up
the number of ones.

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

*/

#include <specific/EOBin/EOBin.h>
#include <ADT/EOEvalFunc.h>

typedef EOBin<float> Chrom;

/** Implementation of a function evaluation object.\
Adds the number of ones in a binary chromosome
*/
struct NumOnes: public EOEvalFunc< Chrom > {
  /** Takes a chromosome, and adds up the number of ones in it
      @param _eo the chromosome that is going to be evaluated
      @return  the number of ones as a floating point number
  */
  virtual float operator()(  Chrom & _eo ) const {
    float numOnes = 0;
    for ( unsigned i = 0; i < _eo.length(); i ++ ) 
      numOnes += ( _eo[i] == true)?1:0 ;
    return numOnes;
  };
};
