object frmMain: TfrmMain
  Left = 657
  Top = 272
  Width = 600
  Height = 600
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstData'
  Color = clBtnFace
  Constraints.MaxHeight = 600
  Constraints.MaxWidth = 600
  Constraints.MinHeight = 600
  Constraints.MinWidth = 600
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
    Width = 465
    Height = 21
    TabOrder = 1
  end
  object btnSTR: TButton
    Left = 536
    Top = 8
    Width = 49
    Height = 25
    Caption = 'STR'
    TabOrder = 2
    OnClick = btnSTRClick
  end
  object pgeMain: TPageControl
    Left = 8
    Top = 40
    Width = 577
    Height = 521
    ActivePage = tabValue
    TabIndex = 0
    TabOrder = 3
    object tabValue: TTabSheet
      Caption = 'Value'
      object txtValue: TMemo
        Left = 0
        Top = 0
        Width = 569
        Height = 493
        Align = alClient
        ReadOnly = True
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object tabImage: TTabSheet
      Caption = 'Image'
      ImageIndex = 1
    end
    object tabAudio: TTabSheet
      Caption = 'Audio'
      ImageIndex = 2
    end
    object tabData: TTabSheet
      Caption = 'Binary'
      ImageIndex = 3
    end
  end
end
