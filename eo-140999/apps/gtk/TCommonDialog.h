#ifndef TDIALOGI_H
#define TDIALOGI_H

#include <gtk--.h>

class TCommonDialog : public Gtk_Dialog {
public:
  /// Ctor
  TCommonDialog(unsigned _max, 
	      unsigned& _generaciones, unsigned& _tamPop, 
	      unsigned& _numGenes, unsigned& _bitXgen,
	      float& _rate, 
	      unsigned& _OkCancel, bool _init = true);

  ///
  ~TCommonDialog(){}

  ///
  gint delete_event_impl(GdkEventAny * e) 
    { closed(); return true; }  
  Signal0 closed;

private:
  unsigned &devG, &devP, &devNG, &devBG, &OkCancel;
  float &devR;
  unsigned max;
  bool init;// true if it is the initial dialog; variables change

  Gtk_Button OK, Cancel;
             
  Gtk_HScale *ScaleG, *ScaleP, *ScaleNG, *ScaleBG, *ScaleR;
  Gtk_Adjustment *AdjG, *AdjP, *AdjNG, *AdjBG, *AdjR;
  Gtk_VBox *ScalesBoxG, *ScalesBoxP, *ScalesBoxNG, *ScalesBoxBG, *ScalesBoxR;

  Gtk_Frame frameG, frameP, frameNG, frameBG, frameR;

  Gtk_VBox cajaG, cajaP, cajaNG, cajaBG, cajaR;

  void ok_clicked ();
  void cancel_clicked ();

};

#endif
