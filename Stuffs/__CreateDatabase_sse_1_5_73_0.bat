@REM creating a database
CreationKit_se_1_5_73 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_se_1_5_73\*.relb) do CreationKit_se_1_5_73 -PEUpdateDatabase "%%I"