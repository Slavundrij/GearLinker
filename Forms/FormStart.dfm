object FormLaunch: TFormLaunch
  Left = 0
  Top = 0
  Caption = #1047#1072#1075#1088#1091#1079#1082#1072
  ClientHeight = 299
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object TimerStart: TTimer
    OnTimer = TimerStartTimer
    Left = 8
    Top = 8
  end
end