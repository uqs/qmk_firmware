/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 * Copyright 2022 Ulrich Spörlein
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spi_master.h"
#include "pmw3360.h"
#include "wait.h"
#include "debug.h"
#include "timer.h"
#include "print.h"
#include PMW3360_FIRMWARE_H

// Registers
// clang-format off
#define REG_Product_ID                   0x00
#define REG_Revision_ID                  0x01
#define REG_Motion                       0x02
#define REG_Delta_X_L                    0x03
#define REG_Delta_X_H                    0x04
#define REG_Delta_Y_L                    0x05
#define REG_Delta_Y_H                    0x06
#define REG_SQUAL                        0x07
#define REG_Raw_Data_Sum                 0x08
#define REG_Maximum_Raw_data             0x09
#define REG_Minimum_Raw_data             0x0a
#define REG_Shutter_Lower                0x0b
#define REG_Shutter_Upper                0x0c
#define REG_Control                      0x0d
#define REG_Config1                      0x0f
#define REG_Config2                      0x10
#define REG_Angle_Tune                   0x11
#define REG_Frame_Capture                0x12
#define REG_SROM_Enable                  0x13
#define REG_Run_Downshift                0x14
#define REG_Rest1_Rate_Lower             0x15
#define REG_Rest1_Rate_Upper             0x16
#define REG_Rest1_Downshift              0x17
#define REG_Rest2_Rate_Lower             0x18
#define REG_Rest2_Rate_Upper             0x19
#define REG_Rest2_Downshift              0x1a
#define REG_Rest3_Rate_Lower             0x1b
#define REG_Rest3_Rate_Upper             0x1c
#define REG_Observation                  0x24
#define REG_Data_Out_Lower               0x25
#define REG_Data_Out_Upper               0x26
#define REG_Raw_Data_Dump                0x29
#define REG_SROM_ID                      0x2a
#define REG_Min_SQ_Run                   0x2b
#define REG_Raw_Data_Threshold           0x2c
#define REG_Config5                      0x2f
#define REG_Power_Up_Reset               0x3a
#define REG_Shutdown                     0x3b
#define REG_Inverse_Product_ID           0x3f
#define REG_LiftCutoff_Tune3             0x41
#define REG_Angle_Snap                   0x42
#define REG_LiftCutoff_Tune1             0x4a
#define REG_Motion_Burst                 0x50
#define REG_LiftCutoff_Tune_Timeout      0x58
#define REG_LiftCutoff_Tune_Min_Length   0x5a
#define REG_SROM_Load_Burst              0x62
#define REG_Lift_Config                  0x63
#define REG_Raw_Data_Burst               0x64
#define REG_LiftCutoff_Tune2             0x65

#define CPI_STEP          100
// clang-format on

// limits to 0--119, resulting in a CPI range of 100 -- 12000 (as only steps of 100 are possible).
// Note that for the PMW3389DM chip, the step size is 50 and supported range is
// up to 16000. The datasheet does not indicate the minimum CPI though, neither
// whether this uses 2 bytes (as 16000/50 == 320)
#ifndef MAX_CPI
#    define MAX_CPI 0x77
#endif

static const pin_t pins[] = PMW3360_CS_PINS;
#define NUMBER_OF_SENSORS (sizeof(pins) / sizeof(pin_t))

bool _inBurst[NUMBER_OF_SENSORS] = {0};

#ifdef CONSOLE_ENABLE
static fast_timer_t init_time;

void print_byte(uint8_t byte) { dprintf("%c%c%c%c%c%c%c%c|", (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')); }
#endif
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

bool pmw3360_spi_start(int8_t index) {
    bool status = spi_start(pins[index], PMW3360_SPI_LSBFIRST, PMW3360_SPI_MODE, PMW3360_SPI_DIVISOR);
    // tNCS-SCLK, 120ns
    wait_us(1);
    return status;
}

spi_status_t pmw3360_write(int8_t index, uint8_t reg_addr, uint8_t data) {
    pmw3360_spi_start(index);

    if (reg_addr != REG_Motion_Burst) {
        _inBurst[index] = false;
    }

    // send address of the register, with MSBit = 1 to indicate it's a write
    spi_status_t status = spi_write(reg_addr | 0x80);
    status              = spi_write(data);

    // tSCLK-NCS for write operation is 35us
    wait_us(35);
    spi_stop();

    // tSWW/tSWR (=180us) minus tSCLK-NCS. Could be shortened, but it looks like a safe lower bound
    wait_us(145);
    return status;
}

uint8_t pmw3360_read(int8_t index, uint8_t reg_addr) {
    pmw3360_spi_start(index);
    // send adress of the register, with MSBit = 0 to indicate it's a read
    spi_write(reg_addr & 0x7f);
    // tSRAD (=160us)
    wait_us(160);
    uint8_t data = spi_read();

    // tSCLK-NCS for read operation is 120ns
    wait_us(1);
    spi_stop();

    //  tSRW/tSRR (=20us) minus tSCLK-NCS
    wait_us(19);
    return data;
}

bool pmw3360_init(int8_t index) {
#ifdef CONSOLE_ENABLE
    init_time = timer_read_fast();
#endif
    spi_init();

#if 0
    // with this on, init() takes 469ms, without we're down to 164ms
    setPinOutput(pins[index]);

    spi_stop();
    pmw3360_spi_start(index);
    spi_stop();

    pmw3360_write(index, REG_Shutdown, 0xb6);  // Shutdown first
    wait_ms(300);
#endif

    // power up, need to first drive NCS high then low.
    // the datasheet does not say for how long, 40us works well in practice.
    pmw3360_spi_start(index);
    wait_us(40);
    spi_stop();
    wait_us(40);
    pmw3360_write(index, REG_Power_Up_Reset, 0x5a);
    wait_ms(50);

    // read registers and discard
    pmw3360_read(index, REG_Motion);
    pmw3360_read(index, REG_Delta_X_L);
    pmw3360_read(index, REG_Delta_X_H);
    pmw3360_read(index, REG_Delta_Y_L);
    pmw3360_read(index, REG_Delta_Y_H);

    pmw3360_upload_firmware(index);

    spi_stop();

    wait_ms(10);
    pmw3360_set_cpi(PMW3360_CPI);

    wait_ms(1);

#ifdef POINTING_DEVICE_ROTATION_pwm3360
    static const int16_t rotation[] = POINTING_DEVICE_ROTATION_pwm3360;
    _Static_assert(NUMBER_OF_SENSORS == sizeof(rotation)/sizeof(rotation[0]));
    // write the 90/180/270 degree angle to REG_Control
    int8_t rot = 0;
    switch (rotation[index]) {
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
    pmw3360_write(index, REG_Control, rot);
#endif

    // XXX data sheet only shows from -30 deg to 30deg, using values 0xe2 for -30, 0x00 for 0, 0x1e for +30
    pmw3360_write(index, REG_Angle_Tune, constrain(ROTATIONAL_TRANSFORM_ANGLE, -127, 127));

    pmw3360_write(index, REG_Lift_Config, PMW3360_LIFTOFF_DISTANCE);

    bool init_success = pmw3360_check_signature(index);
    //writePinLow(pins[index]);

#ifdef CONSOLE_ENABLE
    if (init_success) {
        dprintf("pmw3360 signature(s) verified");
    } else {
        dprintf("pmw3360 signature(s) verification failed!");
    }
    init_time = timer_elapsed_fast(init_time);
#endif

    return init_success;
}

void pmw3360_upload_firmware(int8_t index) {
    // Datasheet claims we need to disable REST mode first, but during startup
    // it's already disabled and we're not turning it on ...
    //pmw3360_write(index, REG_Config2, 0x00);  // disable REST mode
    pmw3360_write(index, REG_SROM_Enable, 0x1d);

    wait_ms(10);

    pmw3360_write(index, REG_SROM_Enable, 0x18);

    pmw3360_spi_start(index);
    spi_write(REG_SROM_Load_Burst | 0x80);
    wait_us(15);

    unsigned char c;
    for (int i = 0; i < FIRMWARE_LENGTH; i++) {
        c = (unsigned char)pgm_read_byte(firmware_data + i);
        spi_write(c);
        wait_us(15);
    }
    wait_us(200);

    pmw3360_read(index, REG_SROM_ID);
    pmw3360_write(index, REG_Config2, 0x00);
}

bool pmw3360_check_signature(int8_t index) {
    uint8_t pid      = pmw3360_read(index, REG_Product_ID);
    uint8_t iv_pid   = pmw3360_read(index, REG_Inverse_Product_ID);
    uint8_t SROM_ver = pmw3360_read(index, REG_SROM_ID);
    return (pid == firmware_signature[0] && iv_pid == firmware_signature[1] && SROM_ver == firmware_signature[2]);  // signature for SROM 0x04
}

uint16_t pmw3360_get_cpi(void) {
    uint8_t cpival = pmw3360_read(0, REG_Config1);
#ifdef CONSOLE_ENABLE
    for (size_t i=1; i<NUMBER_OF_SENSORS; i++) {
        uint8_t othercpival = pmw3360_read(i, REG_Config1);
        if (cpival != othercpival) {
            dprintf("pmw3360 cpivals differ: %d vs %d", cpival, othercpival);
        }
    }
#endif
    return (uint16_t)((cpival + 1) & 0xFF) * CPI_STEP;
}

void pmw3360_set_cpi(uint16_t cpi) {
    uint8_t cpival = constrain((cpi / CPI_STEP) - 1, 0, MAX_CPI);
    for (size_t i=0; i<NUMBER_OF_SENSORS; i++) {
        pmw3360_write(i, REG_Config1, cpival);
    }
}

static int16_t convertDeltaToInt(uint8_t high, uint8_t low) {
    // join bytes into twos compliment
    uint16_t twos_comp = (high << 8) | low;

    // convert twos comp to int
    if (twos_comp & 0x8000) return -1 * (~twos_comp + 1);

    return twos_comp;
}

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

report_pmw3360_t pmw3360_read_burst(int8_t index) {
    report_pmw3360_t report = {0};

    if (!_inBurst[index]) {
#ifdef CONSOLE_ENABLE
        //dprintf("burst on\n");
        dprintf("pmw3360 init took: %d\n", init_time);
#endif
        pmw3360_write(index, REG_Motion_Burst, 0x00);
        _inBurst[index] = true;
    }

    pmw3360_spi_start(index);
    spi_write(REG_Motion_Burst);
    wait_us(35);  // waits for tSRAD_MOTBR

    uint8_t motion = spi_read();
    spi_read();  // skip Observation
    // delta registers
    uint8_t delta_x_l = spi_read();
    uint8_t delta_x_h = spi_read();
    uint8_t delta_y_l = spi_read();
    uint8_t delta_y_h = spi_read();

    spi_stop();

#ifdef CONSOLE_ENABLE
    if (debug_mouse) {
        /*
        dprintf("sensor %d: ", index);
        print_byte(motion);
        print_byte(delta_x_l);
        print_byte(delta_x_h);
        print_byte(delta_y_l);
        print_byte(delta_y_h);
        dprintf("\n");
        */
    }
#endif

    if (motion & 0b111) {  // panic recovery, sometimes burst mode works weird.
        _inBurst[index] = false;
    }

    const bool isMotion    = (motion & 0x80) != 0;
    const bool isOnSurface = (motion & 0x08) == 0;

    if (!isMotion) {
        return report;
    }

    int16_t dx, dy;
    // We need to be able to rotate and invert per sensor, so the single global defines won't do.
#ifdef POINTING_DEVICE_ROTATION_pwm3360_XXX
    // 16bits for 4 potential combinatins, ugh, use an enum or so.
    static const int16_t rotation[] = POINTING_DEVICE_ROTATION_pwm3360;
    _Static_assert(NUMBER_OF_SENSORS == sizeof(rotation)/sizeof(rotation[0]));
    switch (rotation[index]) {
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
    dx = convertDeltaToInt(delta_x_h, delta_x_l);
    dy = convertDeltaToInt(delta_y_h, delta_y_l);
#endif
#ifdef POINTING_DEVICE_INVERT_XY_pwm3360
    static const bool invert[][2] = POINTING_DEVICE_INVERT_XY_pwm3360;
    _Static_assert(NUMBER_OF_SENSORS == sizeof(invert)/sizeof(invert[0]));
    if (invert[index][0])
        dx = -dx;
    if (invert[index][1])
        dy = -dy;
#endif

    report.isMotion |= isMotion;
    report.isOnSurface |= isOnSurface;
    report.dx = signed_sat_add16(report.dx, dx);
    report.dy = signed_sat_add16(report.dy, dy);

    return report;
}
