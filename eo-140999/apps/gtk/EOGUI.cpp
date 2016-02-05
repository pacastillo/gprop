
#include "EOGUI.h"

EOGUI::EOGUI() :
  // Gtk_Window(GTK_WINDOW_TOPLEVEL) : not needed,
  // GTK_WINDOW_TOPLEVEL is the constructor arg's default value
  v_box( false, 0 ),
  m_box1(), // creates a box to pack widgets into
  bRun("Run"), bStep("Step"), bStop("Stop"), bQuit("Quit"),
  aCanvas(),
  aLine1(aCanvas, Gdk_Color("#00000000cd00")),
  aLine2(aCanvas, Gdk_Color("#ff0045000000"))
{

  // this is a new call, this just sets the title of our new window to
  // "Hello Buttons!"
  set_title("Draw on a Canvas");

  // sets the border width of the window.
  set_border_width(10);

  // Sets window size
  set_usize( 640, 400);

  // Create the vertical vbox
  add(&v_box);

  // put the box into the main window.
  v_box.pack_start(m_box1, false, true, 0);

  v_box.pack_start( bRun, false,true,1 );
  v_box.pack_start( bStep, false,true,1 );
  v_box.pack_start( bStop, false,true,1 );
  v_box.pack_start( bQuit, false,true,1 );

  bRun.show();  
  bStep.show(); 
  bStop.show();
  bQuit.show();
      
  v_box.pack_end(aCanvas, true, true, 0);
 
  // now show
  show_all();
  aCanvas.draw();
}

//
void EOGUI::run()
{
  srand( unsigned( time( time_t(0) ) ));
  for ( unsigned i = 0; i < 100; i ++ ) {
    float tmp = (int) (10.0*rand()/(RAND_MAX+1.0));
    float tmp2 = (int) (12.0*rand()/(RAND_MAX+1.0));
    // housekeeping
    aCanvas.clear();
    aCanvas.gc_save();
    aCanvas.draw();
    aLine1.addPoint( tmp );
    aLine1.draw();
    aLine2.addPoint( tmp2 );
    aLine2.draw();
    // housekeeping again
    aCanvas.gc_restore();
    aCanvas.refresh(); //refresh the canvas
    while ( Gtk_Main::instance()->events_pending() ) {
       Gtk_Main::instance()->iteration();
    }
  }
}


