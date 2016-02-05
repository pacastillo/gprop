#ifndef _CANVASLINE_H
#define _CANVASLINE_H

#include <algorithm> // for max_element

#include "AxisCanvas.h"


class CanvasLine {

 public:
  ///
  CanvasLine( AxisCanvas& _a, Gdk_Color _color)
    :points(), aCanvas( _a ), color( _color ) {};
  
  ///
  ~CanvasLine() {};

  ///
  void addPoint( float _p ) {
    points.push_back( _p );
  }

  ///
  void draw() const {

    // do it
    aCanvas.set_foreground( color.get_red(), color.get_green(), color.get_blue() ); //red
    for(unsigned i=0; i < points.size() ; i++) {
      aCanvas.lineTo(  i, points[i] );
    }

    // housekeeping again
    aCanvas.printXLabel();
    aCanvas.printYLabel();
    aCanvas.gc_restore();
    aCanvas.refresh(); //refresh the canvas
  };

 private:
  vector<float> points;
  AxisCanvas& aCanvas;
  Gdk_Color color;
};

#endif
