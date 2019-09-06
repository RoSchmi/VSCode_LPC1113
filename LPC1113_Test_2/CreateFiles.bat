rem Generate binary image file
arm-none-eabi-objcopy -O binary build/LPC1113_Test_2.elf build/LPC1113_Test_2.bin
if %ERRORLEVEL% NEQ 0 goto end

:end
pause