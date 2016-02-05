#include "TCommonDialog.h"

TCommonDialog::TCommonDialog(unsigned _max, 
			 unsigned &_generaciones, unsigned &_tamPop, 
			 unsigned &_numGenes, unsigned &_bitXgen,
			 float &_rate, 
			 unsigned &_OkCancel, bool _init = true) :
  OK("OK"), Cancel("Cancel"), 
  frameG("Number of Generations"), cajaG(false, 5),
  frameP("Initial Pop. Size"), cajaP(false, 5),
  frameNG("Num. Genes"), cajaNG(false, 5),
  frameBG("Bits x Gen"), cajaBG(false, 5),
  frameR("Rate to kill"), cajaR(false, 5),
  devG(_generaciones), devP(_tamPop), devNG(_numGenes), devBG(_bitXgen),
  devR(_rate), 
  OkCancel(_OkCancel), max(_max), init(_init)
{
  set_title ("Initial Parameters");
  set_policy ( false, false, false );

  set_border_width(3);

  cajaG.set_border_width (7);
  cajaP.set_border_width (7);
  cajaNG.set_border_width (7);
  cajaBG.set_border_width (7);
  cajaR.set_border_width (7);
  
  // value, lower, upper, step incr, page incr, page size
  //number of generations.
  AdjG = new Gtk_Adjustment ( devG, 0,max, 1,10, 0 );
  ScaleG = new Gtk_HScale(*AdjG);
  ScaleG->set_digits(0); // make scales use integers
  ScalesBoxG = new Gtk_VBox(false, 0);
  ScalesBoxG->pack_end (*ScaleG);
  ScaleG->show();
  ScalesBoxG->show();

  //pop. size
  AdjP = new Gtk_Adjustment ( devP, 0,max, 1,10, 0 );
  ScaleP = new Gtk_HScale(*AdjP);
  ScaleP->set_digits(0);
  ScalesBoxP = new Gtk_VBox(false, 0);
  ScalesBoxP->pack_end (*ScaleP);
  ScaleP->show();
  ScalesBoxP->show();

  if ( _init ) {
    //number of genes
    AdjNG = new Gtk_Adjustment ( devNG, 1,100, 1,10, 0 );
    ScaleNG = new Gtk_HScale(*AdjNG);
    ScaleNG->set_digits(0); // make scales use integers
    ScalesBoxNG = new Gtk_VBox(false, 0);
    ScalesBoxNG->pack_end (*ScaleNG);
    ScaleNG->show();
    ScalesBoxNG->show();

    //bits x gen
    AdjBG = new Gtk_Adjustment ( devBG, 1,100, 1,10, 0 );
    ScaleBG = new Gtk_HScale(*AdjBG);
    ScaleBG->set_digits(0); // make scales use integers
    ScalesBoxBG = new Gtk_VBox(false, 0);
    ScalesBoxBG->pack_end (*ScaleBG);
    ScaleBG->show();
    ScalesBoxBG->show();
  }

  //rate for the coach
  AdjR = new Gtk_Adjustment ( devR, 0.0,1.0, 0.1,0.2, 0 );
  ScaleR = new Gtk_HScale(*AdjR);
  ScaleR->set_digits(1);
  ScalesBoxR = new Gtk_VBox(false, 0);
  ScalesBoxR->pack_end (*ScaleR);
  ScaleR->show();
  ScalesBoxR->show();

  // More assembly...
  Gtk_HBox * aa = get_action_area();
  aa->add(&OK);
  aa->add(&Cancel);
  OK.show();
  Cancel.show();

  Gtk_VBox * v = get_vbox();
  v->add(&frameG);
  v->add(&frameP);
  v->add(&frameNG);
  v->add(&frameBG);
  v->add(&frameR);
  frameG.add(&cajaG);
  frameP.add(&cajaP);
  if ( _init ) {
    frameNG.add(&cajaNG);
    frameBG.add(&cajaBG);
  }

  frameR.add(&cajaR);
  cajaG.pack_start(*ScalesBoxG);
  cajaP.pack_start(*ScalesBoxP);
  if ( _init ) {
    cajaNG.pack_start(*ScalesBoxNG);
    cajaBG.pack_start(*ScalesBoxBG);
  }
  cajaR.pack_start(*ScalesBoxR);

  cajaG.show();
  cajaP.show();
  if ( _init ) {
    cajaNG.show();
    cajaBG.show();
  }
  cajaR.show();


  frameG.show();
  frameP.show();
  if ( _init ) {
    frameNG.show();
    frameBG.show();
  }
  frameR.show();

  connect_to_method( OK.clicked,this,&TCommonDialog::ok_clicked );
  connect_to_method( Cancel.clicked,this,&TCommonDialog::cancel_clicked );

}

void TCommonDialog::ok_clicked() {
  devG=(unsigned) AdjG->get_value();
  devP=(unsigned) AdjP->get_value();
  if ( init ) {
    devNG=(unsigned) AdjNG->get_value();
    devBG=(unsigned) AdjBG->get_value();
  }
  devR=(float) AdjR->get_value();
  OkCancel = 1;
  closed();
}

void TCommonDialog::cancel_clicked() {
  OkCancel = 0;
  closed();
}
