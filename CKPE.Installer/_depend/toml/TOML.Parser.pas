{ TOML Parser unit that handles parsing of TOML format data.
  This unit implements a lexer and parser for the TOML format specification.
  
  The parser follows the TOML v1.0.0 specification and supports all TOML data types:
  - Basic key/value pairs with string, integer, float, boolean, and datetime values
  - Tables and inline tables for structured data
  - Arrays of any valid TOML type
  - Basic strings and literal strings with proper escaping
  - Numbers in decimal, hexadecimal, octal, and binary formats
  - Dates and times in RFC 3339 format
  
  The parsing process is done in two stages:
  1. Lexical analysis (TTOMLLexer) - converts input text into tokens
  2. Syntactic analysis (TTOMLParser) - converts tokens into TOML data structures
}
unit TOML.Parser;

{$mode objfpc}{$H+}{$J-}

interface

uses
  SysUtils, Classes, TOML.Types, Generics.Collections, TypInfo, DateUtils, Math;

type
  { Token types used during lexical analysis
    Each token represents a meaningful unit in the TOML syntax }
  TTokenType = (
    ttEOF,        // End of file marker
    ttString,     // String literal (basic or literal)
    ttInteger,    // Integer number (decimal, hex, octal, binary)
    ttFloat,      // Floating point number (with optional exponent)
    ttBoolean,    // Boolean value (true/false)
    ttDateTime,   // Date/time value (RFC 3339)
    ttEqual,      // Equal sign (=)
    ttDot,        // Dot for nested keys (.)
    ttComma,      // Comma separator (,)
    ttLBracket,   // Left bracket ([)
    ttRBracket,   // Right bracket (])
    ttLBrace,     // Left brace ({)
    ttRBrace,     // Right brace (})
    ttNewLine,    // Line break
    ttWhitespace, // Whitespace characters
    ttComment,    // Comment (# or ##)
    ttIdentifier  // Key identifier
  );

  { Token record that stores lexical token information }
  TToken = record
    TokenType: TTokenType;  // Type of the token
    Value: string;          // String value of the token
    Line: Integer;          // Line number (1-based)
    Column: Integer;        // Column number (1-based)
  end;

  { Key-Value pair type for TOML tables }
  TTOMLKeyValuePair = specialize TPair<string, TTOMLValue>;

  { Lexer class that performs lexical analysis of TOML input
    Converts raw TOML text into a sequence of tokens }
  TTOMLLexer = class
  private
    FInput: string;      // Input string to tokenize
    FPosition: Integer;  // Current position in input
    FLine: Integer;      // Current line number (1-based)
    FColumn: Integer;    // Current column number (1-based)
    
    { Checks if we've reached the end of input
      @returns True if at end, False otherwise }
    function IsAtEnd: Boolean;
    
    { Peeks at current character without advancing position
      @returns Current character or #0 if at end }
    function Peek: Char;
    
    { Peeks at next character without advancing position
      @returns Next character or #0 if at end }
    function PeekNext: Char;
    
    { Advances position and returns current character
      @returns Current character or #0 if at end }
    function Advance: Char;
    
    { Skips whitespace and comments in the input }
    procedure SkipWhitespace;
    
    { Scans a string token (basic or literal)
      @returns The scanned string token
      @raises ETOMLParserException if string is malformed }
    function ScanString: TToken;
    
    { Scans a number token (integer or float)
      @returns The scanned number token
      @raises ETOMLParserException if number is malformed }
    function ScanNumber: TToken;
    
    { Scans an identifier token
      @returns The scanned identifier token }
    function ScanIdentifier: TToken;
    
    { Scans a datetime token
      @returns The scanned datetime token
      @raises ETOMLParserException if datetime is malformed }
    function ScanDateTime: TToken;
    
    { Character classification helper functions }
    
    { Checks if character is a digit (0-9)
      @param C Character to check
      @returns True if digit, False otherwise }
    function IsDigit(C: Char): Boolean;
    
    { Checks if character is alphabetic (a-z, A-Z)
      @param C Character to check
      @returns True if alphabetic, False otherwise }
    function IsAlpha(C: Char): Boolean;
    
    { Checks if character is alphanumeric (a-z, A-Z, 0-9)
      @param C Character to check
      @returns True if alphanumeric, False otherwise }
    function IsAlphaNumeric(C: Char): Boolean;
  public
    { Creates a new lexer instance
      @param AInput The TOML input string to tokenize }
    constructor Create(const AInput: string);
    
    { Gets the next token from input
      @returns The next token
      @raises ETOMLParserException if invalid input encountered }
    function NextToken: TToken;
  end;

  { Parser class that performs syntactic analysis of TOML input
    Converts tokens into TOML data structures }
  TTOMLParser = class
  private
    FLexer: TTOMLLexer;           // Lexer instance
    FCurrentToken: TToken;         // Current token being processed
    FPeekedToken: TToken;         // Next token (if peeked)
    FHasPeeked: Boolean;          // Whether we have a peeked token
    
    { Advances to next token }
    procedure Advance;
    
    { Peeks at next token without advancing
      @returns The next token }
    function Peek: TToken;
    
    { Checks if current token matches expected type
      @param TokenType Expected token type
      @returns True and advances if matches, False otherwise }
    function Match(TokenType: TTokenType): Boolean;
    
    { Expects current token to be of specific type
      @param TokenType Expected token type
      @raises ETOMLParserException if token doesn't match }
    procedure Expect(TokenType: TTokenType);
    
    { Parsing methods for different TOML constructs }
    
    { Parses a TOML value
      @returns The parsed value
      @raises ETOMLParserException on parse error }
    function ParseValue: TTOMLValue;
    
    { Parses a string value
      @returns The parsed string value
      @raises ETOMLParserException on parse error }
    function ParseString: TTOMLString;
    
    { Parses a number value (integer or float)
      @returns The parsed number value
      @raises ETOMLParserException on parse error }
    function ParseNumber: TTOMLValue;
    
    { Parses a boolean value
      @returns The parsed boolean value
      @raises ETOMLParserException on parse error }
    function ParseBoolean: TTOMLBoolean;
    
    { Parses a datetime value
      @returns The parsed datetime value
      @raises ETOMLParserException on parse error }
    function ParseDateTime: TTOMLDateTime;
    
    { Parses an array value
      @returns The parsed array value
      @raises ETOMLParserException on parse error }
    function ParseArray: TTOMLArray;
    
    { Parses an inline table value
      @returns The parsed table value
      @raises ETOMLParserException on parse error }
    function ParseInlineTable: TTOMLTable;
    
    { Parses a key (bare or quoted)
      @returns The parsed key string
      @raises ETOMLParserException on parse error }
    function ParseKey: string;
    
    { Parses a key-value pair
      @returns The parsed key-value pair
      @raises ETOMLParserException on parse error }
    function ParseKeyValue: TTOMLKeyValuePair;
  public
    { Creates a new parser instance
      @param AInput The TOML input string to parse }
    constructor Create(const AInput: string);
    destructor Destroy; override;
    
    { Parses the input and returns a TOML table
      @returns The parsed TOML table
      @raises ETOMLParserException on parse error }
    function Parse: TTOMLTable;
  end;

{ Helper functions }

{ Parses a TOML string into a table
  @param ATOML The TOML string to parse
  @returns The parsed TOML table
  @raises ETOMLParserException on parse error }
function ParseTOMLString(const ATOML: string): TTOMLTable;

{ Parses a TOML file into a table
  @param AFileName The file to parse
  @returns The parsed TOML table
  @raises ETOMLParserException on parse error
  @raises EFileStreamError if file cannot be opened }
function ParseTOMLFile(const AFileName: string): TTOMLTable;

implementation

{ Helper functions }

function ParseTOMLString(const ATOML: string): TTOMLTable;
var
  Parser: TTOMLParser;
begin
  Parser := TTOMLParser.Create(ATOML);
  try
    Result := Parser.Parse;
  finally
    Parser.Free;
  end;
end;

function ParseTOMLFile(const AFileName: string): TTOMLTable;
var
  FileStream: TFileStream;
  StringStream: TStringStream;
begin
  FileStream := TFileStream.Create(AFileName, fmOpenRead or fmShareDenyWrite);
  try
    StringStream := TStringStream.Create('');
    try
      StringStream.CopyFrom(FileStream, 0);
      Result := ParseTOMLString(StringStream.DataString);
    finally
      StringStream.Free;
    end;
  finally
    FileStream.Free;
  end;
end;

{ TTOMLLexer }

constructor TTOMLLexer.Create(const AInput: string);
begin
  inherited Create;
  FInput := AInput;
  FPosition := 1;
  FLine := 1;
  FColumn := 1;
end;

function TTOMLLexer.IsAtEnd: Boolean;
begin
  Result := FPosition > Length(FInput);
end;

function TTOMLLexer.Peek: Char;
begin
  if IsAtEnd then
    Result := #0
  else
    Result := FInput[FPosition];
end;

function TTOMLLexer.PeekNext: Char;
begin
  if FPosition + 1 > Length(FInput) then
    Result := #0
  else
    Result := FInput[FPosition + 1];
end;

function TTOMLLexer.Advance: Char;
begin
  if not IsAtEnd then
  begin
    Result := FInput[FPosition];
    Inc(FPosition);
    Inc(FColumn);
    if Result = #10 then
    begin
      Inc(FLine);
      FColumn := 1;
    end;
  end
  else
    Result := #0;
end;

procedure TTOMLLexer.SkipWhitespace;
begin
  while not IsAtEnd do
  begin
    case Peek of
      ' ', #9: Advance;
      '#': begin
        while (not IsAtEnd) and (Peek <> #10) do
          Advance;
      end;
      else
        Break;
    end;
  end;
end;

function TTOMLLexer.IsDigit(C: Char): Boolean;
begin
  Result := C in ['0'..'9'];
end;

function TTOMLLexer.IsAlpha(C: Char): Boolean;
begin
  Result := (C in ['a'..'z']) or (C in ['A'..'Z']) or (C = '_');
end;

function TTOMLLexer.IsAlphaNumeric(C: Char): Boolean;
begin
  Result := IsAlpha(C) or IsDigit(C);
end;

function TTOMLLexer.ScanString: TToken;
var
  IsMultiline: Boolean;
  IsLiteral: Boolean;
  QuoteChar: Char;
  StartColumn: Integer;
  TempValue: string;
begin
  IsMultiline := False;
  IsLiteral := False;
  StartColumn := FColumn;
  QuoteChar := Peek;
  IsLiteral := QuoteChar = '''';
  Advance; // Skip opening quote
  
  // Check for multiline string
  if (Peek = QuoteChar) and (PeekNext = QuoteChar) then
  begin
    IsMultiline := True;
    Advance; // Skip second quote
    Advance; // Skip third quote
    if not IsLiteral then
      // Skip first newline in multiline basic strings
      if (Peek = #10) or ((Peek = #13) and (PeekNext = #10)) then
      begin
        if Peek = #13 then Advance;
        if Peek = #10 then Advance;
      end;
  end;
  
  TempValue := '';
  try
    while not IsAtEnd do
    begin
      if IsMultiline then
      begin
        if (Peek = QuoteChar) and (PeekNext = QuoteChar) and 
           (FPosition + 2 <= Length(FInput)) and (FInput[FPosition + 2] = QuoteChar) then
        begin
          Advance; // Skip first quote
          Advance; // Skip second quote
          Advance; // Skip third quote
          Break;
        end;
      end
      else if Peek = QuoteChar then
      begin
        Advance;
        Break;
      end;
      
      if (not IsLiteral) and (Peek = '\') then
      begin
        Advance; // Skip backslash
        case Peek of
          'n': TempValue := TempValue + #10;
          't': TempValue := TempValue + #9;
          'r': TempValue := TempValue + #13;
          '\': TempValue := TempValue + '\';
          '"': TempValue := TempValue + '"';
          '''': TempValue := TempValue + '''';
          'u', 'U': begin
            // Handle Unicode escapes
            // TODO: Implement Unicode escape sequences
            raise ETOMLParserException.Create('Unicode escapes not yet implemented');
          end;
          else raise ETOMLParserException.Create('Invalid escape sequence');
        end;
        Advance;
      end
      else
        TempValue := TempValue + Advance;
    end;
    
    Result.TokenType := ttString;
    Result.Value := TempValue;
    Result.Line := FLine;
    Result.Column := StartColumn;
  except
    on E: Exception do
    begin
      Result.TokenType := ttString;
      Result.Value := '';
      Result.Line := FLine;
      Result.Column := StartColumn;
      raise;
    end;
  end;
end;

function TTOMLLexer.ScanNumber: TToken;
var
  IsFloat: Boolean;
  StartColumn: Integer;
  TempValue: string;
  Ch: Char;
  
  function IsHexDigit(C: Char): Boolean;
  begin
    Result := IsDigit(C) or (C in ['A'..'F', 'a'..'f']);
  end;
  
  function IsBinDigit(C: Char): Boolean;
  begin
    Result := C in ['0', '1'];
  end;
  
  function IsOctDigit(C: Char): Boolean;
  begin
    Result := C in ['0'..'7'];
  end;
  
begin
  IsFloat := False;
  StartColumn := FColumn;
  TempValue := '';
  
  // Handle sign
  if Peek in ['+', '-'] then
    TempValue := TempValue + Advance;
  
  // Check for special float values (inf, nan)
  if (Peek = 'i') then
  begin
    // Check for 'inf'
    TempValue := TempValue + Advance;  // 'i'
    if (Peek = 'n') then
    begin
      TempValue := TempValue + Advance;  // 'n'
      if Peek = 'f' then
      begin
        TempValue := TempValue + Advance;  // 'f'
        Result.TokenType := ttFloat;
        Result.Value := TempValue;
        Result.Line := FLine;
        Result.Column := StartColumn;
        Exit;
      end;
    end;
  end
  else if (Peek = 'n') then
  begin
    // Check for 'nan'
    TempValue := TempValue + Advance;  // 'n'
    if (Peek = 'a') then
    begin
      TempValue := TempValue + Advance;  // 'a'
      if Peek = 'n' then
      begin
        TempValue := TempValue + Advance;  // 'n'
        Result.TokenType := ttFloat;
        Result.Value := TempValue;
        Result.Line := FLine;
        Result.Column := StartColumn;
        Exit;
      end;
    end;
  end;
  
  // Check for hex, octal, or binary
  if (Peek = '0') and not IsAtEnd then
  begin
    Ch := UpCase(PeekNext);
    if Ch in ['X', 'O', 'B'] then
    begin
      TempValue := TempValue + Advance; // '0'
      TempValue := TempValue + Advance; // 'x', 'o', or 'b'
      
      case Ch of
        'X': while not IsAtEnd and (IsHexDigit(Peek) or (Peek = '_')) do
               if Peek <> '_' then TempValue := TempValue + Advance
               else Advance;
               
        'O': while not IsAtEnd and (IsOctDigit(Peek) or (Peek = '_')) do
               if Peek <> '_' then TempValue := TempValue + Advance
               else Advance;
               
        'B': while not IsAtEnd and (IsBinDigit(Peek) or (Peek = '_')) do
               if Peek <> '_' then TempValue := TempValue + Advance
               else Advance;
      end;
      
      Result.TokenType := ttInteger;
      Result.Value := TempValue;
      Result.Line := FLine;
      Result.Column := StartColumn;
      Exit;
    end;
  end;
  
  // Scan integer part
  while not IsAtEnd and (IsDigit(Peek) or (Peek = '_')) do
    if Peek <> '_' then
      TempValue := TempValue + Advance
    else
      Advance;
  
  // Check for decimal point
  if (Peek = '.') and IsDigit(PeekNext) then
  begin
    IsFloat := True;
    TempValue := TempValue + Advance; // Add decimal point
    
    // Scan decimal part
    while not IsAtEnd and (IsDigit(Peek) or (Peek = '_')) do
      if Peek <> '_' then
        TempValue := TempValue + Advance
      else
        Advance;
  end;
  
  // Check for exponent
  if Peek in ['e', 'E'] then
  begin
    IsFloat := True;
    TempValue := TempValue + Advance;
    
    if Peek in ['+', '-'] then
      TempValue := TempValue + Advance;
      
    while not IsAtEnd and (IsDigit(Peek) or (Peek = '_')) do
      if Peek <> '_' then
        TempValue := TempValue + Advance
      else
        Advance;
  end;
  
  if IsFloat then
    Result.TokenType := ttFloat
  else
    Result.TokenType := ttInteger;
    
  Result.Value := TempValue;
  Result.Line := FLine;
  Result.Column := StartColumn;
end;

function TTOMLLexer.ScanIdentifier: TToken;
var
  StartColumn: Integer;
begin
  StartColumn := FColumn;
  Result.Value := '';
  
  while not IsAtEnd and (IsAlphaNumeric(Peek) or (Peek = '-')) do
    Result.Value := Result.Value + Advance;
    
  Result.TokenType := ttIdentifier;
  Result.Line := FLine;
  Result.Column := StartColumn;
end;

function TTOMLLexer.ScanDateTime: TToken;
var
  StartColumn: Integer;
  i: Integer;
  HasTime: Boolean;
  HasTimezone: Boolean;
  HasDate: Boolean;
  TempValue: string;
  
  function ScanDigits(Count: Integer): Boolean;
  var
    i: Integer;
  begin
    Result := True;
    for i := 1 to Count do
    begin
      if not IsDigit(Peek) then
      begin
        Result := False;
        Exit;
      end;
      TempValue := TempValue + Advance;
    end;
  end;
  
begin
  StartColumn := FColumn;
  TempValue := '';
  HasDate := False;
  HasTime := False;
  HasTimezone := False;
  
  // Try to parse as date (YYYY-MM-DD)
  if ScanDigits(4) and (Peek = '-') then
  begin
    TempValue := TempValue + Advance; // -
    if ScanDigits(2) and (Peek = '-') then
    begin
      TempValue := TempValue + Advance; // -
      if ScanDigits(2) then
        HasDate := True;
    end;
  end;
  
  // Try to parse time (HH:MM:SS[.fraction])
  if HasDate and (Peek = 'T') then
  begin
    TempValue := TempValue + Advance; // T
    if ScanDigits(2) and (Peek = ':') then
    begin
      TempValue := TempValue + Advance; // :
      if ScanDigits(2) and (Peek = ':') then
      begin
        TempValue := TempValue + Advance; // :
        if ScanDigits(2) then
        begin
          HasTime := True;
          
          // Optional fractional seconds
          if Peek = '.' then
          begin
            TempValue := TempValue + Advance; // .
            while IsDigit(Peek) do
              TempValue := TempValue + Advance;
          end;
        end;
      end;
    end;
  end
  else if not HasDate then
  begin
    // Try to parse as time only (HH:MM:SS[.fraction])
    if ScanDigits(2) and (Peek = ':') then
    begin
      TempValue := TempValue + Advance; // :
      if ScanDigits(2) and (Peek = ':') then
      begin
        TempValue := TempValue + Advance; // :
        if ScanDigits(2) then
        begin
          HasTime := True;
          
          // Optional fractional seconds
          if Peek = '.' then
          begin
            TempValue := TempValue + Advance; // .
            while IsDigit(Peek) do
              TempValue := TempValue + Advance;
          end;
        end;
      end;
    end;
  end;
  
  // Try to parse timezone
  if HasTime and (Peek in ['Z', '+', '-']) then
  begin
    if Peek = 'Z' then
    begin
      TempValue := TempValue + Advance;
      HasTimezone := True;
    end
    else
    begin
      TempValue := TempValue + Advance; // + or -
      if ScanDigits(2) then
      begin
        if Peek = ':' then
        begin
          TempValue := TempValue + Advance; // :
          if ScanDigits(2) then
            HasTimezone := True;
        end
        else
          HasTimezone := True;
      end;
    end;
  end;
  
  // Determine token type based on what we found
  if HasDate and HasTime and HasTimezone then
    Result.TokenType := ttDateTime
  else if HasDate and HasTime then
    Result.TokenType := ttDateTime
  else if HasDate then
    Result.TokenType := ttDateTime
  else if HasTime then
    Result.TokenType := ttDateTime
  else
    Result.TokenType := ttInteger;
  
  Result.Value := TempValue;
  Result.Line := FLine;
  Result.Column := StartColumn;
end;

function TTOMLLexer.NextToken: TToken;
var
  SavePos: Integer;
  SaveLine: Integer;
  SaveCol: Integer;
begin
  SkipWhitespace;
  
  if IsAtEnd then
  begin
    Result.TokenType := ttEOF;
    Result.Value := '';
    Result.Line := FLine;
    Result.Column := FColumn;
    Exit;
  end;
  
  case Peek of
    '=': begin
      Advance;
      Result.TokenType := ttEqual;
      Result.Value := '=';
    end;
    '.': begin
      Advance;
      Result.TokenType := ttDot;
      Result.Value := '.';
    end;
    ',': begin
      Advance;
      Result.TokenType := ttComma;
      Result.Value := ',';
    end;
    '[': begin
      Advance;
      Result.TokenType := ttLBracket;
      Result.Value := '[';
    end;
    ']': begin
      Advance;
      Result.TokenType := ttRBracket;
      Result.Value := ']';
    end;
    '{': begin
      Advance;
      Result.TokenType := ttLBrace;
      Result.Value := '{';
    end;
    '}': begin
      Advance;
      Result.TokenType := ttRBrace;
      Result.Value := '}';
    end;
    #10, #13: begin
      if (Peek = #13) and (PeekNext = #10) then
        Advance; // Skip CR in CRLF
      Advance;
      Result.TokenType := ttNewLine;
      Result.Value := #10;
    end;
    '"', '''': Result := ScanString;
    '0'..'9': begin
      // Save current position
      SavePos := FPosition;
      SaveLine := FLine;
      SaveCol := FColumn;
      
      // Try to scan as DateTime first
      Result := ScanDateTime;
      
      // If not a DateTime, restore position and try as number
      if Result.TokenType <> ttDateTime then
      begin
        FPosition := SavePos;
        FLine := SaveLine;
        FColumn := SaveCol;
        Result := ScanNumber;
      end;
    end;
    '+', '-': Result := ScanNumber;
    else
      if IsAlpha(Peek) then
      begin
        // Save current position
        SavePos := FPosition;
        SaveLine := FLine;
        SaveCol := FColumn;
        
        Result := ScanIdentifier;
        
        // Check if it's a special float value
        if (Result.Value = 'inf') or (Result.Value = 'nan') then
        begin
          Result.TokenType := ttFloat;
        end;
      end
      else
        raise ETOMLParserException.CreateFmt('Unexpected character: %s at line %d, column %d',
          [Peek, FLine, FColumn]);
  end;
  
  Result.Line := FLine;
  Result.Column := FColumn;
end;

{ TTOMLParser }

constructor TTOMLParser.Create(const AInput: string);
begin
  inherited Create;
  FLexer := TTOMLLexer.Create(AInput);
  FHasPeeked := False;
  Advance;
end;

destructor TTOMLParser.Destroy;
begin
  FLexer.Free;
  inherited;
end;

procedure TTOMLParser.Advance;
begin
  if FHasPeeked then
  begin
    FCurrentToken := FPeekedToken;
    FHasPeeked := False;
  end
  else
    FCurrentToken := FLexer.NextToken;
end;

function TTOMLParser.Peek: TToken;
begin
  if not FHasPeeked then
  begin
    FPeekedToken := FLexer.NextToken;
    FHasPeeked := True;
  end;
  Result := FPeekedToken;
end;

function TTOMLParser.Match(TokenType: TTokenType): Boolean;
begin
  if FCurrentToken.TokenType = TokenType then
  begin
    Advance;
    Result := True;
  end
  else
    Result := False;
end;

procedure TTOMLParser.Expect(TokenType: TTokenType);
begin
  if FCurrentToken.TokenType <> TokenType then
    raise ETOMLParserException.CreateFmt('Expected token type %s but got %s at line %d, column %d',
      [GetEnumName(TypeInfo(TTokenType), Ord(TokenType)),
       GetEnumName(TypeInfo(TTokenType), Ord(FCurrentToken.TokenType)),
       FCurrentToken.Line, FCurrentToken.Column]);
  Advance;
end;

function TTOMLParser.ParseValue: TTOMLValue;
begin
  case FCurrentToken.TokenType of
    ttString: Result := ParseString;
    ttDateTime: begin
      try
        Result := ParseDateTime;
      except
        on E: ETOMLParserException do
          raise;
        on E: Exception do
          raise ETOMLParserException.CreateFmt('Error parsing DateTime: %s at line %d, column %d',
            [E.Message, FCurrentToken.Line, FCurrentToken.Column]);
      end;
    end;
    ttInteger, ttFloat: Result := ParseNumber;
    ttIdentifier:
      if SameText(FCurrentToken.Value, 'true') or SameText(FCurrentToken.Value, 'false') then
        Result := ParseBoolean
      else
        raise ETOMLParserException.CreateFmt('Unexpected identifier: %s at line %d, column %d',
          [FCurrentToken.Value, FCurrentToken.Line, FCurrentToken.Column]);
    ttLBracket: Result := ParseArray;
    ttLBrace: Result := ParseInlineTable;
    else
      raise ETOMLParserException.CreateFmt('Unexpected token type: %s at line %d, column %d',
        [GetEnumName(TypeInfo(TTokenType), Ord(FCurrentToken.TokenType)),
         FCurrentToken.Line, FCurrentToken.Column]);
  end;
end;

function TTOMLParser.ParseString: TTOMLString;
begin
  Result := TTOMLString.Create(FCurrentToken.Value);
  Advance;
end;

function TTOMLParser.ParseNumber: TTOMLValue;
var
  Value: string;
  Code: Integer;
  IntValue: Int64;
  FloatValue: Double;
  IsNegative: Boolean;
  BaseValue: string;
  i: Integer;
begin
  Value := FCurrentToken.Value;
  
  // Handle special float values
  if FCurrentToken.TokenType = ttFloat then
  begin
    // Remove underscores from the value
    i := 1;
    while i <= Length(Value) do
    begin
      if Value[i] = '_' then
        Delete(Value, i, 1)
      else
        Inc(i);
    end;
    
    // Check for special values
    if SameText(Value, 'inf') or SameText(Value, '+inf') then
      FloatValue := 1.0/0.0  // Creates positive infinity
    else if SameText(Value, '-inf') then
      FloatValue := -1.0/0.0  // Creates negative infinity
    else if SameText(Value, 'nan') or SameText(Value, '+nan') or SameText(Value, '-nan') then
      FloatValue := 0.0/0.0  // Creates NaN
    else
    begin
      Val(Value, FloatValue, Code);
      if Code <> 0 then
        raise ETOMLParserException.CreateFmt('Invalid float value: %s at line %d, column %d',
          [Value, FCurrentToken.Line, FCurrentToken.Column]);
    end;
    Result := TTOMLFloat.Create(FloatValue);
  end
  else // Integer handling
  begin
    // Remove underscores from the value
    i := 1;
    while i <= Length(Value) do
    begin
      if Value[i] = '_' then
        Delete(Value, i, 1)
      else
        Inc(i);
    end;
    
    IsNegative := (Value <> '') and (Value[1] = '-');
    if IsNegative then
      Delete(Value, 1, 1);
      
    if (Length(Value) >= 2) and (Value[1] = '0') then
    begin
      case UpCase(Value[2]) of
        'X': begin // Hex
          BaseValue := '$' + Copy(Value, 3, Length(Value));
          Val(BaseValue, IntValue, Code);
        end;
        'O': begin // Octal
          BaseValue := '&' + Copy(Value, 3, Length(Value));
          Val(BaseValue, IntValue, Code);
        end;
        'B': begin // Binary
          BaseValue := '%' + Copy(Value, 3, Length(Value));
          Val(BaseValue, IntValue, Code);
        end;
        else begin // Decimal
          Val(Value, IntValue, Code);
        end;
      end;
    end
    else
      Val(Value, IntValue, Code);
      
    if Code = 0 then
    begin
      if IsNegative then
        IntValue := -IntValue;
      Result := TTOMLInteger.Create(IntValue);
    end
    else
      raise ETOMLParserException.CreateFmt('Invalid integer value: %s at line %d, column %d',
        [Value, FCurrentToken.Line, FCurrentToken.Column]);
  end;
  
  Advance;
end;

function TTOMLParser.ParseBoolean: TTOMLBoolean;
begin
  Result := TTOMLBoolean.Create(SameText(FCurrentToken.Value, 'true'));
  Advance;
end;

function TTOMLParser.ParseDateTime: TTOMLDateTime;
var
  DateStr: string;
  Year, Month, Day, Hour, Minute, Second: Word;
  MilliSecond: Word;
  TZHour, TZMinute: Integer;
  TZNegative: Boolean;
  P: Integer;
  FracStr: string;
  DT: TDateTime;
  HasDate, HasTime: Boolean;
begin
  if FCurrentToken.TokenType <> ttDateTime then
    raise ETOMLParserException.CreateFmt('Expected DateTime but got %s at line %d, column %d',
      [GetEnumName(TypeInfo(TTokenType), Ord(FCurrentToken.TokenType)),
       FCurrentToken.Line, FCurrentToken.Column]);

  DateStr := FCurrentToken.Value;
  HasDate := False;
  HasTime := False;
  
  try
    // Initialize all components to 0
    Year := 0;
    Month := 0;
    Day := 0;
    Hour := 0;
    Minute := 0;
    Second := 0;
    MilliSecond := 0;
    
    P := 1;
    
    // Try to parse date part (YYYY-MM-DD)
    if (Length(DateStr) >= 10) and (DateStr[5] = '-') and (DateStr[8] = '-') then
    begin
      Year := StrToInt(Copy(DateStr, 1, 4));
      Month := StrToInt(Copy(DateStr, 6, 2));
      Day := StrToInt(Copy(DateStr, 9, 2));
      HasDate := True;
      P := 11;
    end;
    
    // Try to parse time part (HH:MM:SS[.fraction])
    if (P <= Length(DateStr)) and ((DateStr[P] = 'T') or not HasDate) then
    begin
      if DateStr[P] = 'T' then Inc(P);
      
      if (P + 7 <= Length(DateStr)) and (DateStr[P+2] = ':') and (DateStr[P+5] = ':') then
      begin
        Hour := StrToInt(Copy(DateStr, P, 2));
        Minute := StrToInt(Copy(DateStr, P+3, 2));
        Second := StrToInt(Copy(DateStr, P+6, 2));
        HasTime := True;
        P := P + 8;
        
        // Parse fractional seconds if present
        if (P <= Length(DateStr)) and (DateStr[P] = '.') then
        begin
          Inc(P);
          FracStr := '';
          while (P <= Length(DateStr)) and (DateStr[P] in ['0'..'9']) do
          begin
            FracStr := FracStr + DateStr[P];
            Inc(P);
          end;
          if Length(FracStr) > 0 then
            MilliSecond := StrToInt(Copy(FracStr + '000', 1, 3));
        end;
      end;
    end
    else if not HasDate then
    begin
      // Try to parse as time only (HH:MM:SS[.fraction])
      if (P <= Length(DateStr)) and (DateStr[P] = 'T') then
      begin
        Inc(P);
        if (P + 7 <= Length(DateStr)) and (DateStr[P+2] = ':') and (DateStr[P+5] = ':') then
        begin
          Hour := StrToInt(Copy(DateStr, P, 2));
          Minute := StrToInt(Copy(DateStr, P+3, 2));
          Second := StrToInt(Copy(DateStr, P+6, 2));
          HasTime := True;
          P := P + 8;
          
          // Parse fractional seconds if present
          if (P <= Length(DateStr)) and (DateStr[P] = '.') then
          begin
            Inc(P);
            FracStr := '';
            while (P <= Length(DateStr)) and (DateStr[P] in ['0'..'9']) do
            begin
              FracStr := FracStr + DateStr[P];
              Inc(P);
            end;
            if Length(FracStr) > 0 then
              MilliSecond := StrToInt(Copy(FracStr + '000', 1, 3));
          end;
        end;
      end;
    end;
    
    // Create DateTime value
    if HasDate then
      DT := EncodeDate(Year, Month, Day)
    else
      DT := 0;
      
    if HasTime then
      DT := DT + EncodeTime(Hour, Minute, Second, MilliSecond);
      
    if not (HasDate or HasTime) then
      raise ETOMLParserException.CreateFmt('Invalid datetime format: %s at line %d, column %d',
        [DateStr, FCurrentToken.Line, FCurrentToken.Column]);
        
    Result := TTOMLDateTime.Create(DT);
  except
    on E: Exception do
      raise ETOMLParserException.CreateFmt('Error parsing datetime: %s at line %d, column %d',
        [E.Message, FCurrentToken.Line, FCurrentToken.Column]);
  end;
  
  Advance;
end;

function TTOMLParser.ParseArray: TTOMLArray;
var
  ItemValue: TTOMLValue;
  HasNewline: Boolean;
begin
  Result := TTOMLArray.Create;
  try
    Expect(ttLBracket);
    
    if FCurrentToken.TokenType <> ttRBracket then
    begin
      repeat
        // Skip any newlines between array elements
        HasNewline := False;
        while FCurrentToken.TokenType = ttNewLine do
        begin
          HasNewline := True;
          Advance;
        end;

        ItemValue := ParseValue;
        Result.Add(ItemValue);
        
        // Skip any newlines after array elements before comma
        while FCurrentToken.TokenType = ttNewLine do
        begin
          HasNewline := True;
          Advance;
        end
      until not Match(ttComma);
      
      // Skip any newlines before closing bracket
      while FCurrentToken.TokenType = ttNewLine do
        Advance;
    end;
    
    Expect(ttRBracket);
  except
    Result.Free;
    raise;
  end;
end;

function TTOMLParser.ParseInlineTable: TTOMLTable;
begin
  Result := TTOMLTable.Create;
  try
    Expect(ttLBrace);
    
    if FCurrentToken.TokenType <> ttRBrace then
    begin
      repeat
        with ParseKeyValue do
          Result.Add(Key, Value);
      until not Match(ttComma);
    end;
    
    Expect(ttRBrace);
  except
    Result.Free;
    raise;
  end;
end;

function TTOMLParser.ParseKey: string;
begin
  if FCurrentToken.TokenType = ttString then
  begin
    Result := FCurrentToken.Value;
    Advance;
  end
  else if FCurrentToken.TokenType = ttIdentifier then
  begin
    Result := FCurrentToken.Value;
    Advance;
  end
  else
    raise ETOMLParserException.CreateFmt('Expected string or identifier but got %s at line %d, column %d',
      [GetEnumName(TypeInfo(TTokenType), Ord(FCurrentToken.TokenType)),
       FCurrentToken.Line, FCurrentToken.Column]);
end;

function TTOMLParser.ParseKeyValue: TTOMLKeyValuePair;
var
  Key: string;
  Value: TTOMLValue;
begin
  Key := ParseKey;
  Value := nil;
  
  try
    while Match(ttDot) do
      Key := Key + '.' + ParseKey;
      
    Expect(ttEqual);
    Value := ParseValue;
    Result := TTOMLKeyValuePair.Create(Key, Value);
  except
    Value.Free;
    raise;
  end;
end;

function TTOMLParser.Parse: TTOMLTable;
var
  CurrentTable: TTOMLTable;
  TablePath: TStringList;
  i: Integer;
  Key: string;
  Value: TTOMLValue;
  KeyPair: TTOMLKeyValuePair;
  IsArrayOfTables: Boolean;
  ArrayValue: TTOMLArray;
  NewTable: TTOMLTable;
begin
  Result := TTOMLTable.Create;
  try
    CurrentTable := Result;
    TablePath := TStringList.Create;
    try
      while FCurrentToken.TokenType <> ttEOF do
      begin
        case FCurrentToken.TokenType of
          ttLBracket:
          begin
            IsArrayOfTables := False;
            Advance;
            
            // Check for array of tables
            if FCurrentToken.TokenType = ttLBracket then
            begin
              IsArrayOfTables := True;
              Advance;
            end;
            
            TablePath.Clear;
            repeat
              TablePath.Add(ParseKey);
            until not Match(ttDot);
            
            Expect(ttRBracket);
            if IsArrayOfTables then
              Expect(ttRBracket);
            
            // Navigate to the correct table
            CurrentTable := Result;
            for i := 0 to TablePath.Count - 2 do
            begin
              Key := TablePath[i];
              if not CurrentTable.TryGetValue(Key, Value) then
              begin
                Value := TTOMLTable.Create;
                CurrentTable.Add(Key, Value);
              end;
              if not (Value is TTOMLTable) then
                raise ETOMLParserException.CreateFmt('Key %s is not a table at line %d, column %d',
                  [Key, FCurrentToken.Line, FCurrentToken.Column]);
              CurrentTable := TTOMLTable(Value);
            end;
            
            // Handle the last key differently for array of tables
            Key := TablePath[TablePath.Count - 1];
            if IsArrayOfTables then
            begin
              // Create or get the array
              if not CurrentTable.TryGetValue(Key, Value) then
              begin
                ArrayValue := TTOMLArray.Create;
                CurrentTable.Add(Key, ArrayValue);
                Value := ArrayValue;
              end;
              
              if not (Value is TTOMLArray) then
                raise ETOMLParserException.CreateFmt('Key %s is not an array at line %d, column %d',
                  [Key, FCurrentToken.Line, FCurrentToken.Column]);
                  
              // Add a new table to the array
              NewTable := TTOMLTable.Create;
              TTOMLArray(Value).Add(NewTable);
              CurrentTable := NewTable;
            end
            else
            begin
              // Regular table
              if not CurrentTable.TryGetValue(Key, Value) then
              begin
                Value := TTOMLTable.Create;
                CurrentTable.Add(Key, Value);
              end
              else if Value is TTOMLArray then
              begin
                // If it's an array, get the last table in the array
                ArrayValue := TTOMLArray(Value);
                if ArrayValue.Count = 0 then
                  raise ETOMLParserException.CreateFmt('Array %s is empty at line %d, column %d',
                    [Key, FCurrentToken.Line, FCurrentToken.Column]);
                Value := ArrayValue.Items[ArrayValue.Count - 1];
              end;
              
              if not (Value is TTOMLTable) then
                raise ETOMLParserException.CreateFmt('Key %s is not a table at line %d, column %d',
                  [Key, FCurrentToken.Line, FCurrentToken.Column]);
              CurrentTable := TTOMLTable(Value);
            end;
          end;
          
          ttIdentifier, ttString:
          begin
            try
              KeyPair := ParseKeyValue;
              try
                CurrentTable.Add(KeyPair.Key, KeyPair.Value);
              except
                KeyPair.Value.Free;
                raise;
              end;
            except
              on E: ETOMLParserException do
                raise;
              on E: Exception do
                raise ETOMLParserException.CreateFmt('Error adding key-value pair: %s at line %d, column %d',
                  [E.Message, FCurrentToken.Line, FCurrentToken.Column]);
            end;
          end;
          
          ttNewLine: Advance;
          
          else
            raise ETOMLParserException.CreateFmt('Unexpected token type: %s at line %d, column %d',
              [GetEnumName(TypeInfo(TTokenType), Ord(FCurrentToken.TokenType)),
               FCurrentToken.Line, FCurrentToken.Column]);
        end;
      end;
    finally
      TablePath.Free;
    end;
  except
    Result.Free;
    raise;
  end;
end;

end. 
