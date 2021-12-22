# MCU name
MCU = atmega32u4

BOOTLOADER = atmel-dfu

## Build Options
##   change yes to no to disable
##
#BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
#EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
#COMMAND_ENABLE = no         # Commands for debug and configuration
## Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
#SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
## if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
#NKRO_ENABLE = no            # USB Nkey Rollover
#BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
#RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
#AUDIO_ENABLE = no           # Audio output

# Debounce eagerly (report change immediately), keep per-key timers. We can use
# this because the Cherry MX keyswitches on the Kinesis only produce noise while
# pressed.
DEBOUNCE_TYPE = asym_eager_defer_pk

# Specify firmware format that is copied to root qmk_firmware folder
FIRMWARE_FORMAT = hex

ENCODER_ENABLE = yes

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3360
