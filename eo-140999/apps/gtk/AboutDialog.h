// -*- C++ -*-

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <gtk--.h>
#include "gtk--canvas.h"

class AboutDialog : public Gtk_Dialog {
public:
  AboutDialog ();
  virtual ~AboutDialog (void) {}
  gint delete_event_impl(GdkEventAny * e) 
  { closed(); return true; }

  virtual void redraw();

  Signal0 closed;

private:
  Gtk_Button OK;
  Gtk_Canvas canvas;

  void ok_clicked () { closed(); } 
};

#endif

