#include "TWindowMenu.h"

TWindowMenu::TWindowMenu (void) : 
  ejecutar("Run Genetic A."), paso("Run one step"), exit("Exit"),
  inicial("Init Pop."), prefs("Preferences"), about("About"),
  file_menu(), file_item("Genetic A."),
  prefs_menu(), prefs_item("Parameters"),
  help_menu(), help_item("Help")
{
  // Put all menus on bar
  prepend(file_item);

  append(prefs_item);

  gtk_menu_item_right_justify( help_item.gtkobj() );
  append(help_item);

  // set up game menu
  file_menu.append(ejecutar);
    //ejecutar.show();
  file_menu.append(paso);
    //paso.show();
  file_menu.append(exit);
   exit.show();
  file_item.set_submenu(file_menu);
  file_item.show();


  //prefs menu
  prefs_menu.append(inicial);
   inicial.show();
  prefs_menu.append(prefs);
    //prefs.show();
  prefs_item.set_submenu(prefs_menu);
  prefs_item.show();


  // Help
  help_menu.append(about);
   about.show();
  help_item.set_submenu(help_menu);
  help_item.show();
}
