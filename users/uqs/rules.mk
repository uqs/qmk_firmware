# don't include for keyboards/ploopyco/mouse/keymaps/uqs
ifneq ($(strip $(POINTING_DEVICE_ENABLE)), yes)
SRC += uqs.c

RGBLIGHT_ENABLE ?= yes      # Enable keyboard RGB underglow
LTO_ENABLE ?= yes  # disables the legacy TMK Macros and Functions features

UCIS_ENABLE = yes
LEADER_ENABLE = yes
COMBO_ENABLE = yes
MOUSEKEY_ENABLE = yes

# From https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/
# adds about 900 bytes!
DEBOUNCE_TYPE = asym_eager_defer_pk
endif
