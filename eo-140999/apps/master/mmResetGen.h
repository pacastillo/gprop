// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* mmResetTerm.h
 Several specific population operators for EO and genetic Mastermind
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


#ifndef _mmResetGen_H
#define _mmResetGen_H

#include <specific/EOString.h>
#include <ADT/EOSelector.h>
#include <generic/EOEasyGen.h>
#include <util/Uniform.h>

using namespace std;

typedef EOString<int> mmChrom;

//_________________________________________________________________

/// random generation of strings
void randomGen( string& _str, unsigned _len, unsigned _colors ) {
	_str="";
	char lastChar = '1' + _colors;
	Uniform<char> uniform('1',lastChar);
	for ( unsigned j = 0; j < _len ; j ++ ){
		_str += uniform(  );
	}
};

//_________________________________________________________________

/// Generates a new population from scratch
class ResetPop: public EOPopOp<mmChrom > {
public:
	ResetPop( unsigned _size, 
		unsigned _posiciones, unsigned _colores ):
	size( _size), posiciones (_posiciones), colores( _colores ) {};

	/// takes an empty population and fills it with random chromosomes
	virtual void operator() ( EOPop< mmChrom >& _pop ) {
	  _pop.clear();
	  for ( unsigned j = 0; j < size; j ++ ) {
	    mmChrom newStr;
	    randomGen( newStr, posiciones, colores );
	    _pop.push_back( newStr );
	  }
	};

private:
	unsigned size, posiciones, colores;
};

//_________________________________________________________________

/// Generates a new population from scratch
class DoublePop: public EOPopOp<mmChrom > {
public:
  DoublePop( unsigned _size, 
	     unsigned _posiciones, unsigned _colores ):
    size( _size), posiciones (_posiciones), colores( _colores ) {};
  
  /// takes a non-empty population and adds random chromosomes
  virtual void operator() ( EOPop< mmChrom >& _pop ) {
    for ( unsigned j = 0; j < size; j ++ ) {
      mmChrom newStr;
      randomGen( newStr, posiciones, colores );
      _pop.push_back( newStr );
    }
  };
  
private:
  unsigned size, posiciones, colores;
  
};



//_________________________________________________________________

/** This class follows the "decorator" idiom; it takes a generation object and converts
it into an object that, besides doing that, resets the population every a certain number
of generations.
 */
template<class EOT>
class EOResetGen: public EOEasyGen<EOT> {
 public:
  
	/** Main ctor, with most of the stuff passed to base class ctor
	*/
	EOResetGen( EOEvaluator<EOT>&  _eval,
	     EOSelector< EOT>& _select,
	     EOBreeder<EOT>& _breed,
	     EOReplace<EOT>& _rep,
	     bool _verbose,
		 unsigned _maxCounter, 
		 EOPopOp<EOT>& _popOp )
		:EOEasyGen<EOT>( _eval, _select, _breed, _rep, _verbose), maxCounter( _maxCounter), numGenerations( 0 ), lastFitness(0), 
		popOp( _popOp), totalEvaluated(0) {};


  /** Copy ctor
   */
  EOResetGen( const EOResetGen<EOT>& _gen )
    :EOEasyGen<EOT>( _gen ), maxCounter( _gen.maxCounter), 
	numGenerations( 0 ),lastFitness(0), popOp( _gen.popOp ), totalEvaluated(0) {};


  /// Ctor must ve virtual
  virtual ~EOResetGen() {};
  
  /// Run the algorithm
  virtual void operator () ( EOPop<EOT>& _pop ) {
    
    // applies the internal generation operator
    EOEasyGen<EOT>::operator ()( _pop );
    
    // get the best, the population is sorted
    const EOString<int>& bestEO= _pop[0];
    numGenerations++;
    if ( bestEO.fitness() == lastFitness ) {
      counter++;
    } else {
      counter = 1;
    }
    lastFitness = bestEO.fitness();
    
    // reboot after a number of generations
    if ( counter % maxCounter == 0 ) {
      popOp( _pop );
      counter = 0;
      EOEvaluator<mmChrom>& thisEval = getEval();
      thisEval( _pop );	// reevaluate population 
      totalEvaluated += _pop.size();
    } 
    
  };

  ///
  void resetCounter() { counter = 0; };

  ///
	unsigned getNumGenerations() const { return numGenerations;};
  
	/// 
	unsigned getTotalEvaluated() const { return totalEvaluated;};


 private:
  unsigned maxCounter, counter, numGenerations;
  int lastFitness;
  EOPopOp<EOT>& popOp;
  unsigned totalEvaluated;
  
};

#endif
