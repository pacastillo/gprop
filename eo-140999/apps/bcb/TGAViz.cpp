//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <vcl/printers.hpp>
#include <math.h>
#include <dos.h>  //para sleep (quitarlo después)
#include "TGAViz.h"
#include <Clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TGAViz* GAViz;



#include <generic/EOEasyGA.h>

/** Function that creates the initial population
@param _pop population object, passed by value
@param _numChrom number of chromosomes that will be created
@param _numBits fixed number of bits in each chromosome
*/
void createInitialPop( EOPop<EOBin<float> >& _pop,
						unsigned _numChrom, unsigned _numBits ) {
	EOBinFactory<float> factory( "SGA", _numBits );
	Chrom* pchrom = factory.make(); // Creates a chrom of specified length
	EOMutation<float> mutation(0.5);
	for ( unsigned i = 0; i < _numChrom; i++ ) {
		Chrom* pchrom2 = factory.make( *pchrom, &mutation); // Creates a chrom of specified length
		_pop.push_back( *pchrom2 );   // Adds it to the population
	}
}
//@}

//---------------------------------------------------------------------------
__fastcall TGAViz::TGAViz(TComponent* Owner):
  TForm(Owner), ya_empezo (false),
  	salimos( true) , running (false),
  	valores(NULL), indice_valores(0), max(0), thisF(),
    eval( thisF), select(), term(), bitFlip(), xOver(),
    breed() {
    Parameters1Click(NULL);
	createInitialPop( pop, SizePop, numGenes*bitXgen );
    for ( unsigned i = 0; i < SizePop; i++ ) {
        strstream s;
        s << pop[i];
        ListBox1->Items->Add( s.str() );
	}
   	breed.addOp( &xOver );
    breed.addOp( &bitFlip );

};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------
void __fastcall TGAViz::Exit2Click(TObject *Sender)
{
    salimos=TRUE;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::Button1Click(TObject *Sender)
{
	RunClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::RunClick(TObject *Sender)
{
	ya_empezo=TRUE;
	RunGA();
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::Button2Click(TObject *Sender)
{
	StepClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::StepClick(TObject *Sender)
{
	ya_empezo=TRUE;
    StepGA();
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::Button3Click(TObject *Sender)
{
	Exit2Click(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::Parameters1Click(TObject *Sender)
{
    if(ya_empezo) new TForm3(NULL);
    else new TForm2(NULL);

	// pongo primero el índice de max,min,avg para no tener que llevar la cuenta
    // de cuánto valía p[0] antes --> y así el delete hacerlo más sencillo
    unsigned i;
    if(valores) for(i=0;i<3;i++) delete[] valores[i];
    delete[] valores;
    valores=new float*[3];
    for(i=0;i<3;i++) valores[i]=new float[pars[0]];
    indice_valores=0;
    max=0;
    generacion=pars[0];
    SizePop=pars[1];
    numGenes=pars[2];
    bitXgen=pars[3];
    rateCoach=pars[4]/10.0;
    selectChaperon=pars[5]/10.0;
    Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::Dibuja_puntos(float *puntos)
{
    unsigned i;
    if(indice_valores==pars[0])
    	MessageBox(NULL,"Hemos llegado al límite","OJO",MB_ICONEXCLAMATION);
    else
    {
    	for(i=0;i<3;i++)
        	valores[i][indice_valores]=puntos[i];
        float maximo=puntos[0];
        for(i=1;i<3;i++) if(puntos[i]>maximo) maximo=puntos[i];
        if(indice_valores==0)
        {
        	max=maximo;
        }
        else
        	if(maximo>max) max=maximo;
        indice_valores++;
    }
    FormPaint(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TGAViz::FormPaint(TObject *Sender)
{
	// Compute drawing area size
	unsigned w=ClientWidth;
    unsigned h=ClientHeight;
    if(h>(unsigned)Panel1->Height)
    	h-=Panel1->Height;

    // Put rectangle around panel 1
    Canvas->Brush->Color=clWindow;
    Canvas->Pen->Color=clActiveBorder;
    Canvas->Rectangle(0,Panel1->Height,w,Panel1->Height+h);

// Habría que usar Panel1->Height (para calcular la altura real)
//    char cadena[50];
//    sprintf(cadena,"Height: %u, Width: %u",h,w);
//    MessageBox(NULL,cadena,"adiós",MB_ICONEXCLAMATION);
    const unsigned padding = 20;
    const unsigned offset = ListBox1->Width + padding;
    unsigned X_ini=offset, Y_ini=Panel1->Height+padding;

    // set initial values
    Canvas->Pen->Style=psSolid;
    Canvas->Pen->Color=clBlack;
    Canvas->Pen->Width=2;
    Canvas->Brush->Style=bsSolid;

    // Create axes
    Canvas->MoveTo(X_ini,Y_ini);
    Canvas->LineTo(X_ini,h);
    Canvas->LineTo(w,h);

    unsigned i,j;
    TColor colores[3]={clRed,clGreen,clBlue};

//    float x0,x1,y0,y1;
    if(indice_valores>1)
    {
    	float escala_y=(h-40.0)/max,escala_x=(w-40.0)/(indice_valores-1.0);
//char cadena[50];
//sprintf(cadena,"escala_y: %f, escala_x: %f",escala_y,escala_x);
//MessageBox(NULL,cadena,"adiós",MB_ICONEXCLAMATION);
    	Canvas->Pen->Width=1;
        for(j=0;j<3;j++)
    	{
        	Canvas->Pen->Color=colores[j];
           	Canvas->MoveTo(offset,h+Panel1->Height-20-valores[j][0]*escala_y);
			for(i=0;i<indice_valores;i++)
            {
            	unsigned x0=offset+i*escala_x;
                unsigned y0=h+Panel1->Height-20-valores[j][i]*escala_y;
//                if(x0>=20 && x0<=w-20 && y0>=(unsigned)Panel1->Height+20 && y0<=(unsigned)Panel1->Height-20+h)
           		Canvas->LineTo(x0,y0);
            }
    	}
	}
    char cadena[50];
    sprintf(cadena,"Generacion: %u",indice_valores);
    StatusBar1->Panels->Items[0]->Text=cadena;
    sprintf(cadena,"Máximo: %g",max);
    StatusBar1->Panels->Items[1]->Text=cadena;
    sprintf(cadena,"N.Gens: %u",numGenes);
    StatusBar1->Panels->Items[2]->Text=cadena;
    sprintf(cadena,"Pop Size: %u",SizePop);
    StatusBar1->Panels->Items[3]->Text=cadena;
	StatusBar1->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGAViz::FormResize(TObject *Sender)
{
	if(ClientWidth<300) ClientWidth=300;
	if(ClientHeight<200) ClientHeight=200;

	Invalidate();
}
void __fastcall TGAViz::RunGA()
{
	if(running==TRUE) return;
    running=TRUE;
//    GAViz->Cursor=crHourGlass;

    salimos=FALSE;
    Button3->Caption="Stop";
    Button3->Hint="Stops algorithm";
	// aquí se mete el algoritmo genético

    // Now more GA things
    term.totalGenerations( generacion );
    eval( pop );	// Initial evaluation of the population

    do {
		StepGA();
        MSG msg;
        while(1)
        {
        	if(PeekMessage(&msg,(HWND)NULL,0,0,PM_REMOVE))
        	{
        		TranslateMessage(&msg);
            	DispatchMessage(&msg);
			}
        	else break;
        }
        if(salimos) break;

    } while ( term( pop ) );

//    GAViz->Cursor=crDefault;
    Button3->Caption="Exit";
    Button3->Hint="Exits program";
    running=FALSE;
}

void __fastcall TGAViz::StepGA() {

	// aquí se mete el algoritmo genético
	//    if(running==TRUE) return;
//    GAViz->Cursor=crHourGlass;

	// Select and eliminate
    EOPop<Chrom> *pool = select( pop );

    //Apply to new Generation
    EOPop<Chrom>* newGeneration = breed( pool );
    delete pool;// has been created by select

    // evaluate the new ones
    eval( *newGeneration );

    for ( EOPop<Chrom>::iterator vi = newGeneration->begin();
	   	vi != newGeneration->end(); vi ++ ) {
		pop.push_back( *vi ) ;
    }

    // Get rid of the structure; newGeneration must have been
    // created by the breeder. But not the pointers, that are transferred
    // to oldGeneration
    delete newGeneration; // created by breed

	try {
		sort( pop.begin(), pop.end(), SortEO<Chrom>() );
    } catch ( UException& _e ) {
		cout << _e.what() << endl;
	}

	/// Compute stats
	Chrom* minEO= max_element( pop.begin(),
    							pop.end(), SortEO<Chrom>() );
    Chrom* maxEO= min_element( pop.begin(), pop.end(),
    							SortEO<Chrom>() );
    EOPop< Chrom >::const_iterator i;
    float accumulated = 0;
    for ( i = pop.begin(); i != pop.end(); i ++ ) {
      accumulated += i->fitness();
    }

	// Average is a floating-point scalar always
    float average = accumulated / (float) pop.size();
	float puntos[3] = { maxEO->fitness(), minEO->fitness(), average };
    Dibuja_puntos(puntos);

    // Show population on the panel
	ListBox1->Items->Clear( );
    for ( unsigned i = 0; i < SizePop; i++ ) {
        strstream s;
        s << pop[i] << " " << pop[i].fitness();
        ListBox1->Items->Add( s.str() );
	}
//    GAViz->Cursor=crDefault;
//    MessageBox(NULL,"Has llamado a Step","",MB_ICONEXCLAMATION);
}
