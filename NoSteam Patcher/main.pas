unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls;

type

  { TFormPatcher }

  TFormPatcher = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Edit1: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    OpenDialog1: TOpenDialog;
    Panel1: TPanel;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    FFileSelected: String;
  end;

var
  FormPatcher: TFormPatcher;

implementation

const
  AppDef: String = 'CreationKit.exe';
  SearchSt: String = 'steam_api64.dll';
  NeedSt: String = 'winhttp.dll';

{$R *.lfm}

{ TFormPatcher }

procedure TFormPatcher.FormCreate(Sender: TObject);
begin
  if FileExists(AppDef) then
  begin
    Edit1.Text := AppDef;
    FFileSelected := ExtractFilePath(Application.ExeName) + AppDef;
  end;
end;

procedure TFormPatcher.Button1Click(Sender: TObject);
begin
  OpenDialog1.InitialDir := ExtractFilePath(Application.ExeName);
  if OpenDialog1.Execute then
  begin
    Edit1.Text := ExtractFileName(OpenDialog1.FileName);
    FFileSelected := OpenDialog1.FileName;
  end;
end;

procedure TFormPatcher.Button2Click(Sender: TObject);
type
  PByte = ^Byte;
var
  FileStream: TFileStream;
  MemoryStream: TMemoryStream;
  IPos, S, SearchPos: Int64;
  Pt: PByte;
  Pss: PAnsiChar;
  ul: Integer;
begin
  if not FileExists(FFileSelected) then
  begin
    MessageDlg('Error',
      Format('I can''t find the file "%s"', [FFileSelected]),
      mtError, [mbOk], 0);
    Exit;
  end;
  try
    try
      FileStream := TFileStream.Create(FFileSelected,
         fmOpenRead or fmShareDenyWrite);
      MemoryStream := TMemoryStream.Create;
      MemoryStream.CopyFrom(FileStream, 0);
      S := MemoryStream.Size - 1;
      if Assigned(FileStream) then
        FreeAndNil(FileStream);

      Pt := PByte(MemoryStream.Memory);
      Pss := PAnsiChar(SearchSt);
      ul := Length(SearchSt);
      SearchPos := 0;
      for IPos := 0 to S do
      begin
        if CompareMem(@Pt[IPos], Pss, ul) then
        begin
          SearchPos := IPos;
          Break;
        end;
      end;

      if SearchPos <> 0 then
      begin
        ul := Length(NeedSt);
        Pss := PAnsiChar(NeedSt);
        Move(Pss[0], Pt[IPos], ul);
        Pt[IPos + ul] := 0;
        Pt[IPos + ul + 1] := 0;
        Pt[IPos + ul + 2] := 0;
        Pt[IPos + ul + 3] := 0;

        FileStream := TFileStream.Create(
         ChangeFileExt(FFileSelected, '') + '_patched.exe',
         fmCreate);
        FileStream.CopyFrom(MemoryStream, 0);

        ShowMessage('Patched.');
      end
      else
        ShowMessage('The signature could not be found.');
    finally
      if Assigned(MemoryStream) then
        MemoryStream.Free;
      if Assigned(FileStream) then
        FileStream.Free;
    end;
  except
    on E: Exception do
      MessageDlg('Error', E.Message, mtError, [mbOk], 0);
  end;
end;

end.

