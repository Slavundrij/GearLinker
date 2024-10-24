// CodeGear C++Builder
// Copyright (c) 1995, 2022 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'System.Win.StdVCL.pas' rev: 35.00 (Windows)

#ifndef System_Win_StdvclHPP
#define System_Win_StdvclHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>

//-- user supplied -----------------------------------------------------------
#include <oaidl.h>

namespace System
{
namespace Win
{
namespace Stdvcl
{
//-- forward type declarations -----------------------------------------------
__interface DELPHIINTERFACE IProvider;
typedef System::DelphiInterface<IProvider> _di_IProvider;
__dispinterface IProviderDisp;
typedef System::DelphiInterface<IProviderDisp> _di_IProviderDisp;
__interface DELPHIINTERFACE IStrings;
typedef System::DelphiInterface<IStrings> _di_IStrings;
__dispinterface IStringsDisp;
typedef System::DelphiInterface<IStringsDisp> _di_IStringsDisp;
__interface DELPHIINTERFACE IDataBroker;
typedef System::DelphiInterface<IDataBroker> _di_IDataBroker;
__dispinterface IDataBrokerDisp;
typedef System::DelphiInterface<IDataBrokerDisp> _di_IDataBrokerDisp;
//-- type declarations -------------------------------------------------------
__interface  INTERFACE_UUID("{6E644935-51F7-11D0-8D41-00A0248E4B9A}") IProvider  : public IDispatch 
{
	virtual HRESULT __safecall Get_Data(System::OleVariant &__Get_Data_result) = 0 ;
	virtual HRESULT __safecall ApplyUpdates(System::OleVariant Delta, int MaxErrors, /* out */ int &ErrorCount, System::OleVariant &__ApplyUpdates_result) = 0 ;
	virtual HRESULT __safecall GetMetaData(System::OleVariant &__GetMetaData_result) = 0 ;
	virtual HRESULT __safecall GetRecords(int Count, /* out */ int &RecsOut, System::OleVariant &__GetRecords_result) = 0 ;
	virtual HRESULT __safecall DataRequest(System::OleVariant Input, System::OleVariant &__DataRequest_result) = 0 ;
	virtual HRESULT __safecall Get_Constraints(System::WordBool &__Get_Constraints_result) = 0 ;
	virtual HRESULT __safecall Set_Constraints(System::WordBool Value) = 0 ;
	virtual HRESULT __safecall Reset(System::WordBool MetaData) = 0 ;
	virtual HRESULT __safecall SetParams(System::OleVariant Values) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::OleVariant _scw_Get_Data() { System::OleVariant __r; HRESULT __hr = Get_Data(__r); System::CheckSafecallResult(__hr); return __r; }
	#pragma option pop
	__property System::OleVariant Data = {read=_scw_Get_Data};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::WordBool _scw_Get_Constraints() { System::WordBool __r; HRESULT __hr = Get_Constraints(__r); System::CheckSafecallResult(__hr); return __r; }
	#pragma option pop
	__property System::WordBool Constraints = {read=_scw_Get_Constraints, write=Set_Constraints};
};

__dispinterface  INTERFACE_UUID("{6E644935-51F7-11D0-8D41-00A0248E4B9A}") IProviderDisp  : public IDispatch 
{
	
};

__interface  INTERFACE_UUID("{EE05DFE2-5549-11D0-9EA9-0020AF3D82DA}") IStrings  : public IDispatch 
{
	
public:
	System::OleVariant operator[](int Index) { return this->ControlDefault[Index]; }
	virtual HRESULT __safecall Get_ControlDefault(int Index, System::OleVariant &__Get_ControlDefault_result) = 0 ;
	virtual HRESULT __safecall Set_ControlDefault(int Index, System::OleVariant Value) = 0 ;
	virtual HRESULT __safecall Count(int &__Count_result) = 0 ;
	virtual HRESULT __safecall Get_Item(int Index, System::OleVariant &__Get_Item_result) = 0 ;
	virtual HRESULT __safecall Set_Item(int Index, System::OleVariant Value) = 0 ;
	virtual HRESULT __safecall Remove(int Index) = 0 ;
	virtual HRESULT __safecall Clear() = 0 ;
	virtual HRESULT __safecall Add(System::OleVariant Item, int &__Add_result) = 0 ;
	virtual HRESULT __safecall _NewEnum(System::_di_IInterface &___NewEnum_result) = 0 ;
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::OleVariant _scw_Get_ControlDefault(int Index) { System::OleVariant __r; HRESULT __hr = Get_ControlDefault(Index, __r); System::CheckSafecallResult(__hr); return __r; }
	#pragma option pop
	__property System::OleVariant ControlDefault[int Index] = {read=_scw_Get_ControlDefault, write=Set_ControlDefault/*, default*/};
	#pragma option push -w-inl
	/* safecall wrapper */ inline System::OleVariant _scw_Get_Item(int Index) { System::OleVariant __r; HRESULT __hr = Get_Item(Index, __r); System::CheckSafecallResult(__hr); return __r; }
	#pragma option pop
	__property System::OleVariant Item[int Index] = {read=_scw_Get_Item, write=Set_Item};
};

__dispinterface  INTERFACE_UUID("{EE05DFE2-5549-11D0-9EA9-0020AF3D82DA}") IStringsDisp  : public IDispatch 
{
	
};

__interface  INTERFACE_UUID("{6539BF65-6FE7-11D0-9E8C-00A02457621F}") IDataBroker  : public IDispatch 
{
	virtual HRESULT __safecall GetProviderNames(System::OleVariant &__GetProviderNames_result) = 0 ;
};

__dispinterface  INTERFACE_UUID("{6539BF65-6FE7-11D0-9E8C-00A02457621F}") IDataBrokerDisp  : public IDispatch 
{
	
};

//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE GUID LIBID_StdVCL;
extern DELPHI_PACKAGE GUID IID_IProvider;
extern DELPHI_PACKAGE GUID IID_IStrings;
extern DELPHI_PACKAGE GUID IID_IDataBroker;
}	/* namespace Stdvcl */
}	/* namespace Win */
}	/* namespace System */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_SYSTEM_WIN_STDVCL)
using namespace System::Win::Stdvcl;
#endif
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_SYSTEM_WIN)
using namespace System::Win;
#endif
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_SYSTEM)
using namespace System;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// System_Win_StdvclHPP
