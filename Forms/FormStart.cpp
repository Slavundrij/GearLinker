//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormStart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLaunch *FormLaunch;

int TimeStart = 0;
//---------------------------------------------------------------------------
__fastcall TFormLaunch::TFormLaunch(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLaunch::TimerStartTimer(TObject *Sender)
{
    TimeStart++;
    if(TimeStart >= 2){
        TimerStart->Enabled = false;
        this->Hide();
        FormAddDataFiles->Show();
    }  
}
//---------------------------------------------------------------------------

