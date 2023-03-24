set HEXFILE=%1

set "HEXFILE=%HEXFILE:\=/%"
openocd -f ./tools/wch-interface.cfg -f ./tools/wch-target.cfg -c init -c halt  -c "program %HEXFILE% verify " -c reset -c wlink_reset_resume -c exit  

