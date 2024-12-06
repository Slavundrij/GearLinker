//---------------------------------------------------------------------------

#ifndef TFormManualParametersH
#define TFormManualParametersH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <map>
#include <iostream>
#include <fstream>

#include "TFileLoader.h"
//---------------------------------------------------------------------------
class TFormManualParameters : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *MinShiftsGrid;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TStringGrid *ModuleGrid;
	TStringGrid *TeethGrid;
	TLabel *Label5;
	TStringGrid *WidthGrid;
	TButton *SaveButton;
	TButton *AbortButton;
	TComboBox *DefaultSizeComboBox;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall AbortButtonClick(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	void updateParamGrids(void);
	__fastcall TFormManualParameters(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormManualParameters *FormManualParameters;
//---------------------------------------------------------------------------
void resizeGrid(TStringGrid *Grid, bool need_rotate);
#endif
