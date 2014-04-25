object frmMain: TfrmMain
  Left = 657
  Top = 272
  Width = 678
  Height = 600
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstData'
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
  object btnLE: TButton
    Left = 8
    Top = 8
    Width = 49
    Height = 25
    Caption = 'LE'
    TabOrder = 0
    OnClick = btnLEClick
  end
  object edtInput: TEdit
    Left = 64
    Top = 8
    Width = 542
    Height = 21
    TabOrder = 1
  end
  object btnBE: TButton
    Left = 613
    Top = 8
    Width = 49
    Height = 25
    Caption = 'BE'
    TabOrder = 2
    OnClick = btnBEClick
  end
  object pgeMain: TPageControl
    Left = 8
    Top = 40
    Width = 654
    Height = 521
    ActivePage = tabValue
    TabIndex = 0
    TabOrder = 3
    object tabValue: TTabSheet
      Caption = 'Value'
      object txtValue: TMemo
        Left = 0
        Top = 0
        Width = 646
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
