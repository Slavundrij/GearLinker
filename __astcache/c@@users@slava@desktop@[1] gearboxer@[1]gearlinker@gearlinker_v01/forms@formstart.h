//---------------------------------------------------------------------------

#ifndef FormStartH
#define FormStartH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "TFormAddDataFiles.h"
//---------------------------------------------------------------------------
class TFormLaunch : public TForm
{
__published:	// IDE-managed Components
    TTimer *TimerStart;
    void __fastcall TimerStartTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormLaunch(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLaunch *FormLaunch;
//---------------------------------------------------------------------------
#endif
