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

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <gdk/gdk.h>
#include <gdk/gdkprivate.h>

#include "gtk--canvas.h"

Gtk_Canvas::Gtk_Canvas() :  Gtk_DrawingArea(), buffer_(0),
  width_(0), height_(0), gc_(0),
  timeout_interval_(0), timeout_tag_(-1), idle_tag_(-1)
{
  // Make sure we get all of the events that Gtk_DrawingArea doesn't
  // usually see.
  gint mask = gtk_widget_get_events (GTK_WIDGET (gtkobject));
  mask |= GDK_EXPOSURE_MASK;
  mask |= GDK_POINTER_MOTION_MASK;
  mask |= GDK_BUTTON_PRESS_MASK;
  mask |= GDK_BUTTON_RELEASE_MASK;
  mask |= GDK_KEY_PRESS_MASK;
  mask |= GDK_ENTER_NOTIFY_MASK;
  mask |= GDK_LEAVE_NOTIFY_MASK;
  gtk_widget_set_events(GTK_WIDGET(gtkobject), mask);
}

Gtk_Canvas::~Gtk_Canvas()
{
  if (buffer_) gdk_pixmap_unref(buffer_);
  if (gc_) gdk_gc_destroy(gc_);
  while (!gc_stack_.empty()) {
    gdk_gc_destroy(gc_stack_.back());
    gc_stack_.pop_back();
  }
}

gint
Gtk_Canvas::expose_event_impl(GdkEventExpose* e)
{
  refresh(e->area.x, e->area.y, e->area.width, e->area.height);
  return Gtk_DrawingArea::expose_event_impl(e);
}

gint
Gtk_Canvas::configure_event_impl(GdkEventConfigure* c)
{
  if (buffer_)
    gdk_pixmap_unref(buffer_);

  buffer_ = gdk_pixmap_new(GTK_WIDGET(gtkobject)->window,
			   width_ = c->width, height_ = c->height,
			   -1);

  // start with a clean white background
  clear();

  // I explicitly turn of any clipping here.  Technically I guess I
  // shouldn't, but a few times I forgot to do so any got an
  // unexpected surprise when resizing a window.
  set_noclip();
  redraw();
  
  Gtk_DrawingArea::configure_event_impl(c);

  return 0;
}

void
Gtk_Canvas::refresh(gint x, gint y, gint w, gint h)
{
  set_noclip(); // See above comment about set_noclip().
  gdk_draw_pixmap(GTK_WIDGET(gtkobject)->window,
		  GTK_WIDGET(gtkobject)->style->fg_gc
		  [GTK_WIDGET_STATE (gtkobject)],
		  buffer_,
		  x,y,x,y,w,h);
}

///////////////////////////////////////////////////////////////////////////

/// Functions related to GCs and fonts

GdkGC*
Gtk_Canvas::gc()
{
  // By doing this "lazy allocation", we never have to worry about
  // creating the GC before we have been realized.
  if (gc_ == 0) 
    gc_ = gdk_gc_new(GTK_WIDGET(gtkobject)->window);
  return gc_;
}

void
Gtk_Canvas::gc_save()
{
  gc_stack_.push_back(gc_);
  gc_ = 0;
}

void
Gtk_Canvas::gc_clone()
{
  gc_save();
  gdk_gc_copy(gc(), gc_stack_.back());
}

void
Gtk_Canvas::gc_restore()
{
  if (gc_)
    gdk_gc_destroy(gc_);
  if (!gc_stack_.empty()) {
    gc_ = gc_stack_.back();
    gc_stack_.pop_back();
  } else
    gc_ = 0;
}

GdkFont*
Gtk_Canvas::font()
{
  GdkGCValues val;
  gdk_gc_get_values(gc(), &val);
  if (val.font == 0) {
    set_font("fixed");
    return font();  // If we don't have "fixed", this could get ugly.
  }
  return val.font;
}


///////////////////////////////////////////////////////////////////////////

/// Functions related to manipulating the GC

void
Gtk_Canvas::set_line_width(gint line_width)
{
  GdkGCPrivate* gc_private = (GdkGCPrivate*)gc();
  XGCValues xvalues;
  xvalues.line_width = line_width;
  XChangeGC(gc_private->xdisplay, gc_private->xgc, GCLineWidth, &xvalues);
}

void
Gtk_Canvas::set_line_style(GdkLineStyle line_style)
{
  GdkGCPrivate* gc_private = (GdkGCPrivate*)gc();
  XGCValues xvalues;
  xvalues.line_style = line_style;
  XChangeGC(gc_private->xdisplay, gc_private->xgc, GCLineStyle, &xvalues);
}

void
Gtk_Canvas::set_cap_style(GdkCapStyle cap_style)
{
  GdkGCPrivate* gc_private = (GdkGCPrivate*)gc();
  XGCValues xvalues;
  xvalues.cap_style = cap_style;
  XChangeGC(gc_private->xdisplay, gc_private->xgc, GCCapStyle, &xvalues);
}

void
Gtk_Canvas::set_join_style(GdkJoinStyle join_style)
{
  GdkGCPrivate* gc_private = (GdkGCPrivate*)gc();
  XGCValues xvalues;
  xvalues.join_style = join_style;
  XChangeGC(gc_private->xdisplay, gc_private->xgc, GCJoinStyle, &xvalues);
}

void
Gtk_Canvas::set_dashes(gint dash_offset, char* dash_list, gint dash_list_len) {
  GdkGCPrivate* gc_private = (GdkGCPrivate*)gc();
  XSetDashes(gc_private->xdisplay, gc_private->xgc,
	     dash_offset, dash_list, dash_list_len);
}

void
Gtk_Canvas::set_dashes(gint size)
{
  char d[1];
  d[0] = (char)size;
  set_dashes(0,d,1);
}

void
Gtk_Canvas::set_dashes(gint on, gint off)
{
  char d[2];
  d[0] = (char)on;
  d[1] = (char)off;
  set_dashes(0,d,2);
}

///////////////////////////////////////////////////////////////////////////

/// Drawing functions

void
Gtk_Canvas::draw_lines(GdkPoint* points, gint npoints, GdkGC* GC)
{
  GdkWindowPrivate* drawable_private = (GdkWindowPrivate*)buffer();
  GdkGCPrivate* gc_private = (GdkGCPrivate*)(GC ? GC : gc());

  XDrawLines( drawable_private->xdisplay, drawable_private->xwindow,
	      gc_private->xgc,
	      (XPoint*) points, npoints, CoordModeOrigin );
}

void
Gtk_Canvas::draw_polygon(bool filled, GdkPoint* points, gint npoints,
			 GdkGC* GC)
{
  GdkPoint* loc_points = points;
  gint loc_npoints = npoints;
  bool loc_alloc = false;

  if (npoints < 2) return;

  if (!filled && (points[0].x != points[npoints-1].x ||
		  points[0].y != points[npoints-1].y)) {
    ++loc_npoints;
    loc_points = new GdkPoint[loc_npoints];
    memcpy(loc_points, points, npoints * sizeof(GdkPoint));
    loc_points[loc_npoints-1].x = loc_points[0].x;
    loc_points[loc_npoints-1].y = loc_points[0].y;
  }

  gdk_draw_polygon(buffer(), GC ? GC : gc(), filled, loc_points, loc_npoints);

  if (loc_alloc) delete [] loc_points;
}

GdkPixmap*
Gtk_Canvas::copy_rectangle_to_pixmap(gint x, gint y, gint w, gint h, GdkGC* GC)
{
  GdkPixmap* pix = gdk_pixmap_new(GTK_WIDGET(gtkobject)->window, w, h, -1);
  gdk_draw_pixmap(pix, GC ? GC : gc(), buffer(), x, y, 0, 0, w, h);
  return pix;
}

void
Gtk_Canvas::draw_aligned_string(gint x, gint y, const gchar* str,
				gint x_align, gint y_align,
				gint x_adjust, gint y_adjust,
				GdkFont* FONT, GdkGC* GC) {
  if (FONT == 0) FONT = font();
  if (GC == 0) GC = gc();

  gint tw = gdk_string_width(FONT, str);
  if (x_align < 0)
    x -= tw + x_adjust;
  else if (x_align == 0)
    x -= tw/2;
  else
    x += x_adjust;

  gint th = font_height(FONT);
  if (y_align == 0)
    y += th/2;
  else if (y_align < 0)
    y -= y_adjust;
  else
    y += th + y_adjust;

  draw_string(x,y,str,FONT,GC);
}

void
Gtk_Canvas::draw_crooked_string(gint x, gint y, const gchar* str,
				double x_step, double y_step,
				GdkFont* FONT, GdkGC* GC)
{
  char sbuf[2];
  sbuf[1] = '\0';

  double xc=x, yc=y;

  while (*str) {
    sbuf[0] = *str;
    draw_string((gint)xc, (gint)yc, sbuf,
		FONT ? FONT : font(), GC ? GC : gc());
    xc += x_step * string_width(sbuf);
    yc += y_step * font_height(FONT ? FONT : font());
    ++str;
  }
}

//////////////////////////////////////////////////////////////////////////////

/// Timeout functions

void
Gtk_Canvas::activate_timeout_function(guint32 interval)
{
  if (timeout_tag_ != -1)
    terminate_timeout_function();
  continue_timeout_ = true;
  timeout_interval_ = interval;
  timeout_tag_ =
    gtk_timeout_add(timeout_interval_, canvas_timeout_function, this);
}

void
Gtk_Canvas::terminate_timeout_function()
{
  continue_timeout_ = false;
  if (timeout_tag_ != -1)
    gtk_timeout_remove(timeout_tag_);
  timeout_tag_ = -1;
}

void
Gtk_Canvas::activate_idle_function()
{
  if (idle_tag_ != -1)
    return;
  continue_idle_ = true;
  idle_tag_ = gtk_idle_add(canvas_idle_function, this);
}

void
Gtk_Canvas::terminate_idle_function()
{
  if (idle_tag_ == -1)
    return;
  continue_idle_ = false;
  gtk_idle_remove(idle_tag_);
  idle_tag_ = -1;
}

gint
canvas_timeout_function(gpointer c)
{
  Gtk_Canvas* canv = (Gtk_Canvas*)c;
  canv->timeout_function();
  return canv->continue_timeout_;
}

gint
canvas_idle_function(gpointer c)
{
  Gtk_Canvas* canv = (Gtk_Canvas*)c;
  canv->idle_function();
  return canv->continue_idle_;
}


