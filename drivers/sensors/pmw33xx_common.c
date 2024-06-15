// Copyright 2022 Pablo Martinez (@elpekenin)
// Copyright 2022 Daniel Kao (dkao)
// Copyright 2022 Stefan Kerkmann (KarlK90)
// Copyright 2022 Ulrich Spörlein (@uqs)
// Copyright 2021 Alabastard (@Alabastard-64)
// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2019 Sunjun Kim
// Copyright 2020 Ploopy Corporation
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pointing_device_internal.h"
#include "pmw33xx_common.h"
#include "string.h"
#include "wait.h"
#include "spi_master.h"
#include "progmem.h"

#include "debug.h"
#include "timer.h"

extern const uint8_t pmw33xx_firmware_data[PMW33XX_FIRMWARE_LENGTH] PROGMEM;
extern const uint8_t pmw33xx_firmware_signature[3] PROGMEM;

static const pin_t cs_pins_left[]  = PMW33XX_CS_PINS;
static const pin_t cs_pins_right[] = PMW33XX_CS_PINS_RIGHT;
#define NUMBER_OF_SENSORS (sizeof(cs_pins) / sizeof(pin_t))
static bool in_burst_left[ARRAY_SIZE(cs_pins_left)]   = {0};
static bool in_burst_right[ARRAY_SIZE(cs_pins_right)] = {0};

#ifdef CONSOLE_ENABLE
static fast_timer_t init_time;

void print_byte(uint8_t byte) {
    dprintf("%c%c%c%c%c%c%c%c|", (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0'));
}
#endif
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

const size_t pmw33xx_number_of_sensors = ARRAY_SIZE(cs_pins);

bool __attribute__((cold)) pmw33xx_upload_firmware(uint8_t sensor);
bool __attribute__((cold)) pmw33xx_check_signature(uint8_t sensor);

void pmw33xx_set_cpi_all_sensors(uint16_t cpi) {
    for (uint8_t sensor = 0; sensor < pmw33xx_number_of_sensors; sensor++) {
        pmw33xx_set_cpi(sensor, cpi);
    }
}

bool pmw33xx_spi_start(uint8_t sensor) {
    if (!spi_start(cs_pins[sensor], false, 3, PMW33XX_SPI_DIVISOR)) {
        spi_stop();
        return false;
    }
    // tNCS-SCLK, 10ns
    wait_us(1);
    return true;
}

bool pmw33xx_write(uint8_t sensor, uint8_t reg_addr, uint8_t data) {
    if (!pmw33xx_spi_start(sensor)) {
        return false;
    }

    if (reg_addr != REG_Motion_Burst) {
        in_burst[sensor] = false;
    }

    // send address of the register, with MSBit = 1 to indicate it's a write
    uint8_t command[2] = {reg_addr | 0x80, data};
    if (spi_transmit(command, sizeof(command)) != SPI_STATUS_SUCCESS) {
        return false;
    }

    // tSCLK-NCS for write operation is 35us
    wait_us(35);
    spi_stop();

    // tSWW/tSWR (=18us) minus tSCLK-NCS. Could be shortened, but it looks like
    // a safe lower bound
    wait_us(145);
    return true;
}

uint8_t pmw33xx_read(uint8_t sensor, uint8_t reg_addr) {
    if (!pmw33xx_spi_start(sensor)) {
        return 0;
    }

    // send adress of the register, with MSBit = 0 to indicate it's a read
    spi_write(reg_addr & 0x7f);
    // tSRAD (=160us)
    wait_us(160);
    uint8_t data = spi_read();

    // tSCLK-NCS, 120ns
    wait_us(1);
    spi_stop();

    //  tSRW/tSRR (=20us) mins tSCLK-NCS
    wait_us(19);
    return data;
}

bool pmw33xx_check_signature(uint8_t sensor) {
    uint8_t signature_dump[3] = {
        pmw33xx_read(sensor, REG_Product_ID),
        pmw33xx_read(sensor, REG_Inverse_Product_ID),
        pmw33xx_read(sensor, REG_SROM_ID),
    };

    return memcmp(pmw33xx_firmware_signature, signature_dump, sizeof(signature_dump)) == 0;
}

bool pmw33xx_upload_firmware(uint8_t sensor) {
    // Datasheet claims we need to disable REST mode first, but during startup
    // it's already disabled and we're not turning it on ...
    // pmw33xx_write(REG_Config2, 0x00);  // disable REST mode
    if (!pmw33xx_write(sensor, REG_SROM_Enable, 0x1d)) {
        return false;
    }
    wait_ms(10);
    pmw33xx_write(sensor, REG_SROM_Enable, 0x18);

    if (!pmw33xx_spi_start(sensor)) {
        return false;
    }

    spi_write(REG_SROM_Load_Burst | 0x80);
    wait_us(15);

    for (size_t i = 0; i < PMW33XX_FIRMWARE_LENGTH; i++) {
        spi_write(pgm_read_byte(pmw33xx_firmware_data + i));
        wait_us(15);
    }
    wait_us(200);

    pmw33xx_read(sensor, REG_SROM_ID);
    pmw33xx_write(sensor, REG_Config2, 0x00);

    return true;
}

bool pmw33xx_init(uint8_t sensor) {
    if (sensor >= pmw33xx_number_of_sensors) {
        return false;
    }
#ifdef CONSOLE_ENABLE
    init_time = timer_read_fast();
#endif
    spi_init();

    // power up, need to first drive NCS high then low. the datasheet does not
    // say for how long, 40us works well in practice.
    if (!pmw33xx_spi_start(sensor)) {
        return false;
    }
    wait_us(40);
    spi_stop();
    wait_us(40);

    if (!pmw33xx_write(sensor, REG_Power_Up_Reset, 0x5a)) {
        return false;
    }
    wait_ms(50);

    // read registers and discard
    pmw33xx_read(sensor, REG_Motion);
    pmw33xx_read(sensor, REG_Delta_X_L);
    pmw33xx_read(sensor, REG_Delta_X_H);
    pmw33xx_read(sensor, REG_Delta_Y_L);
    pmw33xx_read(sensor, REG_Delta_Y_H);

    if (!pmw33xx_upload_firmware(sensor)) {
        pd_dprintf("PMW33XX (%d): firmware upload failed!\n", sensor);
        return false;
    }

    spi_stop();
    pd_dprintf("PMW33XX (%d): checkpoint!\n", sensor);

    wait_ms(10);
    pmw33xx_set_cpi(sensor, PMW33XX_CPI);

    wait_ms(1);

#ifdef POINTING_DEVICE_ROTATION_pwm33xx
    static const int16_t rotation[] = POINTING_DEVICE_ROTATION_pwm33xx;
    _Static_assert(NUMBER_OF_SENSORS == sizeof(rotation)/sizeof(rotation[0]));
    // write the 90/180/270 degree angle to REG_Control
    int8_t rot = 0;
    switch (rotation[sensor]) {
        case 90:
            rot = 0b11000000;
            break;
        case 180:
            rot = 0b01100000;
            break;
        case 270:
            rot = 0b10100000;
            break;
    }
    pmw33xx_write(sensor, REG_Control, rot);
#endif

    pmw33xx_write(sensor, REG_Config2, 0x00);
    pmw33xx_write(sensor, REG_Angle_Tune, CONSTRAIN(ROTATIONAL_TRANSFORM_ANGLE, -127, 127));
    pmw33xx_write(sensor, REG_Lift_Config, PMW33XX_LIFTOFF_DISTANCE);

    if (!pmw33xx_check_signature(sensor)) {
        pd_dprintf("PMW33XX (%d): firmware signature verification failed!\n", sensor);
        return false;
    }
#ifdef CONSOLE_ENABLE
    init_time = timer_elapsed_fast(init_time);
#endif

    return true;
}

#if 0
static int16_t convertDeltaToInt(uint8_t high, uint8_t low) {
    // join bytes into twos compliment
    uint16_t twos_comp = (high << 8) | low;

    // convert twos comp to int
    if (twos_comp & 0x8000) return -1 * (~twos_comp + 1);

    return twos_comp;
}
#endif

#if 0
static int16_t signed_sat_add16(int16_t a, int16_t b) {
    int16_t res;
    bool overflow = __builtin_add_overflow(a, b, &res);
    if (overflow) {
        // if b was positive, this will return INT16_MAX, if b was negative,
        // it'll slap its negative sign bit onto INT16_MAX, returning
        // INT16_MIN.
        return ((uint16_t)b >> 15) + INT16_MAX;
    }
    return res;
}
#endif

pmw33xx_report_t pmw33xx_read_burst(uint8_t sensor) {
    pmw33xx_report_t report = {0};

    if (sensor >= pmw33xx_number_of_sensors) {
        return report;
    }

#ifdef CONSOLE_ENABLE
    //uint8_t squal = pmw33xx_read(sensor, REG_SQUAL);
    //dprintf("squal for index %d is %d\n", sensor, 8*squal);
#endif

    if (!in_burst[sensor]) {
#ifdef CONSOLE_ENABLE
        //dprintf("burst on for index %d\n", sensor);
        //dprintf("pmw33xx init took: %d\n", init_time);
#endif
        pd_dprintf("PMW33XX (%d): burst\n", sensor);
        if (!pmw33xx_write(sensor, REG_Motion_Burst, 0x00)) {
            return report;
        }
        in_burst[sensor] = true;
    }

    if (!pmw33xx_spi_start(sensor)) {
        return report;
    }

    spi_write(REG_Motion_Burst);
    wait_us(35); // waits for tSRAD_MOTBR

    spi_receive((uint8_t*)&report, sizeof(report));

    // panic recovery, sometimes burst mode works weird.
    if (report.motion.w & 0b111) {
        in_burst[sensor] = false;
    }

    spi_stop();

    pd_dprintf("PMW33XX (%d): motion: 0x%x dx: %i dy: %i\n", sensor, report.motion.w, report.delta_x, report.delta_y);

    int16_t dx, dy;
    // We need to be able to rotate and invert per sensor, so the single global defines won't do.
#ifdef POINTING_DEVICE_ROTATION_pwm33xx_XXX
    // 16bits for 4 potential combinatins, ugh, use an enum or so.
    static const int16_t rotation[] = POINTING_DEVICE_ROTATION_pwm33xx;
    _Static_assert(NUMBER_OF_SENSORS == sizeof(rotation)/sizeof(rotation[0]));
    switch (rotation[sensor]) {
        case 90:
            dx = convertDeltaToInt(delta_y_h, delta_y_l);
            dy = -convertDeltaToInt(delta_x_h, delta_x_l);
            break;
        case 180:
            dx = -convertDeltaToInt(delta_x_h, delta_x_l);
            dy = -convertDeltaToInt(delta_y_h, delta_y_l);
            break;
        case 270:
            dx = -convertDeltaToInt(delta_y_h, delta_y_l);
            dy = convertDeltaToInt(delta_x_h, delta_x_l);
            break;
        default:
            dx = convertDeltaToInt(delta_x_h, delta_x_l);
            dy = convertDeltaToInt(delta_y_h, delta_y_l);
    }
#else
    dx = report.delta_x;
    dy = report.delta_y;
#endif
#ifdef POINTING_DEVICE_INVERT_XY_pwm33xx
    static const bool invert[][2] = POINTING_DEVICE_INVERT_XY_pwm33xx;
    _Static_assert(NUMBER_OF_SENSORS == sizeof(invert)/sizeof(invert[0]));
    if (invert[sensor][0])
        dx = -dx;
    if (invert[sensor][1])
        dy = -dy;
#endif

    // XXX need to not read directly into report for that one
    // report.dx = signed_sat_add16(report.dx, dx);
    // report.dy = signed_sat_add16(report.dy, dy);
    report.delta_x = dx;
    report.delta_y = dy;

    report.delta_x *= -1;
    report.delta_y *= -1;

    return report;
}
