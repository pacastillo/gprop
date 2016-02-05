// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOREPLACERS_H
#define _EOREPLACERS_H


/** @name {\em Replacement algorithms}

Replacement take the old and the new population, eliminate those that are not needed, and 
leave the rest for the other generations

These classes will usually be used inside a EOAlgo.
@author GeNeura team
@version 0.0
@see EOAlgo
*/
//@{
#include "EOElimAll.h"
#include "EOElimWorst.h"
#include "EOCrowding.h"
    //@Include: EOElimAll.h EOElimWorst.h EOCrowding.h
//@}
#endif
