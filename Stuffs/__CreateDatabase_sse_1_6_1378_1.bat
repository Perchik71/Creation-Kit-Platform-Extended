@REM creating a database
CreationKit_se_1_6_1378_1 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_se_1_6_1378_1\*.relb) do CreationKit_se_1_6_1378_1 -PEUpdateDatabase "%%I"