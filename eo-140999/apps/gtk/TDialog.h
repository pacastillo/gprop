#ifndef TDIALOG_H
#define TDIALOG_H

#include <gtk--.h>

class TDialog : public Gtk_Dialog {
public:
  TDialog(unsigned _max,  
	  unsigned *_generaciones, int *_tamPop,
	  float *_rate, float *_select,
	  unsigned *_OkCancel);
  ~TDialog(){}

  gint delete_event_impl(GdkEventAny * e) 
    { closed(); return true; }  
  Signal0 closed;

private:
  unsigned *devG, *OkCancel;
  int *devP;
  float *devR, *devS;
  unsigned max;


  Gtk_Button OK, Cancel;
             
  Gtk_HScale *ScaleG, *ScaleP, *ScaleR, *ScaleS;
  Gtk_Adjustment *AdjG, *AdjP, *AdjR, *AdjS;
  Gtk_VBox *ScalesBoxG, *ScalesBoxP, *ScalesBoxR, *ScalesBoxS;

  Gtk_Frame frameG, frameP, frameR, frameS;

  Gtk_VBox cajaG, cajaP, cajaR, cajaS;


  void ok_clicked ();
  void cancel_clicked ();

};

#endif
