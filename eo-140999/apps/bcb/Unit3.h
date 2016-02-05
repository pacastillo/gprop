//---------------------------------------------------------------------------
#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
//---------------------------------------------------------------------------
extern unsigned pars[6];
class TForm3 : public TForm
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
	TButton *Ok;
	TButton *Cancel;
	void __fastcall OkClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall Change1(TObject *Sender);
	void __fastcall Change2(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
