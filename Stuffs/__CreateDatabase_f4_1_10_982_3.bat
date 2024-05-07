@REM creating a database
creationkit_f4_1_10_982_3 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_f4_1_10_982_3\*.relb) do creationkit_f4_1_10_982_3 -PEUpdateDatabase "%%I"