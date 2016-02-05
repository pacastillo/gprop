// -*- C++ -*-

#ifndef TWMENU_H
#define TWMENU_H

#include <gtk--.h>

class TWindowMenu : public Gtk_MenuBar {
public:
  TWindowMenu (void);
  virtual ~TWindowMenu (void) {};

  Gtk_MenuItem ejecutar, paso;
  Gtk_MenuItem exit;
  Gtk_MenuItem inicial, prefs;
  Gtk_MenuItem about;

private:
  Gtk_Menu file_menu;
  Gtk_MenuItem file_item;

  Gtk_Menu prefs_menu;
  Gtk_MenuItem prefs_item;

  Gtk_Menu help_menu;
  Gtk_MenuItem help_item;
};

#endif

