object frmMain: TfrmMain
  Left = 577
  Top = 333
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'xParamSet'
  ClientHeight = 566
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object pgeMain: TPageControl
    Left = 0
    Top = 0
    Width = 792
    Height = 521
    ActivePage = tabQLAB
    Align = alTop
    TabIndex = 0
    TabOrder = 0
    object tabQLAB: TTabSheet
      Caption = 'QuestLAB'
      object grpQstView2D: TGroupBox
        Left = 8
        Top = 8
        Width = 161
        Height = 193
        Caption = 'QstView2D'
        TabOrder = 0
        object Label1: TLabel
          Left = 16
          Top = 64
          Width = 65
          Height = 20
          AutoSize = False
          Caption = #32972#26223#39068#33394
          Transparent = True
        end
        object Label2: TLabel
          Left = 88
          Top = 64
          Width = 65
          Height = 20
          AutoSize = False
          Caption = #20851#38190#39068#33394
          Transparent = True
        end
        object txtQV2D_bkcolor: TStaticText
          Left = 16
          Top = 24
          Width = 58
          Height = 33
          AutoSize = False
          BorderStyle = sbsSingle
          TabOrder = 0
          OnClick = txtQV2D_bkcolorClick
        end
        object txtQV2D_def_keycolor: TStaticText
          Left = 88
          Top = 24
          Width = 58
          Height = 33
          AutoSize = False
          BorderStyle = sbsSingle
          TabOrder = 1
          OnClick = txtQV2D_def_keycolorClick
        end
        object isQV2D_is_blends: TCheckBox
          Left = 24
          Top = 88
          Width = 129
          Height = 17
          Caption = #21551#29992#36879#26126#28151#21512
          TabOrder = 2
        end
        object isQV2D_is_center: TCheckBox
          Left = 24
          Top = 112
          Width = 129
          Height = 17
          Caption = #22270#29255#23621#20013#23545#40784
          TabOrder = 3
        end
        object isQV2D_use_keycolor: TCheckBox
          Left = 24
          Top = 136
          Width = 129
          Height = 17
          Caption = #21551#29992#20851#38190#39068#33394
          TabOrder = 4
        end
        object isQV2D_use_keyalpha: TCheckBox
          Left = 24
          Top = 160
          Width = 129
          Height = 17
          Caption = #36879#26126#30340#20851#38190#33394
          TabOrder = 5
        end
      end
      object grpQstMedia: TGroupBox
        Left = 8
        Top = 208
        Width = 161
        Height = 81
        Caption = 'QstMedia'
        TabOrder = 1
        object isQXMM_enabled: TCheckBox
          Left = 24
          Top = 24
          Width = 129
          Height = 17
          Caption = #21551#29992#23186#20307#25773#25918
          TabOrder = 0
        end
        object isQXMM_loop_play: TCheckBox
          Left = 24
          Top = 48
          Width = 129
          Height = 17
          Caption = #24490#29615#25773#25918#23186#20307
          TabOrder = 1
        end
      end
    end
  end
  object btnOK: TButton
    Left = 480
    Top = 528
    Width = 97
    Height = 33
    Caption = 'OKAY'
    Default = True
    TabOrder = 1
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 688
    Top = 528
    Width = 97
    Height = 33
    Cancel = True
    Caption = 'CANCEL'
    TabOrder = 3
    OnClick = btnCancelClick
  end
  object btnApply: TButton
    Left = 584
    Top = 528
    Width = 97
    Height = 33
    Caption = 'APPLY'
    TabOrder = 2
    OnClick = btnApplyClick
  end
  object dlgColor: TColorDialog
    Ctl3D = True
    Left = 8
    Top = 528
  end
end
