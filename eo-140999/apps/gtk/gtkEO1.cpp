#include <gtk--.h>
#include "TWindow.h"

#include <iostream.h>

// Standard C libraries for rand..
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

         
main(int argc, char **argv) {
  // Initialize random Gen
  srand( (unsigned)time( NULL ) );
  
  //initialize the aplication and the main window; run the app.
  Gtk_Main m( &argc, &argv );
  TWindow<eoFloat> g( m );
  m.run();
  
  return 0;
}
