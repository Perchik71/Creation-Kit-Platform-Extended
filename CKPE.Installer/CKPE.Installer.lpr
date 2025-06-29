program CKPE.Installer;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}
  cthreads,
  {$ENDIF}
  {$IFDEF HASAMIGA}
  athreads,
  {$ENDIF}
  Interfaces, // this includes the LCL widgetset
  ComObj, ActiveX, Forms, Windows, SysUtils, CKPE.Installer.MainForm,
  CKPE.Installer.ApplicationVersion, CKPE.Installer.FileVersion, TOML.Types,
  TOML, TOML.Parser
  { you can add units after this };

{$R *.res}

begin
  try
    CoInitFlags := COINIT_MULTITHREADED;
    RequireDerivedFormResource:=True;
    Application.Scaled:=True;
    Application.Initialize;
    Application.CreateForm(TFormMainInstaller, FormMainInstaller);
    Application.Run;
  except
    on E: Exception do
      MessageBox(0, PChar(E.Message), 'Exception', MB_OK or MB_ICONERROR);
  end;
end.

