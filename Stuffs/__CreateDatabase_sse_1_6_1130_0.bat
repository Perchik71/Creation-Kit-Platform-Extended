@REM creating a database
CreationKit_se_1_6_1130 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_se_1_6_1130\*.relb) do CreationKit_se_1_6_1130 -PEUpdateDatabase "%%I"