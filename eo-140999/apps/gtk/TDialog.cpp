#include "TDialog.h"

TDialog::TDialog(unsigned _max,
		 unsigned *_generaciones, int *_tamPop,
		 float *_rate, float *_select,
		 unsigned *_OkCancel) :
  OK("OK"), Cancel("Cancel"), 
  frameG("Number of Generations"), cajaG(false, 5),
  frameP("Initial Pop. Size"), cajaP(false, 5),
  frameR("Rate to kill"), cajaR(false, 5),
  frameS("\% to reproduce"), cajaS(false, 5),
  devG(_generaciones), devP(_tamPop),
  devR(_rate), devS(_select),
  OkCancel(_OkCancel), max(_max)
{
  set_title ("Set Parameters");
  set_policy ( false, false, false );

  set_border_width(3);

  cajaG.set_border_width (7);
  cajaP.set_border_width (7);
  cajaR.set_border_width (7);
  cajaS.set_border_width (7);
  
  // value, lower, upper, step incr, page incr, page size
  AdjG = new Gtk_Adjustment ( (*devG), 0,max, 1,2, 0 );
  ScaleG = new Gtk_HScale(*AdjG);
  ScaleG->set_digits(0); // make scales use integers
  ScalesBoxG = new Gtk_VBox(false, 0);
  ScalesBoxG->pack_end (*ScaleG);
  ScaleG->show();
  ScalesBoxG->show();

  //pop. size
  AdjP = new Gtk_Adjustment ( 0, -50,50, 1,2, 0 );
  ScaleP = new Gtk_HScale(*AdjP);
  ScaleP->set_digits(0);
  ScalesBoxP = new Gtk_VBox(false, 0);
  ScalesBoxP->pack_end (*ScaleP);
  ScaleP->show();
  ScalesBoxP->show();

  //rate for the coach
  AdjR = new Gtk_Adjustment ( (*devR), 0.0,1.0, 0.1,0.2, 0 );
  ScaleR = new Gtk_HScale(*AdjR);
  ScaleR->set_digits(1);
  ScalesBoxR = new Gtk_VBox(false, 0);
  ScalesBoxR->pack_end (*ScaleR);
  ScaleR->show();
  ScalesBoxR->show();

  //select for the chaperon
  AdjS = new Gtk_Adjustment ( (*devS), 0.0,100.0, 1.0,10.0, 0 );
  ScaleS = new Gtk_HScale(*AdjS);
  ScaleS->set_digits(1);
  ScalesBoxS = new Gtk_VBox(false, 0);
  ScalesBoxS->pack_end (*ScaleS);
  ScaleS->show();
  ScalesBoxS->show();


  // More assembly...
  Gtk_HBox * aa = get_action_area();
  aa->add(&OK);
  aa->add(&Cancel);
  OK.show();
  Cancel.show();

  Gtk_VBox * v = get_vbox();
  v->add(&frameG);
  v->add(&frameP);
  v->add(&frameR);
  v->add(&frameS);
  frameG.add(&cajaG);
  frameP.add(&cajaP);
  frameR.add(&cajaR);
  frameS.add(&cajaS);
  cajaG.pack_start(*ScalesBoxG);
  cajaP.pack_start(*ScalesBoxP);
  cajaR.pack_start(*ScalesBoxR);
  cajaS.pack_start(*ScalesBoxS);

  cajaG.show();
  cajaP.show();
  cajaR.show();
  cajaS.show();

  frameG.show();
  frameP.show();
  frameR.show();
  frameS.show();

  connect_to_method( OK.clicked,this,&TDialog::ok_clicked );
  connect_to_method( Cancel.clicked,this,&TDialog::cancel_clicked );

}

void TDialog::ok_clicked() {
  *devG=(unsigned) AdjG->get_value();
  *devP=(int) AdjP->get_value();
  *devR=(float) AdjR->get_value();
  *devS=(float) AdjS->get_value();
  *OkCancel = 1;
  closed();
}

void TDialog::cancel_clicked() {
  *OkCancel = 0;
  closed();
}
