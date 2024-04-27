@REM creating a database
creationkit_f4_1_10_162_0 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_f4_1_10_162_0\*.relb) do creationkit_f4_1_10_162_0 -PEUpdateDatabase "%%I"