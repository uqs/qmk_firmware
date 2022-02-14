# MCU name
MCU = atmega32u4

BOOTLOADER = atmel-dfu

## Build Options
##   change yes to no to disable
##
#BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
#COMMAND_ENABLE = no         # Commands for debug and configuration
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow

# Specify firmware format that is copied to root qmk_firmware folder
FIRMWARE_FORMAT = hex

ENCODER_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3360
