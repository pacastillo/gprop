
#include "AboutDialog.h"

static const gchar * credits[] = {
  "EOGI v0.5",
  "Evolutionary Objects",
  "Graphic Interface",
  "",
  " f(x) = x + abs( sin(32*pi*x) ) ",
  "x in [0,1]. The max is 1.98442447",
  "",
  "GeNeura Team",
  "http://kal-el.ugr.es"
};
static const gint num_credits_lines = 9;

AboutDialog::AboutDialog () : OK("OK"), canvas() {
  set_title ("About EOGI");
  set_usize (400, 200);
  // don't allow resizing
  set_policy ( false, false, false );

  get_vbox()->add( &canvas );
  get_action_area()->add( &OK );

  connect_to_method ( OK.clicked, this, &AboutDialog::ok_clicked );
	
  OK.show();
  canvas.show();
}

void AboutDialog::redraw (void) {
  canvas.clear();
  canvas.gc_save();

  canvas.set_foreground( (guint16)16384, (guint16)16384, (guint16)16384 );
  for ( int i = 0; i < num_credits_lines; ++i) {
    canvas.draw_aligned_string ( canvas.width()/2, 20+(i*15), credits[i], 0, 0, 0,0,0,0);
  }

  canvas.gc_restore();
  canvas.refresh();
}
