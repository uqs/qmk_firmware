RGBLIGHT_ENABLE = yes
#VIA_ENABLE = yes  # doesn't fit with 7 layers :/
#BOOTMAGIC_ENABLE = lite
LTO_ENABLE = yes  # disables the legacy TMK Macros and Functions features

UCIS_ENABLE = yes
LEADER_ENABLE = yes

# From https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/
# adds about 900 bytes!
DEBOUNCE_TYPE = sym_eager_pk
