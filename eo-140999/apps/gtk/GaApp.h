#ifndef TWINDOW_H
#define TWINDOW_H

#include <iostream.h>
#include <math.h>
#include <vector>

#include <util/Uniform.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>
#include <generic/EOGOps.h>
#include <specific/EOBin/EOView.h>
#include <generic/EOselect/EOSteadyState.h>
#include <generic/EOselect/EOReplacers.h>
#include <generic/EOEasyGA.h>
#include <generic/EOGenTerm.h>
#include <generic/EOVector.h>
#include <generic/EOEvalAll.h>

#include <gtk--.h>
#include "gtk--canvas.h"
#include "AboutDialog.h"
#include "TWindowMenu.h"
#include "TDialogInit.h"
#include "TDialog.h"

// Some specific includes
#include "Riolo.h"

// useful typedefs
typedef EOBin<float> eoFloat;
typedef eoFloat* ptEoFloat;

const unsigned NumGenes=2;
const unsigned BitXgen =sizeof(float) * BITSPERBYTE;


  
const unsigned LIM_GENERACIONES = 400;
const unsigned sizeWindowH = 600;
const unsigned sizeWindowV = 400;
const unsigned margenAxis = 20;
const unsigned xCadMax = 50;
const unsigned yCadUp = 5;
const unsigned yCadGen = 8;
const unsigned popSize=50;
const float selectRate=0.3;

template <class EOT>
class TWindow : public Gtk_Window, public EOEvaluator<EOT> {
 public:

  //ctor.
  TWindow( Gtk_Main &m ) :
    Gtk_Window ( GTK_WINDOW_TOPLEVEL ), 
    canvas(), vbox (false, 0), barra(false,0),
    bRun("Run"), bStep("Step"), bStop("Stop"), bQuit("Quit"), pBar(),
    menu(), main (&m), 
    max(0.0), max_generaciones(20),
    generacion(0), 
    sizePop(50), OkCancel(0),
    numGenes(NumGenes), bitXgen(BitXgen), range(1),
    rateCoach(selectRate), selectChaperon( (unsigned) floor( selectRate*popSize ) ), incrTamPop(0),
    population(), termGens(20), termStep(0),
    coach(selectRate), chaperon( (unsigned) floor( selectRate*popSize ) ), 
    popKiller((unsigned) floor( selectRate*popSize ) ), evalFunc()
    {
      
      //create the algorithm instance
      evaluator = new EOEvalAll<eoFloat>( evalFunc );
      algorithm = new EOEasyGA<eoFloat>( (*this), coach, chaperon, termGens, popKiller );
      
      //Add operators
      EOMutation<float> *mutation = new EOMutation<float>(0.25);
      chaperon.addOp( mutation );
      
      Uniform<float> rate(0.25, 0.75);
      EOUniformCrossover< eoFloat > xOver( rate() );
      chaperon.addOp( &xOver ); 

      // don't allow resizing
      set_policy ( false, false, false );
      set_usize( sizeWindowH, sizeWindowV );
      set_title("EO Graphic Interface");
      
      add (&vbox);
      
      vbox.pack_start ( menu,false,true,1 );       
      vbox.pack_start ( barra,false,true,1 ); 
      vbox.pack_start ( canvas,true,true,0 );
      

      vbox.show();
                        
      barra.pack_start( bRun, false,true,1 );
      barra.pack_start( bStep, false,true,1 );
      barra.pack_start( bStop, false,true,1 );
      barra.pack_start( bQuit, false,true,1 );
      barra.pack_start ( pBar,true,true,0 );

      barra.show();
      canvas.show();
      menu.show();
      bRun.show();  //empieza oculto
      bStep.show(); //empieza oculto
      bStop.show();
      bQuit.show();
      pBar.show();

      show();
      
      connect_to_method(menu.ejecutar.activate,this,&TWindow::ejecutar );
      connect_to_method(menu.paso.activate,this,&TWindow::pasoApaso );
      connect_to_method(menu.exit.activate,this,&TWindow::exit );
      connect_to_method(menu.inicial.activate,this,&TWindow::config_dialog_inicial );
      connect_to_method(menu.prefs.activate,this,&TWindow::config_dialog );
      connect_to_method(menu.about.activate,this,&TWindow::about_dialog );
            
      connect_to_method(bRun.clicked,this,&TWindow::ejecutar );      
      connect_to_method(bStep.clicked,this,&TWindow::pasoApaso );
      connect_to_method(bQuit.clicked,this,&TWindow::exit );

      //crear los DLG para evitar estar tomando y liberando mem continuamente.
      inicial = new TDialogInit( LIM_GENERACIONES , &generacion , 
				 &sizePop , &numGenes , &bitXgen , 
				 &range , &rateCoach , &selectChaperon , 
				 &OkCancel);
      connect_to_method( inicial->closed, this, &TWindow::close_config_dialog_inicial );

      prefs = new TDialog( LIM_GENERACIONES , &generacion , 
			   &incrTamPop , 
			   &rateCoach , &selectChaperon , 
			   &OkCancel);
      connect_to_method( prefs->closed, this, &TWindow::close_config_dialog );

      ad = new AboutDialog();
      connect_to_method( ad->closed,this,&TWindow::close_about_dialog );

      inicial->show();
    };


  //dtor.
  virtual ~TWindow (void) {
    delete algorithm;
    delete evaluator;
    delete inicial;
    delete prefs;
    delete ad;
  };



/******************METHODS (gtk--)******************/
  
  gint delete_event_impl(GdkEventAny *) {
    main->instance()->quit();
    return 0;
  };


  gint key_press_event_impl (GdkEventKey * event) {
    return 1;
  };


  void exit (void) {
    Gtk_Main::instance()->quit();
  };


  //*****************dlg de acerca de.*****************
  void about_dialog (void) {
    ad->show();
    ad->redraw();
  };
  
  void close_about_dialog() {
    ad->hide();
  };


  //******************dlg para modif datos pop.*****************
  void config_dialog(void) {
    prefs->show();
  };

  void close_config_dialog() {
    prefs->hide();

    if( !OkCancel ){ return; }

    termGens.totalGenerations( generacion );
    max_generaciones = generacion;
    generacion=0;

    //set the "select rate" for the COACH and CHAPERON
    coach.rate( rateCoach );
    chaperon.select( selectChaperon );
    
    sizePop += incrTamPop;

    //  aniadir/quitar los bichos que faltan/sobran
    if( incrTamPop >= 0){   //aniadir bichos
      for ( unsigned j=0 ; j<incrTamPop ; j++ ) {
	//create a bicho, initialized to 000...00
	eoFloat *bicho = new eoFloat("",numGenes, bitXgen);
	//randomize the bicho
	EORandom<float> random; random( *bicho );
	//add the new bicho
	population.push_back( *bicho );
      }

    }else{                 //eliminar bichos
      if( sizePop > 0 ){ //eliminar si  van a quedar alguno.
	population.erase( population.end() + incrTamPop , population.end() );
      }
    }

  };


  //*****************dlg de datos iniciales*****************
  void config_dialog_inicial(void) {
    inicial->show();
  };

  void close_config_dialog_inicial() {
    inicial->hide();

    if( !OkCancel ){ return; }

    //show the run and step options
    menu.ejecutar.show(); 
    bRun.show(); 
    menu.paso.show(); 
    bStep.show();


    termGens.totalGenerations( generacion );
    max_generaciones = generacion;
    generacion=0;

    //set the "select rate" for the COACH and CHAPERON
    coach.rate( rateCoach );
    chaperon.select( selectChaperon );

    //eliminar lo que hubiera antes de aniadir mas bichos.
    population.erase( population.begin() , population.end() );

    //aniadir los bichos a la poblacion.
    for ( unsigned j = 0; j < sizePop; j ++ ) {
	//create a bicho, initialized to 000...00
	eoFloat *bicho = new eoFloat("", numGenes, bitXgen);
	//randomize the bicho
	EORandom<float> random;
	random( (*bicho) );
	//add the new bicho
	population.push_back( *bicho );
    }

  };


  //*****************redibujar las graficas.*****************
  void dibujar (void) {

    canvas.clear(); //limpia el canvas...
    canvas.gc_save();
    
    canvas.set_foreground( (guint16)0, (guint16)0, (guint16)0 ); //black color
    canvas.set_line_width((gint)3); //3 pixels widthed lines
    canvas.draw_line( margenAxis, margenAxis , margenAxis, canvas.height()-margenAxis );
    canvas.draw_line( margenAxis, canvas.height()-margenAxis , canvas.width()-margenAxis, canvas.height()-margenAxis );
    
    canvas.set_line_width((gint)1);
    
    
    int x0=margenAxis, y0=canvas.height()-margenAxis; // coords (0,0) == |_
    int xa=x0,yMa=y0,yma=y0,yaa=y0; //last x, last y_of_Maximuns, last y_of_minimuns, last y_of_averages
    int x,y; //current x and y coords
    int divx = (int) ( (float)(canvas.width()-margenAxis - x0) / (float)generacion ); //width for each x
    float divy = (float) ( (float)(y0 - margenAxis) / (float)max ); //height for each y
    
    for(unsigned i=0; i < generacion ; i++) {
      x = (divx*i) + x0; 
      
      //pintar los maximos
      y = y0 - (int)( Maximos[i] * divy );
      canvas.set_foreground( (guint16)65535, (guint16)0, (guint16)0 ); //red
      canvas.draw_line( xa, yMa , x, y );
      yMa=y;
      
      //pintar los minimos
      y = y0 - (int)( Minimos[i] * divy );
      canvas.set_foreground( (guint16)0, (guint16)65535, (guint16)0 ); //green
      canvas.draw_line( xa, yma , x, y );
      yma=y;
      
      //pintar las medias
      y = y0 - (int)( Medias[i] * divy );
      canvas.set_foreground( (guint16)0, (guint16)0, (guint16)65535 ); //blue
      canvas.draw_line( xa, yaa , x, y );
      yaa=y;
      
      xa=x;

    }

    //pintar arriba del todo el max
    canvas.set_foreground( (guint16)0, (guint16)0, (guint16)0 ); //black
    char tmp_cad_max[128];
    sprintf( tmp_cad_max, "Max= %.5f", max );
      canvas.draw_aligned_string ( xCadMax, yCadUp, tmp_cad_max, 0, 0, 0,0,0,0 );
    sprintf( tmp_cad_max, "%d", generacion );
      canvas.draw_aligned_string ( canvas.width()/2, canvas.height()-yCadGen, tmp_cad_max, 0, 0, 0,0,0,0 );

    sprintf( tmp_cad_max, "N.Gens=%d   SizePop=%d", max_generaciones , sizePop );
      canvas.draw_aligned_string ( canvas.width()/2, yCadUp, tmp_cad_max, 0, 0, 0,0,0,0 );

    
    canvas.gc_restore();
    canvas.refresh(); //refresh the canvas

    // Actualizar la barra de progreso
    pBar.set_percentage( generacion*1.0/max_generaciones );
    
    // Actualizar los eventos de GTK
    main->iteration();
  };


  //*****************func para ejec. el algoritmo*****************
  void ejecutar (void) {
    if( population.size() > 0 ) {
      termGens.totalGenerations( max_generaciones ) ;

      (*evaluator)( population );
      (*algorithm)( population );

      //hide the InitPrefs option
      menu.inicial.hide();
      menu.prefs.show();
    }

  };
  
  
  //*****************func para ejec. solo un paos*****************
  void pasoApaso(void) {
    if( population.size() > 0 ) {
      termGens.totalGenerations( 1 );

      (*evaluator)( population );
      (*algorithm)( population );

      //hide the InitPrefs option
      menu.inicial.hide();
      menu.prefs.show();
    }

  };
  
  
  void nuevosvalores(float M, float m, float a) {

    Maximos.push_back( M );
    Minimos.push_back( m );
    Medias.push_back( a );
    if( max < M ) max=M;

    generacion++;
    
    dibujar();

    //cout <<"(max="<<max<<")  G= "<<generacion<<"\tM="<<M<<"\ta="<<a<<"\tm="<<m<< endl;
  };



/******************METHODS (EO)******************/

  virtual void Ef( const EOEvalFunc<EOT> & _ef ) {
    evaluator->Ef( _ef );
  }


  typedef typename EOT::EOFitT EOFitT;
  virtual void operator()( EOPop< EOT >& _vEO ) {
    (*evaluator)( _vEO );
    EOT* minEO= max_element( _vEO.begin(), _vEO.end(), SortEO<EOT>() );
    EOT* maxEO= min_element( _vEO.begin(), _vEO.end(), SortEO<EOT>() );
    EOPop< EOT >::const_iterator i;
    EOFitT accumulated = 0;
    for ( i = _vEO.begin(); i != _vEO.end(); i ++ ) {
      accumulated += i->fitness();
    }
    float average = accumulated / (float) _vEO.size();

    float M_tmp = maxEO->fitness();
    float m_tmp = minEO->fitness();

    nuevosvalores( M_tmp , m_tmp , average );
  }



/******************MEMBERS******************/
private:
  Gtk_Canvas canvas;
  Gtk_VBox vbox;
  Gtk_HBox barra;
  Gtk_Button bRun, bStep, bStop, bQuit;
  Gtk_ProgressBar pBar;
  TWindowMenu menu;
  Gtk_Main * main;
  AboutDialog *ad;
  TDialog *prefs;
  TDialogInit *inicial;

  EOEasyGA<eoFloat> *algorithm;
  EOPop<eoFloat> population;
  Riolo evalFunc;
  EOEvalAll<eoFloat> *evaluator;
  EOSteadyState<eoFloat> coach;
  EORank<eoFloat> chaperon;
  EOElimWorst< eoFloat > popKiller;
  EOGenTerm<eoFloat>  termGens , termStep;  

  vector<float> Maximos;
  vector<float> Minimos;
  vector<float> Medias;
  float max;

  unsigned max_generaciones, range;
  unsigned generacion, sizePop, numGenes, bitXgen;
  float rateCoach, selectChaperon;
  unsigned OkCancel;
  int incrTamPop;
};

#endif
