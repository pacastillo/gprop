
#ifndef _AXISCANVAS_H
#define _AXISCANVAS_H

#include <gtk--.h>
#include <gtk--canvas.h>

#include <strstream>

const unsigned margenAxis = 20;

class AxisCanvas: public Gtk_Canvas {

 public:
  /// This is a ctor
  AxisCanvas( unsigned _margin = margenAxis)
    : Gtk_Canvas(), margin( _margin ), maxX(0), maxY(0), 
    x0(0), y0(0), divx(0), divy(0) {};
  
  ///
  ~AxisCanvas() {};

  ///
  void draw () {
    //*****************redibujar las graficas.*****************

    clear(); //cleans canvas...
    gc_save();
    
    set_foreground( (guint16)0, (guint16)0, (guint16)0 ); //black color
    set_line_width((gint)3); //3 pixels widthed lines
    draw_line( margin, margin , margin, height()-margin );
    draw_line( margin, height()-margin , width()-margin, height()-margin );
    printXLabel();
    printYLabel();
    gc_restore();
    refresh(); //refresh the canvas
    
  };
  
  // redefinition of this function, which computes ranges
  void lineTo( float x1, float y1 ) {
    maxX = ( x1 > maxX)?x1:maxX;
    maxY = ( y1 > maxX)?y1:maxY;

    divx =  ( (float)(width()-2*margin) / (maxX +1 ));
    divy =  ( (float)(height()-2*margin) / (maxY + 1));
    if ( x1 == 0 ) {
      draw_point( margin+x1*divx, height()- margin - y1*divy );
    } else {
      draw_line( x0, y0, margin+x1*divx, height()-margin-y1*divy);
    }
    x0 = margin+x1*divx;
    y0 = height()-margin-y1*divy;;
    
  }			 

  ///
  unsigned Margin() const { return margin;};

  ///
  void printXLabel() {
    strstream ssCad;
    ssCad << maxX << '\0'<< endl;
    set_foreground( (guint16)0, (guint16)0, (guint16)0 ); //black color
    draw_aligned_string (width()-margin , height()-margin/2, ssCad.str(), 1, 0);
  }

  ///
  void printYLabel() {
    strstream ssCad;
    ssCad << maxY << '\0' << endl;
    set_foreground( (guint16)0, (guint16)0, (guint16)0 ); //black color
    draw_aligned_string (0 , margin, ssCad.str(), 1, 0 );
  }

 private:
  unsigned margin;
  float maxX, maxY, x0, y0, divx, divy;
};

#endif
