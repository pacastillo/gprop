
#include "MiApp1.h"

MiApp1::MiApp1() :
  // Gtk_Window(GTK_WINDOW_TOPLEVEL) : not needed,
  // GTK_WINDOW_TOPLEVEL is the constructor arg's default value
  v_box( false, 0 ),
  m_box1(), // creates a box to pack widgets into
  m_button1("Run"),
  m_button2("Quit"),
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

  // Now when the button is clicked, we call the "callback" function
  // with a pointer to "button 1" as it's argument
  connect_to_method(m_button1.clicked, this, &run );

  // instead of gtk_container_add, we pack this button into the invisible
  // box, which has been packed into the window.
  // note that the pack_start default arguments are true, true, 0
  m_box1.pack_start(m_button1, false, true, 0);
  
  // always remember this step, this tells GTK that our preparation
  // for this button is complete, and it can be displayed now.
  m_button1.show();

  // call the same callback function with a different argument,
  // passing a pointer to "button 2" instead.
  connect_to_method(m_button2.clicked, Gtk_Main::instance(), &Gtk_Main::quit );

  m_box1.pack_start(m_button2, false, true, 0);

  // The order in which we show the buttons is not really important,
  // but I recommend showing the window last, so it all pops up at
  // once.
  m_button2.show();

  v_box.pack_end(aCanvas, true, true, 0);
 
  // now show
  show_all();
  aCanvas.draw();
}

//
void MiApp1::run()
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


