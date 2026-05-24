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
    explicit BrakeLight();
    virtual void begin();
    virtual bool step(float ax, float ay, float az);
    virtual void draw(RGBMatrix &matrix);

private:
    float brightness;
    float smoothed_az;
    float breath_phase;
};

#endif
