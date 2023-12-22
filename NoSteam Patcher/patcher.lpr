program patcher;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}
  cthreads,
  {$ENDIF}
  {$IFDEF HASAMIGA}
  athreads,
  {$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, main
  { you can add units after this };

{$R *.res}

begin
  RequireDerivedFormResource:=True;
  Application.Title:='NoSteam patcher for CK';
  Application.Scaled:=True;
  Application.Initialize;
  Application.CreateForm(TFormPatcher, FormPatcher);
  Application.Run;
end.

