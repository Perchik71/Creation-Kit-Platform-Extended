unit CKPE.Installer.FileVersion;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils;

type
  TFileVersionInfo = record
     Major : Cardinal;
     Minor : Cardinal;
     Revision : Cardinal;
     Build : Cardinal;
  end;

function LGetFileVersion(const AFileName: String): TFileVersionInfo;

implementation

uses
  Windows;

function LGetFileVersion(const AFileName: String): TFileVersionInfo;
var
  { useful only as long as we don't need to touch different stack pages }
  buf: array[0..3071] of byte;
  bufp: pointer;
  fn: string;
  valsize, size: DWORD;
  h: DWORD;
  valrec: PVSFixedFileInfo;
begin
  result := Default(TFileVersionInfo);
  fn := AFileName;
  UniqueString(fn);
  size := GetFileVersionInfoSizeA(pchar(fn),@h);
  if size > SizeOf(buf) then
  begin
    GetMem(bufp, size);
    try
      if GetFileVersionInfoA(pchar(fn),h,size,bufp) then
        if VerQueryValue(bufp,'\',valrec,valsize) then
          with valrec^ do
          begin
            Result.Major:= HiWord(valrec^.dwFileVersionMS);
            Result.Minor:=LoWord(valrec^.dwFileVersionMS);
            Result.Build:=LoWord(valrec^.dwFileVersionLS);
            Result.Revision:=HiWord(valrec^.dwFileVersionLS);
          end;
    finally
      FreeMem(bufp);
    end;
  end
  else
  begin
    if GetFileVersionInfoA(pchar(fn),h,size,@buf) then
      if VerQueryValue(@buf,'\',valrec,valsize) then
      begin
        Result.Major:= HiWord(valrec^.dwFileVersionMS);
        Result.Minor:=LoWord(valrec^.dwFileVersionMS);
        Result.Build:=LoWord(valrec^.dwFileVersionLS);
        Result.Revision:=HiWord(valrec^.dwFileVersionLS);
      end;
  end;
end;

end.

