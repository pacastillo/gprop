// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* mmOutput.h
 Classes to perform output from the Genetic MasterMind program
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
-----------------------------------------------------------------------------*/


#include "mmOutput.h"

#include <stdexcept>

// Ctor
mmOutput::mmOutput( mmOutput::outputType _t ) {
	switch ( _t ) {
	case STDOUT:
		realOutput = new mmSTDOutput;
		break;
	case CGI:
		realOutput = new mmCGIOutput;
		break;
	default:
		throw invalid_argument( "Invalid argument" );
	}
}


