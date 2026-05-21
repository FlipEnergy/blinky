/** \file
 * Brake light demo for the EightByEight badge.
 *
 * Instantiate with mode 0 (off when idle) or mode 1 (breathing when idle).
 * In either case, Z-axis deceleration fades the full 8x8 grid to solid red.
 */
#ifndef _BrakeLight_h_
#define _BrakeLight_h_

#include "Demo.h"
#include <stdint.h>

class BrakeLight : public Demo
{
public:
    explicit BrakeLight(int mode = 0);   // 0 = off when idle, 1 = breathing when idle
    virtual void begin();
    virtual bool step(float ax, float ay, float az);
    virtual void draw(RGBMatrix &matrix);

private:
    const int mode;
    float brightness;
    bool braking;
    float smoothed_az;
    float breath_phase;
    unsigned long last_brake_ms;
};

#endif
