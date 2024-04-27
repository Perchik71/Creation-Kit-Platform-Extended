@REM creating a database
CreationKit_se_1_5_3 -PECreateDatabase
@REM writing to the patch database
for %%I in (base_se_1_5_3\*.relb) do CreationKit_se_1_5_3 -PEUpdateDatabase "%%I"