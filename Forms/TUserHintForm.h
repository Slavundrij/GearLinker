//---------------------------------------------------------------------------

#ifndef TUserHintFormH
#define TUserHintFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TUserHintForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
private:	// User declarations
public:		// User declarations
	__fastcall TUserHintForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TUserHintForm *UserHintForm;
//---------------------------------------------------------------------------
#endif
