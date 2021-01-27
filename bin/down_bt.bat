@echo off
%~dp0aria2c.exe %3 --dir=%2 --file-allocation=none --check-integrity=true --max-concurrent-downloads=16 --max-overall-upload-limit=10K --max-upload-limit=10K --enable-dht=true %1
