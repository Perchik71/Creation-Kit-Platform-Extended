@REM writing to the patch database
for %%I in (base_se_1_6_438\*.relb) do CreationKit_se_1_6_438 -PEUpdateDatabase "%%I"