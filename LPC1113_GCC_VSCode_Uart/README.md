# LPC1113_GCC_VSCode_Uart

Working simple Uart and Blinky example for LPC1113F/301 MCU for gcc-toolchain on Visual Studio Code.
The VS-Code extensions CMake, CMake Tool and Cortex-Debug are used
(only "buildType": "Debug" is realized)

Links:
https://github.com/Marus/cortex-debug

https://marcelball.ca/projects/cortex-debug/cortex-debug-launch-configurations/

Settings:
launch.json:

{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "jlink",
            "cwd": "${workspaceRoot}",
            "executable": "./build/LPC1113_gCC_VSCode_Uart.elf",
            "name": "Debug (J-Link)",
            "device": "LPC1113/301",
            "interface": "swd",
            "ipAddress": null,
            "serialNumber": null,
        }
    ]
}

{
    "C_Cpp.default.configurationProvider": "vector-of-bool.cmake-tools",
    "cortex-debug.JLinkGDBServerPath" : "C:/Program Files (x86)/SEGGER/JLink"
}




