// This is -*- C++ -*-

/* 
 * A (primitive) canvas class for the Gtk-- toolkit, v0.3alpha
 *
 * Copyright (C) 1998 Jon E. Trowbridge <trow@emccta.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef __GTK_CANVAS_H
#define __GTK_CANVAS_H

#include <string>
#include <list>
#include <iostream.h>

#include <gtk--.h>
#include <gdk/gdk.h>

const unsigned angPerCircle=360;
const unsigned extrangeXCte=64;


class Gtk_Canvas : public Gtk_DrawingArea {
public:

  Gtk_Canvas();
  virtual ~Gtk_Canvas();

  virtual gint expose_event_impl(GdkEventExpose*);
  virtual gint configure_event_impl(GdkEventConfigure*);

  ///////////////////////////////////////////////////////////////////////

  // buffer() allows us to directly access our backing pixmap.
  GdkPixmap* buffer() { return buffer_; }
  gint width() { return width_; }
  gint height() { return height_; }

  /*
    The refresh() routines copy all or part of our backing pixmap
    to the screen.  Until it is called, things drawn on the canvas
    will not be visible.
  */
  virtual void refresh(gint x, gint y, gint w, gint h);
  void refresh() { refresh(0, 0, width(), height()); }

  ///////////////////////////////////////////////////////////////////////

  /*
    Each Canvas keeps a "current GC" that is the default for any
    drawing functions.

    GdkGC* gc()
    Return the current GC.

    void gc_save()
    Push the current GC onto the stack, and replace the current GC
    with a fresh one.

    void gc_clone()
    Push a copy of the current GC onto the stack.  This has no effect
    on the current GC.

    void gc_restore()
    Pop the top GC off of the stack and make it the current GC.
    If the GC stack is empty, replace the current GC with a fresh one.
  */

  GdkGC* gc();
  void gc_save();
  void gc_clone();
  void gc_restore();

  /*
    A font specification is embedded in every GC.  These routines let
    you access the current GC's font info and set fonts by name.

    Programs like "xfontsel" and "xlsfonts" will show you the names
    of the various fonts available to you.
  */

  GdkFont* font();
  void set_font(const gchar* s) { gdk_gc_set_font(gc(), gdk_font_load(s)); }

  ///////////////////////////////////////////////////////////////////////

  /*
    You can set the foreground and background color of the current 
    GC any of four ways:
    (1) Though a GdkColor color object.  In this case, it is sufficient
        to set the .red, .green and .blue values --- the routine will take
        care of setting .pixel properly.
    (2) By name, as in "red" or "chartreuse3".  Color names are defined
        in the file rgb.txt.  On my system, that file lives in /usr/lib/X11.
    (3) By specifying the integer RGB components.  A typical X gotcha ---
        Intensity ranges go 0 - 65535, not 0 - 255.
    (4) By specifying floating-point RGB components.  In this case,
        we rescale so that intensities go from 0.0 to 1.0. 
  */

  void set_foreground(GdkColor& c) {
    gdk_color_alloc(gdk_colormap_get_system(), &c);
    gdk_gc_set_foreground(gc(), &c);
  }
  
  void set_foreground(const gchar* spec) {
    GdkColor c;
    gdk_color_parse(spec, &c);
    set_foreground(c);
  }

  void set_foreground(guint16 r, guint16 g, guint16 b) {
    GdkColor c;
    c.red = r;
    c.green = g;
    c.blue = b;
    set_foreground(c);
  }

  void set_foreground(double r, double g, double b) {
    set_foreground((guint16)(r*65535),(guint16)(g*65535),(guint16)(b*65535));
  }

  void set_background(GdkColor& c) {
    gdk_color_alloc(gdk_colormap_get_system(), &c);
    gdk_gc_set_background(gc(), &c);
  }

  void set_background(const gchar* spec) {
    GdkColor c;
    gdk_color_parse(spec, &c);
    set_background(c);
  }

  void set_background(guint16 r, guint16 g, guint16 b) {
    GdkColor c;
    c.red = r;
    c.green = g;
    c.blue = b;
    set_background(c);
  }

  void set_background(double r, double g, double b) {
    set_background((guint16)(r*65535),(guint16)(g*65535),(guint16)(b*65535));
  }

  ///////////////////////////////////////////////////////////////////////

  /*
    Here are some more routines for manipulating our current GC.
  */

  void set_clip_origin(gint x, gint y) { gdk_gc_set_clip_origin(gc(),x,y); }
  void set_clip_mask(GdkBitmap* mask) { gdk_gc_set_clip_mask(gc(),mask); }
  void set_clip_rectangle(gint x, gint y, gint w, gint h) {
    GdkRectangle rect;
    rect.x = x;
    rect.y = y;
    rect.width = w;
    rect.height = h;
    gdk_gc_set_clip_rectangle(gc(), &rect);
  }
  void set_noclip() { gdk_gc_set_clip_mask(gc(), 0); }

  // Drawing Functions: GDK_COPY, GDK_INVERT, GDK_XOR
  void set_function(GdkFunction func) { gdk_gc_set_function(gc(), func); }

  // Fill Styles: GDK_SOLID, GDK_TILED, GDK_STIPPLED, GDK_OPAQUE_STIPPLED
  void set_fill_style(GdkFill fill) { gdk_gc_set_fill(gc(), fill); }

  void set_tile(GdkPixmap* tile) { gdk_gc_set_tile(gc(), tile); }
  void set_stipple(GdkPixmap* stip) { gdk_gc_set_stipple(gc(), stip); }
  void set_ts_origin(gint x, gint y) { gdk_gc_set_ts_origin(gc(), x, y); }

  // Subwindow modes: GDK_CLIP_BY_CHILDREN, GDK_INCLUDE_INFERIORS
  void set_subwindow(GdkSubwindowMode mode) { 
    gdk_gc_set_subwindow(gc(), mode);
  }

  void set_exposures(gint exp) { gdk_gc_set_exposures(gc(), exp); }

  void set_line_width(gint line_width);
  
  // Line Styles: GDK_LINE_SOLID, GDK_LINE_ON_OFF_DASH, GDK_LINE_DOUBLE_DASH
  void set_line_style(GdkLineStyle line_style);

  // Cap Styles: GDK_CAP_NOT_LAST, GDK_CAP_BUTT, GDK_CAP_ROUND,
  //             GDK_CAP_PROJECTING
  void set_cap_style(GdkCapStyle cap_style);

  // Join Styles: GDK_JOIN_MITER, GDK_JOIN_ROUND, GDK_JOIN_BEVEL
  void set_join_style(GdkJoinStyle join_style);

  // If you want to set everything at one, you can use this...
  void set_line_attributes(gint line_width,
			   GdkLineStyle line_style,
			   GdkCapStyle cap_style,
			   GdkJoinStyle join_style) {
    gdk_gc_set_line_attributes(gc(), line_width, line_style,
			       cap_style, join_style);
  }

  void set_dashes(gint dash_offset, char* dash_list, gint dash_list_length);

  // Here are the two cases that you use most of the time:
  void set_dashes(gint size); // on and off have same size
  void set_dashes(gint on, gint off);
    
  ///////////////////////////////////////////////////////////////////////

  /*
    Each of these drawing routines can take a pointer to a GdkGC as the
    final argument.  If none is provided, the canvas' current GC is used.
  */

  void draw_point(gint x, gint y, GdkGC* GC = 0) {
    gdk_draw_point(buffer(), GC ? GC : gc(), x, y);
  }

  void draw_line(gint x0, gint y0, gint x1, gint y1, GdkGC* GC = 0) {
    gdk_draw_line(buffer(), GC ? GC : gc(), x0, y0, x1, y1);
  }

  void draw_lines(GdkPoint* points, gint npoints, GdkGC* GC = 0);

  // draw a rectangle, specifying the (x,y) of the upper left-hand corner
  // and the width and height.
  void draw_rectangle(bool filled, gint x, gint y, gint w, gint h, 
		      GdkGC* GC = 0) {
    gdk_draw_rectangle(buffer(), GC ? GC : gc(), filled, x, y, w, h);
  }

  // draw a rectangle, specifying the (x,y) of two of the opposite corners.
  void draw_rectangle_coor(bool filled, gint x0, gint y0, gint x1, gint y1,
			   GdkGC* GC = 0) {
    draw_rectangle(filled,
		   x0 < x1 ? x0 : x1, y0 < y1 ? y0 : y1,
		   1+abs(x1-x0), 1+abs(y1-y0), GC);
  }

  /*
    Annoying X-ism: When specifying the angle for an arc, it has to be given
    as degrees*64.  Therefore an arc that sweeps from 0 to 180 degrees
    should have a1=0, a2=11530 (=180*64).
  */
  void draw_arc(bool filled, gint x, gint y, gint w, gint h, gint a1, gint a2,
		GdkGC* GC = 0) {
    gdk_draw_arc(buffer(), GC ? GC : gc(), filled, x, y, w, h, a1, a2);
  }

  // This is usually what I want to do when I call draw_arc() anyway...
  void draw_circle(bool filled, gint x, gint y, guint r, GdkGC* GC = 0) {
    draw_arc(filled,x-r,y-r,2*r,2*r,0,angPerCircle*extrangeXCte,GC);
  }

  void draw_polygon(bool filled, GdkPoint* points, gint npoints, GdkGC* GC=0);

  GdkPixmap* copy_rectangle_to_pixmap(gint x, gint y, gint w, gint h,
				      GdkGC* GC=0);

  void paste_pixmap(GdkPixmap* pix, gint x, gint y, GdkGC* GC=0) {
    gdk_draw_pixmap(buffer(), GC ? GC : gc(), pix, 0, 0, x, y, 
		    ((_GdkWindowAttr*)pix)->width, 
		    ((_GdkWindowAttr*)pix)->height);
  }


  ///////////////////////////////////////////////////////////////////////



  ///////////////////////////////////////////////////////////////////////

  /*
    Here are some routines for drawing text.  Here you can also
    optionally specify another font to override the font that is
    set in the current GC.
  */

  void draw_string(gint x, gint y, const gchar* str, 
		   GdkFont* FONT = 0, GdkGC* GC = 0) {
    gdk_draw_string(buffer(), FONT ? FONT : font(), GC ? GC : gc(), x, y, str);
  }

  /*
    Normally when drawing text, the lower left corner of the first
    character is placed at the given (x,y) coordinate.  This routine
    lets you control the alignment of the text around the (x,y) coordinate.

    Alignment is specified as follows:
    If x_align < 0, the right edge of the text string is at the given x.
    If x_align > 0, the left edge of the text string is at the given x.
    if x_align = 0, the string is horizontally centered around the given x.

    If y_align < 0, the bottom edge of the text string is at the given y.
    If y_align > 0, the top edge of the text string is at the given y.
    If y_align = 0, the string is vertically centered around the given y.

    x_adjust and y_adjust allows the text to be shifted the given number
    of pixels in the direction of the alignment.  (So if x_align < 0
    and x_adjust = 10, the right edge of the text is set 10 pixels to left
    of the given x.)

  */
  void draw_aligned_string(gint x, gint y, const gchar* str,
			   gint x_align, gint y_align,
			   gint x_adjust = 0, gint y_adjust = 0,
			   GdkFont* FONT = 0, GdkGC* GC = 0);

  // We now have string, so we should use it!
  void draw_string(gint x, gint y, const string& str, 
		   GdkFont* FONT = 0, GdkGC* GC = 0) {
    draw_string(x,y,str.c_str(),FONT,GC);
  }

  void draw_aligned_string(gint x, gint y, const string& str,
			   gint x_align, gint y_align,
			   gint x_adjust = 0, gint y_adjust = 0,
			   GdkFont* FONT = 0, GdkGC* GC = 0) {
    draw_aligned_string(x,y,str.c_str(),
			x_align,y_align,
			x_adjust,y_adjust,
			FONT,GC);
  }

  void draw_crooked_string(gint x, gint y, const gchar* str,
			   double x_step, double y_step,
			   GdkFont* FONT = 0, GdkGC* GC = 0);

  void draw_crooked_string(gint x, gint y, const string& str,
			   double x_step, double y_step,
			   GdkFont* FONT = 0, GdkGC* GC = 0) {
    draw_crooked_string(x,y,str.c_str(), x_step, y_step, FONT, GC);
  }
			   

  // These two are trivial but come in handy...
  gint string_width(const gchar* s, GdkFont* FONT = 0) {
    return gdk_string_width(FONT ? FONT : font(), s);
  }
  gint font_height(GdkFont* FONT = 0) {
    if (FONT == 0) FONT = font();
    return FONT->ascent + FONT->descent;
  }

  ///////////////////////////////////////////////////////////////////////

  /*
    If you want your canvas to have a background that is anything other
    than solid white, redefine this function in a derived class.
  */
  virtual void clear() {
    draw_rectangle(true,0,0,width(),height(),
		   GTK_WIDGET(gtkobject)->style->white_gc);
  }
  
  /*
    This redraw() function is called when the widget is created and
    whenever the widget is resized.

    Note: You don't need to call refresh() explicitly at the end of your
    redraw() routine.  That happens automatically.
  */

  virtual void redraw() { } // does nothing by default

  ///////////////////////////////////////////////////////////////////////

  /*
    To create a canvas with a "timeout function" (a function that gets
    called every x milliseconds), simply:
    - Create a derived class where timeout_function() is defined to
      actually do something.
    - Call activate_timeout_function(x).  (Where x is the period in
      milliseconds, as above.)

    To stop timeout_function() from being executed, call
    terminate_timeout_function().  The time interval between calls
    to timeout_function() can be changed at any time by using
    activate_timeout_function() again.
  */

  virtual void timeout_function() { } // does nothing by default
  void activate_timeout_function(guint32 interval);
  void terminate_timeout_function();
  bool timeout_function_is_active() { return timeout_tag_ != -1; }

  /*
    To create a class with an "idle function" (a function that gets
    called whenever nothing else is happening), simply:
    - Create a derived class where idle_function() is defined to
      actually do something.
    - Call activate_idle_function().

    To stop the idle function from being executed, call
    terminate_idle_function().  A terminated idle function can be
    activated again with another call to activate_idle_function().
    (Hmmm... this all sure sounds familiar...)
  */

  virtual void idle_function() { } // does nothing by default
  void activate_idle_function();
  void terminate_idle_function();
  bool idle_function_is_active() { return idle_tag_ != -1; }

private:

  friend gint canvas_timeout_function(gpointer);
  friend gint canvas_idle_function(gpointer);

  GdkPixmap* buffer_;
  gint width_, height_;
  GdkGC* gc_;

  // This should be a stack<> and not a list<>, but this makes life
  // easier for people stuck with gcc 2.7
  list<GdkGC*> gc_stack_;

  guint32 timeout_interval_;
  gint timeout_tag_;
  bool continue_timeout_;

  gint idle_tag_;
  bool continue_idle_;
};

#endif // __GTK_CANVAS_H






