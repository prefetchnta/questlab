@cd %1
@filehash.exe
@if not exist __hash__.old goto no_cmp
@TortoiseMerge __hash__.old __hash__.txt
:no_cmp
