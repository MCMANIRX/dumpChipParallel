# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)


project(dump C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(dump
  #src/rc_mqtt.c
  main.c

  #src/wifi_temp2.c
  #src/motor_test.c
 # pwm.c
 #src/stream_test.c



)

target_include_directories(dump PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}


        
)

#target_sources(stoc PRIVATE ${PICO_SDK_PATH}/lib/lwip/src/apps/mqtt/mqtt.c)


target_link_libraries(dump
        pico_stdlib              
        pico_bootrom   

)

# enable usb output, disable uart output
pico_enable_stdio_usb(dump 1)
pico_enable_stdio_uart(dump 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(dump)
