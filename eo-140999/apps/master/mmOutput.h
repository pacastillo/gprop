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


#ifndef _mmOutput_H
#define _mmOutput_H

#include <string>
#include <iostream>

using namespace std;

/** Class that makes mastermind output device-independent
*/
class mmOutput {
public:
	typedef enum { STDOUT, CGI } outputType;

	/// Ctor
	mmOutput( outputType _t );

	/// Dtor
	virtual ~mmOutput() { 
		if ( realOutput  )
			delete realOutput;
	};

	/// 
	virtual void printHeader() const {
		realOutput->printHeader();
	}

	/// 
	virtual void printFooter() const {
		realOutput->printFooter();
	}
	
	/// 
	virtual void printCombination( const string& _str, unsigned _black, 
		unsigned _white) const {
		realOutput->printCombination( _str, _black, _white);
	}

	/// 
	virtual void printMsg( const string& _msg) const {
		realOutput->printMsg( _msg );
	}
	
	/// 
	virtual void printError( const string& _msg) const {
		realOutput->printError( _msg );
	}

protected:
	mmOutput(): realOutput( 0 ) {};

private:
	mmOutput* realOutput;
};

//_________________________________________________________________________


/** Clas that makes mastermind output device-independent
*/
class mmSTDOutput: public mmOutput {
public:
	/// Ctor
	mmSTDOutput():mmOutput() {};

  ///Dtor
  ~mmSTDOutput(){};

	/// 
	virtual void printHeader() const {
		cout << "Genetic Mastermind 1.5 by Geneura Team " << endl
			<< "GMM comes with ABSOLUTELY NO WARRANTY; for details view gpl.html" << endl
			<< "or gpl.txt. This is free software, and you are welcome to distribute " << endl
			<< "it under certain circunstances; read the above files for details " << endl;
	}

	/// 
	virtual void printFooter() const {
		cout << "\nFinished" << endl;
	}
	
	/// 
	virtual void printCombination( const string& _str, unsigned _black, 
		unsigned _white) const {
		cout << "\nString " << _str;
		if ( _black )
			cout <<" Black " << _black;
		if ( _white )
			cout << " White " << _white;
		cout << endl;
	}

	/// 
	virtual void printMsg( const string& _msg) const {
		cout<< _msg << endl;
	}
	
	/// 
	virtual void printError( const string& _msg) const {
		cerr << _msg << endl;
	}

};

/** Clas that makes mastermind output device-independent
*/
class mmCGIOutput: public mmOutput {
public:
	/// Ctor
	mmCGIOutput():mmOutput() {};

  	/// Dtor
	~mmCGIOutput() {};

	/// 
	virtual void printHeader() const {
		cout << "Content-type:text/html" << endl << endl;
		// Print the web page
		cout << "<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\
			<html>\
			<head>\
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\
			<meta name=\"Author\" content=\"jmerelo\">\
			<meta name=\"GENERATOR\" content=\"Mozilla/4.5 [en] (WinNT; I) [Netscape]\">\
			<title>Master mind solution</title>\
			</head>\
			<body>\
			<h1>Mastermind game by GA</h1>";
	}

	/// 
	virtual void printFooter() const {
		cout << "</body></html>\n";
	}
	
	/// 
	virtual void printCombination( const string& _str, unsigned _black, 
		unsigned _white) const {
		const unsigned pegWidth=40;
		const unsigned pegSeparation = 5;
		const unsigned littlePegWidth = pegWidth/2;
		unsigned cellLength = _str.size()*(pegWidth+littlePegWidth+pegSeparation+15);
		cout << "<table NOBORDER  WIDTH=" << cellLength << " BACKGROUND=/images/fondo.gif align=center>\
																										 <tr>\
																										 <td align=left width=66%>";
		
		// Print combination
		unsigned i;
		for (  i = 0; i < _str.length(); i ++ ) {
			cout << "<img src=/images/bola" << _str[i] << ".gif hspace= "<<  pegSeparation << " width = " << pegWidth << " alt=color" <<
				_str[i] << ">";
		}
		cout << "</td><td width=33% valign=middle> ";
		// Now black and white
		for ( i = 0; i < _black; i ++ ) {
			cout << "<img src=/images/negra.gif width= " << littlePegWidth << " alt=[black]>";
		}
		
		for ( i = 0; i < _white; i ++ ) {
			cout << "<img src=/images/blanca.gif width= " << littlePegWidth << " alt=[white]>";
		}
		cout << "</td></tr></table>" << endl;
	}

	/// 
	virtual void printMsg( const string& _msg) const {
		cout<< _msg << "<br>" << endl;
	}
	
	/// 
	virtual void printError( const string& _msg) const {
		printMsg( _msg );
		printFooter();
	}

};

#endif
