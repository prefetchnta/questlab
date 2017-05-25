@echo off
cd inc
if exist crhack goto bat_exit
junction crhack D:\GIT\crhack\inc
:bat_exit
