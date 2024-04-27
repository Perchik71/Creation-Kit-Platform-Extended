@REM writing to the patch database
for %%I in (base_f4_1_10_943_1\*.relb) do creationkit_f4_1_10_943_1 -PEUpdateDatabase "%%I"