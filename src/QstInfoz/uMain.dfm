object frmMain: TfrmMain
  Left = 768
  Top = 707
  Width = 800
  Height = 160
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstInfoz'
  Color = clBtnFace
  Constraints.MinHeight = 160
  Constraints.MinWidth = 800
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object sptLnR: TSplitter
    Left = 400
    Top = 0
    Width = 3
    Height = 130
    Cursor = crHSplit
  end
  object txtInfoL: TMemo
    Left = 0
    Top = 0
    Width = 400
    Height = 130
    Align = alLeft
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object txtInfoR: TMemo
    Left = 403
    Top = 0
    Width = 389
    Height = 130
    Align = alClient
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 1
    WordWrap = False
  end
end
