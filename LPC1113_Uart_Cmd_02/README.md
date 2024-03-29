# LPC1113_Uart_Cmd_02

Working application for NXP LPC1113F/301 MCU for gcc-toolchain on Visual Studio Code.
The receive function of Uart is realized using interrupts.
The VS-Code extensions CMake, CMake Tool and Cortex-Debug are used.
(only "buildType": "Debug" is realized)
Segger J-Link JTAG-Adapter was used.

You can enter commands through a terminal program (e.g. teraterm) on your PC which
is connected through a FTDI USB/Serial adapter (Baudrate 38400).
You can enter a command with max. 3 parameters separated by one or more spaces.

Commands:

'dump', followed by 'this' as second parameter
        or a memory address in hex like '0x00541234' 
        followed by the number of addresses to dump.
        'this' means that the values of the first 16 memory 
        addresses starting with the address
        of the command-name ('dump') are printed.

'stoptimer'  - stops the timer (which toggles a GPIO)

'starttimer' - starts the timer (which toggles a GPIO)

It's a working example but is not exensively tested.
As I'm not an experienced C programmer, the code may be of low quality.

![gallery](https://github.com/RoSchmi/VSCode_LPC1113/blob/master/LPC1113_Uart_Cmd_02/pictures/Console.png)


![gallery](https://github.com/RoSchmi/VSCode_LPC1113/blob/master/LPC1113_Uart_Cmd_02/pictures/Lpc1113%20(GHI%20DL40%20Module).png)

