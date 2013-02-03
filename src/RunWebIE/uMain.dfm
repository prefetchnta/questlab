object frmMain: TfrmMain
  Left = 605
  Top = 305
  Width = 800
  Height = 600
  Caption = 'RunWebIE'
  Color = clBtnFace
  Constraints.MinHeight = 600
  Constraints.MinWidth = 800
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object cppWeb: TCppWebBrowser
    Left = 0
    Top = 0
    Width = 792
    Height = 570
    Align = alClient
    TabOrder = 0
    OnBeforeNavigate2 = cppWebBeforeNavigate2
    ControlData = {
      4C000000DB510000E93A00000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E126208000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
end
