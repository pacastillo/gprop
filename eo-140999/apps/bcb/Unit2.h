//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label13;
	TPanel *Panel1;
	TLabel *Label7;
	TLabel *Label1;
	TTrackBar *TrackBar1;
	TPanel *Panel2;
	TLabel *Label8;
	TLabel *Label2;
	TTrackBar *TrackBar2;
	TPanel *Panel4;
	TLabel *Label9;
	TLabel *Label3;
	TTrackBar *TrackBar3;
	TPanel *Panel5;
	TLabel *Label10;
	TLabel *Label4;
	TTrackBar *TrackBar4;
	TButton *Ok;
	TButton *Cancel;
	void __fastcall OkClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	
	void __fastcall Change1(TObject *Sender);
	void __fastcall Change2(TObject *Sender);
	void __fastcall Change3(TObject *Sender);
	void __fastcall Change4(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
