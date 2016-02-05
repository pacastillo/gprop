
/** @name EO framework and library description


  EO is a library created in Granada, Spain, Europe, with the main target
  of being an open platform for evolutionary computation, in general.\\

  EO provides interfaces (in the #ADT#, abstract data types, directory),
  templates which follow those interfaces (in the #generic# directory),
  specific implementation of those templates (in the #specific# directory),
  and utilities, graphics representation, and so on a so forth.\\

  EO has been designed so that all EC paradigms, except maybe genetic 
  programming, can fit into it. All those paradigms are not that different,
  having for instance different genetic representations (in EO any object or
  {\em chromosome} with randomly accesible components or {\em genes} can be
  evolved), or selection and reproduction strategies (in EO, selection and
  reproduction strategies are just mappings from one {\em population} to
  another, so any of them can fit in).\\

  The idea behind EO is component- and object-based computing. EO provides
  interfaces and some examples, but the community that uses it should provide
  the rest of components, needed to program {\em any} EC application, in
  any environment. EO has been developed and tested in WinNT and Unix machines,
  so it should have no problem in any one of them.

  If you have any new component to include in EO, please write to us in 
  eodev@kal-el.ugr,es; also if there is any compilation/runtime problem.

\begin{center} {\em Conventions} \end{center}
 This library will use an initial capital for all classes, initial
 lowercase for all objects.
 Abstract data types like EO belonging to the library will have the
 EO preffix, as in evolutionary || evolvable object, or both.
 Some other conventions \begin{itemize}
 \item Capitals (uppercase) will also be used after EO
 \item Parameters passed to functions will have an initial underscore
\item class methods will start by lowercase; 
 \item Valid template instantiation classes will be indicated with the name
  of the generic class. For instance, if a template is to be instantia
  ted with EOs, the type used in the "class" clause will EOT (as in
  EO Type. 
 \item To read and write variables, the  name of the variable without #rep# is used, 
       returning void to write, and returning the type of the variable to read. 
       #read# and #write# are used for properties, not for variables, as in #readGene#
       and #writeGene#. For instance, #fitness()# will read and write variable #repFitness#
 \item The extensions for headers should be #.h#, and for definitions, if there
	were any, #.cpp#. In this way, VC++ will be able to understand it perfectly. The name
	of the file should be {\em exactly the same} as the class it contains, or the
	main class it contains; for instance, the header file for the EO class should be
	called #EO.h#, including capitalizations and everything.
 \end{itemize}

  @name Portability issues

  EO should be kept as portable as possible, including at least
  UNIX and Win platforms, with as many compilers as possible. In order to do so, 
  try to follow these guidelines\begin{itemize}
\item Don´t use the standard IOSTREAM library include files, such as iostream.h or strstream.h.
	Instead, use the STL versions of the IO standard library, that is, <iostream> or
	<strstream>
\item For all others, use always standard STL headers, such as <functional> instead
	of <stl_function.h>, all <stl_*> headers are gcc-specific. <algorithm> is standard, but 
	<algo.h> is not.
\item Some other headers are UNIX specific: specifically, <values.h>, it does not work
	even on Win-egcs.
\item Use always public STL interfaces; never use private or protected variables or 
	functions, because they could change with the implementation.
\item VC++ requires void ctors in classes used to initialize a vector template. I think
	that is standard STL, but other implementations are not so strict.
\item {\em Initialization of static member variables}: it should be made {\em outside} 
the class definition, like this:\\
{\tt class foo {
	static int bar;
}
static foo::bar = 7;
} never like this: \\
{\tt class foo {
	static int bar = 7;
}
which produces an error in VC++, at least.
\end{itemize}
  */
//@{
	//@Include: ADT.h
#include <ADT.h>
	//@Include: EOGeneric.h
#include <EOGeneric.h>
    //@Include: EOSpecific.h
#include <EOSpecific.h>
    //@Include: EOutil.h
#include <EOutil.h>
//@}
