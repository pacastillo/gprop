//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifndef TGAVizH
#define TGAVizH
//---------------------------------------------------------------------------
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include "unit2.h"
#include "unit3.h"

// Now the EO Classes
#include <specific/EOBin/EOBin.h>
#include <ADT/EOPop.h>

#include <generic/EOEvalAll.h>      // for Evaluator
#include <generic/EOGOps.h>
#include <generic/EOEasyGA.h>
#include <generic/EOselect/EOGSLottery.h>     // for EOSelector and Breeder
#include <generic/EOselect/EOGSTourn.h>     // for EOSelector
#include <generic/EOselect/EORandomBreed.h>     // for Breeder
#include <generic/EOGenTerm.h>
#include <specific/EOBin/EOBin.h>
#include <specific/EOBin/EOBinOp.h>        // For EOBitFlip

typedef EOBin<float> Chrom;

/// Evaluation function
struct NumOnes: public EOEvalFunc< Chrom > {
  virtual float evaluate( Chrom & _eo ) const {
    float numOnes = 0;
    for ( unsigned i = 0; i < _eo.length(); i ++ )
      numOnes += ( _eo[i] == true)?1:0 ;
    _eo.fitness( numOnes );
    return numOnes;
  };
};

///
class TGAViz: public TForm {
__published:
    TPanel *Panel1;
    TMainMenu *MainMenu1;
	TMenuItem *Genetic;
	TMenuItem *Run;
	TMenuItem *Step;
	TMenuItem *Parameters1;
	TMenuItem *N2;
	TMenuItem *Exit2;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TStatusBar *StatusBar1;

	TListBox *ListBox1;
	void __fastcall Exit2Click(TObject *Sender);

	void __fastcall Button1Click(TObject *Sender);
	void __fastcall RunClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall StepClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Parameters1Click(TObject *Sender);

	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

	
public:

	/// Something like a ctor
   	virtual __fastcall TGAViz(TComponent* Owner);

    /// Run without stop
    void __fastcall RunGA();

    /// Run step by step
    void __fastcall StepGA();

    /// Draw everything
    void __fastcall Dibuja_puntos(float *puntos);


private:        // private user declarations
    bool ya_empezo;
    bool salimos,running;
    float **valores;
    unsigned indice_valores;
    float max;

// Éstas son las funciones que realmente interesan para el interface
// Tambíen pongo aquí las variables de la interfase

	// Some typedefs
	typedef EOPop<Chrom> Pop;

  	// Create an initial population
  	Pop pop;
    NumOnes thisF;
    EvalAll<Chrom>  eval;
  	EOLottery<Chrom> select;
  	EOGenTerm<Chrom> term;

  	EOBitFlip<float> bitFlip;
  	EOXOver2<Chrom> xOver;

    EOLottery<Chrom> chaperon;
  	EORandomBreed<Chrom> breed;


	unsigned generacion,SizePop,numGenes,bitXgen;
    float rateCoach,selectChaperon;
};
//---------------------------------------------------------------------------
extern TGAViz *GAViz;

#endif
