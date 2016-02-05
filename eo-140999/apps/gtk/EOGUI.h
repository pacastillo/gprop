#ifndef _MIAPP1_H
#define _MIAPP1_H

#include <gtk--.h>
#include "AxisCanvas.h"
#include "CanvasLine.h"



class EOGUI : public Gtk_Window
{

public:
  EOGUI();
  
  /// run
  void run();

  ///
  gint delete_event_impl(GdkEventAny*) { 
    Gtk_Main::instance()->quit(); return 0; 
  }

  ///
  void quit( ) { 
    Gtk_Main::instance()->quit();
  }

private:
  Gtk_VBox v_box;
  Gtk_HButtonBox m_box1;
  Gtk_Button bRun, bStep, bStop, bQuit;
  AxisCanvas aCanvas;
  CanvasLine aLine1, aLine2;
};

#endif
