/** @name EO Specific Classes
\begin{center} EO Specific Classes \end{center}

Specific classes have been placed here because they are usually
chromosome or population specific; they are only valid for some kind
of chromosomes, for instance, chromosomes with boolean genes, or
chromosomes with floating-point genes. The specifity of these classes
refers to the EO templates it can be instantiated with.\\

There are basically three groups: floating point, string and boolean
operators; there is also a Freezer which can be used as cooling
schedule for simulated annealing; this operator is algorithm-specific: 
it can be used only with SimAlg.

@author J. J. Merelo
@version 0.1

*/
//@{
	//@Include: specific/EO*.h
	//@Include: specific/EOBin/EO*.h
//@}

#include <specific/EOLinearFreezer.h>
#include <specific/EOSIntOps.h> 
#include <specific/EOSFloatOps.h>
#include <specific/EOString.h>
#include <specific/EOBin/EOView.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>

