object frmMain: TfrmMain
  Left = 657
  Top = 272
  Width = 678
  Height = 600
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstHash'
  Color = clBtnFace
  Constraints.MaxHeight = 600
  Constraints.MaxWidth = 678
  Constraints.MinHeight = 600
  Constraints.MinWidth = 678
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object btnHEX: TButton
    Left = 8
    Top = 8
    Width = 49
    Height = 25
    Caption = 'HEX'
    TabOrder = 0
    OnClick = btnHEXClick
  end
  object edtInput: TEdit
    Left = 64
    Top = 8
    Width = 542
    Height = 21
    TabOrder = 1
  end
  object btnSTR: TButton
    Left = 613
    Top = 8
    Width = 49
    Height = 25
    Caption = 'STR'
    TabOrder = 2
    OnClick = btnSTRClick
  end
  object lstHash: TCheckListBox
    Left = 8
    Top = 40
    Width = 217
    Height = 521
    ItemHeight = 13
    TabOrder = 3
    OnDblClick = lstHashDblClick
  end
  object txtResult: TMemo
    Left = 232
    Top = 40
    Width = 430
    Height = 521
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 4
    OnDblClick = txtResultDblClick
  end
end
