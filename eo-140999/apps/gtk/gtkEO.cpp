// Program to test several EO features

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <string>
#include <iostream>
#include <numeric> // for accumulate
#include <iterator>
#include <vector>

using namespace std;

// Standard C libraries for rand
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>
#include <generic/EOGOps.h>
#include <generic/EOEvalAll.h>
#include <generic/EOselect/EOTournament.h>
#include <generic/EOselect/EORandomBreed.h>
#include <generic/EOGenTerm.h>
#include <generic/EOselect/EOReplacers.h>
#include <generic/EOEasyGA.h>
#include <util/EOGrapher.h>

//Utils
#include <util/Parser.h>

// gtk includes
#include <math.h>
#include <gtk/gtk.h>
#include <time.h>

//
struct coord {
  unsigned x, y;
};

// Some useful typedefs
typedef EOBin<float> Chrom;

#include "riolo.h"    //Fitness class

/// Generation that plots stuff using gtk
template< class T >
class IdleGen {
  
#ifdef _MSC_VER
  typedef T::EOType EOType;
#else
  typedef typename T::EOType EOType;
#endif
public:

  /// Ctor
  IdleGen( EOPop<T>& _pop, EOEasyGen<T>& _gen, 
	   EOTerm<T>& _term, float _yscale )
    :population( _pop ), generation( _gen ), terminator (_term ),
     yScale( _yscale), values(), idx( 0 ) {};

  /// Dtor
  ~IdleGen() {};

  /** This is the function that can be called from gtk 
      @param data the Widget it's going to draw to */
  gint Repaint (gpointer data)
  {
    GtkWidget*	drawing_area = (GtkWidget *) data;
    GdkDrawable *drawable;
    
    // while the algorithm hasnot finished
    if ( terminator( population ) ) {

      /* Run the genetic algorithm part */
      generation( population );

      /* --- Get drawing area window --- */
      drawable = drawing_area->window;
      
      /* --- Clear Rectangle --- */
      gdk_draw_rectangle (drawable,
			  drawing_area->style->white_gc,
			  TRUE,
			  0, 0,
			  drawing_area->allocation.width,
			  drawing_area->allocation.height);
      
      // Generar aleatoriamente dos numeros
      coord thisCoord;
      thisCoord.x = idx++;
      EOType fitness = population[0].fitness();
      cout << fitness << endl;
      thisCoord.y = unsigned(ceil(fitness*yScale));

      values.push_back( thisCoord );
      gdk_draw_line( drawable, drawing_area->style->black_gc, 0, drawing_area->allocation.height, 0,  drawing_area->allocation.height);
      unsigned oldx = 0, oldy = 0;
      for ( vector<coord>::iterator i = values.begin(); 
	    i != values.end(); i++ ) {
	gdk_draw_line (drawable, drawing_area->style->black_gc, oldx, 
		       drawing_area->allocation.height-oldy, i->x, drawing_area->allocation.height-i->y );
	oldx = i->x;
	oldy = i->y;
      }
    }	    
    return (TRUE);
}


private:
  EOPop<T>& population;
  EOEasyGen<T>& generation;
  EOTerm<T>& terminator;
  float yScale;
  vector<coord> values;  
  unsigned idx;
};

IdleGen<Chrom>* fitPainter;

// Trick of the almendrication
gint Repaint (gpointer data) {
  return fitPainter->Repaint( data );
}


//____________________________________________________________________________________________________
/*
 * quit
 *
 * exit gtk message loop
 */
void quit ()
{
    gtk_exit (0);
}

//____________________________________________________________________________________________________

main( int argc, char** argv) {
  const unsigned chromSize = 12;
  const unsigned height = 200;
  const double RioloMax = 1.98442447185516357421875;

  Parser parser("pr-tourn-w-graph-1", "Demo of an easy Genetic algorithm with flexible algorithms\\
that uses Tournament selection and a population decoration -- graph ");

  parser.addParam("-s", "--size", "10", Param::INT, "Chromosome size");
  parser.addParam("-p", "--popsize", "100",  Param::INT, "Number of EOs in the population");
  parser.addParam("-r", "--range", "100", Param::FLOAT, "EO values range from -range/2 to range/2");
  parser.addParam("-g", "--generations", "100",
		  Param::INT, "Number of generations the GA will be run" ); 
  parser.addParam("-t", "--tournamentsize", "3", Param::INT, "Tournament size");

  unsigned size, 
    popSize, 
    range,
    numGenerations,
    tournamentSize;

  try {
    parser.parse(argc, argv);
    
    size = parser.getInt( "-s" );
    popSize = parser.getInt( "-p" );
    range = parser.getInt("-r");
    numGenerations = parser.getInt("-g");
    tournamentSize= parser.getInt("-t" );
  }
  catch (UException & e){
	cout << e.what() << endl;
  }
	
  srand( (unsigned)time( NULL ) );
  unsigned j;

  // useful typedefs
  EOPop< Chrom > initPop;
  Uniform< float > u( 0, 1);
  for ( j = 0; j < popSize; j ++ ) {
    Chrom* EOIA = new Chrom( "", 1, chromSize);	
    for ( unsigned i = 0; i < chromSize; i ++ ) {
      (*EOIA)[i] = u()>0.5?1:0;
    }
    initPop.push_back( *EOIA );	  
    cout << *EOIA << endl;
  }

  // Evaluate
  Riolo thisEvalFunc;
  
  EOEvalAll<Chrom> thisEval( thisEvalFunc );

  // Go ahead to next generation
  EOTournament< Chrom > coach( tournamentSize );

  // Replacement
  EOElimAll< Chrom > popKiller;
	
  // And now breed
  EORandomBreed<Chrom> chaperon;

  //Add operators
  EOXOver2<Chrom> xOver( 1 ); 
  chaperon.addOp( &xOver );

  //Add mutation
  EOBitFlip<float> mutator( 1 );
  chaperon.addOp( &mutator );

  //Termination condition
  EOGenTerm< Chrom> ngenerations( numGenerations );
	
  //And now the population itself
  EOEasyGen<Chrom> 
    thisAlgorithm( thisEval, coach, chaperon, popKiller );

  // Declare the generational grapher
  fitPainter = new IdleGen<Chrom>( initPop, thisAlgorithm, ngenerations, 
				   height/RioloMax );

  // Evaluate the initial population
  thisEval( initPop );

  // Now the gtk stuff
  GtkWidget *window;
  GtkWidget *drawing_area;
  GtkWidget *vbox;
  
  /* --- Initialize GTK --- */
  gtk_init (&argc, &argv);
  
  /* --- Create a top level window --- */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  
  /* --- Get a packing box --- */
  vbox = gtk_hbox_new (FALSE, 0);
  
  /* --- Add packing box to window --- */
  gtk_container_add (GTK_CONTAINER (window), vbox);
  
  /* --- Make packing box visible --- */
  gtk_widget_show (vbox);
  
  /* --- Listen for the destroy --- */
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (quit), NULL);
  
  /* --- Create the drawing area  --- */
  drawing_area = gtk_drawing_area_new ();
  
  /* --- Set the size --- */
  gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), numGenerations, height);
  
  /* --- Add drawing area to packing box --- */
  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);
  
  /* --- Make drawing area visible --- */
  gtk_widget_show (drawing_area);
  
  /* --- Make the window visible --- */
  gtk_widget_show (window);
  
  /* --- Repaint every 1000ms (every second) --- */
  gtk_idle_add ( Repaint, (gpointer) drawing_area);
  
  /* --- Start gtk message loop --- */
  gtk_main ();

  return 0;
}
	
