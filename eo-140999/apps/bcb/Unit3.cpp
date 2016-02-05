//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TForm3 *Form3;
unsigned pars[6]={20,50,2,32,3,150};

//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
    char cadena[50];
    TrackBar1->Position=pars[0];
    Label1->Caption=itoa(TrackBar1->Position,cadena,10);
    TrackBar2->Position=pars[1];
    Label2->Caption=itoa(TrackBar2->Position,cadena,10);
	ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::OkClick(TObject *Sender)
{
    pars[0]=TrackBar1->Position;
	pars[1]=TrackBar2->Position;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Change1(TObject *Sender)
{
	char cadena[50];
    Label1->Caption=itoa(TrackBar1->Position,cadena,10);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Change2(TObject *Sender)
{
	char cadena[50];
    Label2->Caption=itoa(TrackBar2->Position,cadena,10);

}

