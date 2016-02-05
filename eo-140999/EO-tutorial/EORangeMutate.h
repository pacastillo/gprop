// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; -*-

/*   EORangeMutate.h
     
Genetic operator that mutates components keeping them within a range

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

#ifndef _EORANGEMUTATE_H
#define _EORANGEMUTATE_H

#include <ADT/EOOp.h>

/** Mutates by adding or substracting a random number with a uniform
    distribution, but keeps the result within a range */
template<class EOT>
class EORangeMutate: public MonOp<EOT>{

#ifdef _MSC_VER
  typedef EOT::EOType EOType;
#else
  typedef typename EOT::EOType EOType;
#endif

 public:
  ///
  EORangeMutate(  EOType _mutRange, EOType _lowerBound, 
		  EOType _upperBound, float _priority = 1):
    MonOp<EOT>( _priority ), range( _mutRange ), lowerBound (_lowerBound), 
    upperBound( _upperBound )  {};
  
  /// needed virtual dtor
  virtual ~EORangeMutate() {};
  
  virtual void operator()( EOT& _eo ) const {
    unsigned length=_eo.length();
    for ( unsigned i=0; i<length; i++ ) { 
      applyAt( _eo, i );
    } 
  }
  
  /// applies operator to one gene in the EO
  virtual void applyAt( EOT& _eo, 
			unsigned _i, unsigned _j=0 ) const {
    
    EOType tmpC1 = _eo.readGene( _i );
    Uniform<EOType> uniform( 0, range*2);
    tmpC1 +=  uniform() - range;
    tmpC1 = ( tmpC1 > upperBound )?upperBound:tmpC1;
    tmpC1 = ( tmpC1 < lowerBound )?lowerBound:tmpC1;
    _eo.writeGene( _i, tmpC1 );
    
  }
 private:
  EOType range, lowerBound, upperBound;

  /// Empty ctor does not make much sense
  EORangeMutate() {};

}; 

#endif
