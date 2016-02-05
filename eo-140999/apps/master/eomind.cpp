// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
/* RandMM.cpp
 Random algorithm that plays mastermind
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


#include "mmMatchString.h" // For matching strings
#include "mmGenAlgo.h"     // For the genetic algorithm
#include "rules.h"


//-----------------------------------------------------------------------------
// Gustavo
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>  // srand, atof
#include <time.h>    // time
#include <unistd.h>
#include <GL/glut.h>
#include <EO++.h>

//-----------------------------------------------------------------------------

mmGenAlgo* anAlgo;
match hints;
unsigned posiciones = 6;
unsigned colores = 6;
string guess;
mmMatchString secretCode;

//-----------------------------------------------------------------------------

float min_x = 111, max_x = 666, min_y = 111, max_y = 666;

//-----------------------------------------------------------------------------

static void Init( void )
{
  glShadeModel(GL_FLAT);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glPointSize(2);

  // arguments
  string smin_x, smax_x, smin_y, smax_y;
  for (unsigned i = 0; i < posiciones; ++i)
    if (i < posiciones / 2)
      {      
	smin_x += "1";
	smax_x += (char)('0' + colores);
      }
    else
      {
	smin_y += "1";
	smax_y += (char)('0' + colores);
      }
  min_x = atof(smin_x.c_str());
  max_x = atof(smax_x.c_str());
  min_y = atof(smin_y.c_str());
  max_y = atof(smax_y.c_str());
  cout << "min_x = " << min_x << " max_x = " << max_x
       << "min_y = " << min_y << " max_y = " << max_y << endl; 

  anAlgo->initAlgo();
}

//-----------------------------------------------------------------------------

static void Idle( void )
{
  // Main Loop
  int thisScore;
  string guess;
  
  const EOPop<mmChrom>& thisPop = anAlgo->getPop();
  const rules& theseRules = anAlgo->getRules();
  
  if ( hints.full < posiciones ) {
    anAlgo->singleStep();
    thisScore = -theseRules.dist( thisPop[0] );
    if ( !thisScore ) {
      guess = thisPop[0];
      hints = secretCode.check( guess );
      anAlgo->setLast( guess );
      anAlgo->gimmeAnswers( hints.full, hints.color );
      cout << "String " << secretCode << "\nString " << guess <<
	" Full " << hints.full <<
	" Partial " << hints.color << endl;
      anAlgo->initAlgo();
    }
    glutPostRedisplay();
  }
}

//-----------------------------------------------------------------------------

static void Display( void )
{
  const EOPop<mmChrom>& pop = anAlgo->getPop();
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  glPushMatrix();
  
  glBegin(GL_POINTS);
  
  // all chrom's
  for (EOPop<mmChrom>::const_iterator chrom = pop.begin();
       chrom != pop.end();
       ++chrom)
    {
      string sx, sy;
      for (unsigned i = 0; i < chrom->size(); ++i)
        if (i < chrom->size() / 2)
          sx += (*chrom)[i];
        else
          sy += (*chrom)[i];
      int fitness = chrom->fitness();
      glColor3f( 1.0 - fitness/(-31.0), 
		 1.0 - fitness/(-31.0), 
		 1.0 - fitness/(-31.0));
      glVertex2f(atof(sx.c_str()), atof(sy.c_str()));
    }
  
  // best chrom
  glColor3f(1.0, 0.0, 0.0);
  string sx, sy;
  for (unsigned i = 0; i < pop[0].size(); ++i)
    if (i < pop[0].size() / 2)
      sx += pop[0][i];
    else
      sy += pop[0][i];
  glVertex2f(atof(sx.c_str()), atof(sy.c_str()));

  // solution
  glColor3f(0.0, 0.0, 1.0);
  sx = sy = "";
  for (unsigned i = 0; i < secretCode.size(); ++i)
    if (i < secretCode.size() / 2)
      sx += secretCode[i];
    else
      sy += secretCode[i];
  glVertex2f(atof(sx.c_str()), atof(sy.c_str()));

  glEnd();
  
  glPopMatrix();
  
  glutSwapBuffers();
}

//-----------------------------------------------------------------------------

static void Reshape( int width, int height )
{
  glViewport( 0, 0, width, height );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( min_x * 0.97, max_x * 1.02, min_y * 0.97, max_y * 1.02, -1, 1);
  glMatrixMode( GL_MODELVIEW );
}

//-----------------------------------------------------------------------------

static void Key( unsigned char key, int, int )
{
  switch (key) 
    {
    case 27:
      exit(0);
      break;
    }
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------

static void SpecialKey( int key, int, int )
{
  switch (key) 
    {
    case GLUT_KEY_UP:
      break;
    case GLUT_KEY_DOWN:
      break;
    case GLUT_KEY_LEFT:
      break;
    case GLUT_KEY_RIGHT:
      break;
    }
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------

main( int argc, char ** argv) {
  
  // Initialize random seed
  srand((unsigned)time( NULL ) );
  
  if ( argc <= 1) {
    cerr << "Usage : " << argv[0] 
	 << "[positions] [colors]";
    exit(1);
  }
  
  // arguments
  posiciones = atoi( argv[1] );
  colores = atoi( argv[2] );
  
  // Random/GA algorithm
  anAlgo= new mmGenAlgo( posiciones, colores );
    
  // Random string to search for
  randomGen( secretCode, posiciones, colores );
  
  // 
  guess = anAlgo->gimmeFirst();
  
  // Check against secret
  hints = secretCode.check( guess );
  anAlgo->gimmeAnswers( hints.full, hints.color );

  // OpenGL
  glutInit( &argc, argv );
  glutInitWindowSize( 400, 400 );
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutCreateWindow(argv[0]);
  Init();
  glutReshapeFunc( Reshape );
  glutKeyboardFunc( Key );
  glutSpecialFunc( SpecialKey );
  glutDisplayFunc( Display );
  glutIdleFunc( Idle );
  glutMainLoop(); 
  
  return 0;
}
