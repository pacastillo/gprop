//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TForm2 *Form2;
//extern unsigned pars[6];
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
    char cadena[50];
    TrackBar1->Position=pars[0];
    Label1->Caption=itoa(TrackBar1->Position,cadena,10);
    TrackBar2->Position=pars[1];
    Label2->Caption=itoa(TrackBar2->Position,cadena,10);
    TrackBar3->Position=pars[2];
    Label3->Caption=itoa(TrackBar3->Position,cadena,10);
    TrackBar4->Position=pars[3];
    Label4->Caption=itoa(TrackBar4->Position,cadena,10);
	ShowModal();

}
//---------------------------------------------------------------------------
void __fastcall TForm2::OkClick(TObject *Sender)
{
    pars[0]=TrackBar1->Position;
	pars[1]=TrackBar2->Position;
    pars[2]=TrackBar3->Position;
	pars[3]=TrackBar4->Position;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Change1(TObject *Sender)
{
	char cadena[5];
    Label1->Caption=itoa(TrackBar1->Position,cadena,10);

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Change2(TObject *Sender)
{
	char cadena[5];
    Label2->Caption=itoa(TrackBar2->Position,cadena,10);

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Change3(TObject *Sender)
{
	char cadena[5];
    Label3->Caption=itoa(TrackBar3->Position,cadena,10);

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Change4(TObject *Sender)
{
	char cadena[5];
    Label4->Caption=itoa(TrackBar4->Position,cadena,10);

}

