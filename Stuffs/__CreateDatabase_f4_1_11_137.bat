@REM creating a database
creationkit_f4_1_11_137 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_f4_1_11_137\*.relb) do creationkit_f4_1_11_137 -PEUpdateDatabase "%%I"