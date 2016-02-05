#ifndef TDIALOGI_H
#define TDIALOGI_H

#include <gtk--.h>

class TDialogInit : public Gtk_Dialog {
public:
  TDialogInit(unsigned _max, 
	      unsigned *_generaciones, unsigned *_tamPop, 
	      unsigned *_numGenes, unsigned *_bitXgen,
	      unsigned *_rangeVal,
	      float *_rate, float *_select,
	      unsigned *_OkCancel);
  ~TDialogInit(){}

  gint delete_event_impl(GdkEventAny * e) 
    { closed(); return true; }  
  Signal0 closed;

private:
  unsigned *devG, *devP, *devNG, *devBG, *OkCancel;
  float *devR, *devS;
  unsigned max;


  Gtk_Button OK, Cancel;
             
  Gtk_HScale *ScaleG, *ScaleP, *ScaleNG, *ScaleBG, *ScaleR, *ScaleS;
  Gtk_Adjustment *AdjG, *AdjP, *AdjNG, *AdjBG, *AdjR, *AdjS;
  Gtk_VBox *ScalesBoxG, *ScalesBoxP, *ScalesBoxNG, *ScalesBoxBG, *ScalesBoxR, *ScalesBoxS;

  Gtk_Frame frameG, frameP, frameNG, frameBG, frameR, frameS;

  Gtk_VBox cajaG, cajaP, cajaNG, cajaBG, cajaR, cajaS;

  void ok_clicked ();
  void cancel_clicked ();

};

#endif
