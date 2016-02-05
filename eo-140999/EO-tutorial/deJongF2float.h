// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-

/*   deJongF2.h
     
Implementation of the classical de Jong's F2 function.

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

#include <generic/EOVector.h>
#include <ADT/EOEvalFunc.h>

typedef EOVector<float,float> Chrom;

/** Implementation of a function evaluation object.\
    This is an implementation of the classical DeJong's F2 function\
    taken from CMU GA-benchmark repository. In this case, the representation \
    of the parameters are a floating point array
*/
struct deJongF2float: public EOEvalFunc< Chrom > {
  /** Takes a chromosome, and adds up the number of ones in it
      @param _eo the chromosome that is going to be evaluated
      @return  the number of ones as a floating point number
  */
  virtual float operator()(  Chrom & _eo ) const {
    float ans;
    ans = _eo[0] * _eo[0] - _eo[1];
    ans *= 100.0*ans;
    ans += (1.0 - _eo[0])*(1.0 - _eo[0]);
    return(ans);
    
  };
};

