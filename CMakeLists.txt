#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_VERBOSE_MAKEFILE on)
cmake_minimum_required(VERSION 3.25)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-gcc-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(USBx LANGUAGES C CXX ASM)

# project settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_AR> s <TARGET>")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_AR> s <TARGET>")

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)

add_compile_options(
	-mcpu=cortex-m7
	-mthumb
	-mfloat-abi=hard
	-mfpu=fpv5-d16
	-O0
	-fmessage-length=0
	-fsigned-char
	-ffunction-sections
	-fdata-sections
	-Wall
	-g3
)


add_compile_definitions(STM32H743xx)
add_compile_definitions(STM32H7)
add_compile_definitions(USE_RTOS=0)
add_compile_definitions(CUBE_INIT_CLK=1)
add_compile_definitions(TINY_USB_CLK=0)
add_compile_definitions(HSE_VALUE=8000000)
add_compile_definitions(BOARD_TUD_RHPORT=0)
add_compile_definitions(CFG_TUSB_MCU=OPT_MCU_STM32H7)
add_compile_definitions(LATHE_USB)

# set(LTO lto)
# -f${LTO}

add_compile_options(
)

include_directories(
        ./Core/Inc
        ./hw
        ./hw/bsp/stm32h7
        ./hw/bsp/stm32h7/boards/stm32h743nucleo
        ./hw/mcu/st/cmsis_device_h7/Include
        ./hw/mcu/st/stm32h7xx_hal_driver/Inc
        ./lib/CMSIS_5/CMSIS/Core/Include
        ./project
        ./project/include
        ./src
        ./hw/bsp
)

        # ./src/class/audio/audio_device.c
        # ../tinyusb/src/class/dfu/dfu_device.c
        # ../tinyusb/src/class/dfu/dfu_rt_device.c
        # ../tinyusb/src/class/hid/hid_device.c
        # ../tinyusb/src/class/midi/midi_device.c
        # ../tinyusb/src/class/msc/msc_device.c
        # ../tinyusb/src/class/net/ecm_rndis_device.c
        # ../tinyusb/src/class/net/ncm_device.c
        # ../tinyusb/src/class/usbtmc/usbtmc_device.c
        # ../tinyusb/src/class/vendor/vendor_device.c
        # ../tinyusb/src/class/video/video_device.c


 set(SOURCES
         ./Core/Startup/startup_stm32h743zitx.s
         ./Core/Src/system_stm32h7xx.c
         ./Core/src/main.c
         ./project/main.c
         ./project/fixSys.c
         ./project/usb_descriptors.c
         ./project/serialio.c
         ./project/trace.c
         ./project/stm32Info.c
         ./project/getSP.S
         ./src/portable/synopsys/dwc2/dcd_dwc2.c
         ./src/tusb.c
         ./src/common/tusb_fifo.c
         ./src/device/usbd.c
         ./src/device/usbd_control.c
         ./src/class/cdc/cdc_device.c
         ./src/class/vendor/vendor_device.c
         ./hw/bsp/stm32h7/family.c
         ./hw/bsp/board.c

	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_cortex.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_dma.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_rcc.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_rcc_ex.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_gpio.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_uart.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_uart_ex.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_pwr.c
	 ./hw/mcu/st/stm32h7xx_hal_driver/Src/stm32h7xx_hal_pwr_ex.c
)

add_subdirectory(systemlib)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H743ZITX_FLASH.ld)
add_link_options(-T ${LINKER_SCRIPT})

add_link_options(
		-Wl,-gc-sections
		-Wl,--print-memory-usage
		-Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
)
add_link_options(-Xlinker --cref)
add_link_options(-Wl,--no-warn-rwx-segments)
#add_link_options(-f${LTO})
add_link_options(
	-mcpu=cortex-m7
	-mthumb
	-mfloat-abi=hard
	-mfpu=fpv5-d16
	-O0
	-fmessage-length=0
	-fsigned-char
	-ffunction-sections
	-fdata-sections
	-Wall
	-g3
	--specs=nano.specs
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_link_directories(${PROJECT_NAME}.elf PRIVATE cmake-build-debug/systemLib)

target_link_libraries(${PROJECT_NAME}.elf
        -Wl,--whole-archive systemLib -Wl,--no-whole-archive)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
