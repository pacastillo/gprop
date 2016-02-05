#ifndef EOGENERIC_H
#define EOGENERIC_H

/** @name EO Generic Classes
\begin{center} EO Generic Classes \end{center}


Generic classes are one step above Abstract classes; they are concrete 
implementations of abstract classes, but they can be used for many 
instantiations of them. For instance, a crossover genetic operator (which
should subclass EOop) can be used for any chromosome EO.

  This genericity is achieved through templates; generic classes provide
  code, but code is type-independent, or at least for those types used to 
  instantiate the class. The type with which the class is instantiated is
  indicated in its declaration: fitT or fitnessT is a fitness-Type, which
  should have the same interface as EOfitness, or directly subclass EOfitness,
  which is the same since EOfitness has no code per se; EOT is an EO; that
  template parameter should only be instantiated with an EO.

  Generic classes can be user-created, but they should be user-contributed:
  if you program a class and feel it can be used for a wide range of 
  audiences, please tell us.

  The #generic# subdir includes also subsubdirs for instantiation of specific
  ADTs: EOselect contains specific instantiations of the EOselect abstract
  data type.

  Generic classed should have EOG (as in EO-Generic) as preffix; this is not always respected
  but it should. EOGS indicates EO-Generic and implementations of Select-type
  objects.

@author J. J. Merelo
@version 0.1
*/
//@{
	//@Include: generic/EO*.h
	//@Include: generic/EOselect/EO*.h
//@}

#include <generic/EOEvalAll.h>
#include <generic/EOGenTerm.h> 
#include <generic/EOFitTerm.h>
#include <generic/EOGOps.h>
// Algorithms
#include <generic/EOEasyGA.h>
#include <generic/EOSGA.h>
#include <generic/EOSimAnnAlg.h>
// EOs
#include <generic/EOVector.h>

// Selection ops
#include <generic/EOselect/EOLottery.h>
#include <generic/EOselect/EOTournament.h>
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOselect/EORandomSelect.h>

//Reproduction ops
#include <generic/EOselect/EORandomBreed.h>
#include <generic/EOselect/EORank.h>

//Replacement ops
#include <generic/EOselect/EOAgeReplace.h>
#include <generic/EOselect/EOReplacers.h>

// for age GA's
#include <generic/EOAge.h>
#include <generic/EOselect/EOAgeReplace.h>
#include <generic/EOAgeGA.h>

#endif
