object FormManualParameters: TFormManualParameters
  Left = 0
  Top = 0
  Caption = #1056#1091#1095#1085#1086#1081' '#1074#1074#1086#1076' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074' '#1089#1073#1086#1088#1082#1080
  ClientHeight = 524
  ClientWidth = 996
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OnCreate = FormCreate
  TextHeight = 15
  object Label1: TLabel
    Left = 48
    Top = 8
    Width = 161
    Height = 15
    Caption = #1053#1072#1080#1084#1077#1085#1100#1096#1080#1077' '#1076#1086#1087'. '#1089#1084#1077#1097#1077#1085#1080#1103
  end
  object Label2: TLabel
    Left = 384
    Top = 8
    Width = 44
    Height = 15
    Caption = #1052#1086#1076#1091#1083#1080
  end
  object Label3: TLabel
    Left = 640
    Top = 8
    Width = 73
    Height = 15
    Caption = #1063#1080#1089#1083#1072' '#1079#1091#1073#1100#1077#1074
  end
  object Label4: TLabel
    Left = 849
    Top = 8
    Width = 126
    Height = 15
    Caption = #1056#1072#1079#1084#1077#1088' '#1087#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
  end
  object Label5: TLabel
    Left = 456
    Top = 296
    Width = 92
    Height = 15
    Caption = #1064#1080#1088#1080#1085#1072' '#1076#1086#1087#1091#1089#1082#1072
  end
  object MinShiftsGrid: TStringGrid
    Left = 8
    Top = 24
    Width = 250
    Height = 250
    ColCount = 2
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goFixedRowDefAlign]
    TabOrder = 0
  end
  object ModuleGrid: TStringGrid
    Left = 280
    Top = 24
    Width = 250
    Height = 250
    ColCount = 2
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goFixedRowDefAlign]
    TabOrder = 1
    RowHeights = (
      24
      24
      24
      24
      24
      23
      20
      24
      24)
  end
  object TeethGrid: TStringGrid
    Left = 552
    Top = 24
    Width = 250
    Height = 250
    ColCount = 2
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goFixedRowDefAlign]
    TabOrder = 2
    RowHeights = (
      24
      24
      25
      23
      24
      24
      24
      24
      24)
  end
  object WidthGrid: TStringGrid
    Left = 8
    Top = 317
    Width = 980
    Height = 199
    ColCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goFixedRowDefAlign]
    TabOrder = 3
  end
  object SaveButton: TButton
    Left = 838
    Top = 152
    Width = 150
    Height = 50
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 4
    OnClick = SaveButtonClick
  end
  object AbortButton: TButton
    Left = 838
    Top = 224
    Width = 150
    Height = 50
    Caption = #1054#1090#1084#1077#1085#1080#1090#1100
    TabOrder = 5
    OnClick = AbortButtonClick
  end
  object DefaultSizeComboBox: TComboBox
    Left = 838
    Top = 24
    Width = 150
    Height = 23
    Style = csDropDownList
    TabOrder = 6
  end
end
