#include <iostream>
#include <gtk--.h>

#include "AxisCanvas.h"
#include "CanvasLine.h"
#include "MiApp1.h"


//____________________________________________________________________

int main (int argc, char *argv[])
{
          
  // all GTK applications must have a gtk_main(). Control ends here
  // and waits for an event to occur (like a key press or mouse event).
  Gtk_Main myapp(&argc, &argv);

  MiApp1 miApp;

  myapp.run();
  return 0;
}
