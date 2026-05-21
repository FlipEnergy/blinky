/** \file
 * Brake light demo implementation.
 */

#include "BrakeLight.h"
#include <Arduino.h>
#include <math.h>

static const float DECEL_THRESHOLD      = 0.5f;    // ~0.25 g
static const unsigned long BRAKE_HOLD_MS = 5000;   // hold brake light for 5 seconds
static const float BREATH_MIN           = 20.0f / 255.0f;
static const float BREATH_MAX           = 60.0f / 255.0f;
static const float BREATH_SPEED         = 0.021f;   // ~10-second cycle at 30 fps
static const float FADE_IN_SPEED        = 0.25f;    // fast: ~4 frames 0→1
static const float FADE_OUT_SPEED       = 0.015f;   // slow: ~2 s from 1→0

BrakeLight::BrakeLight(int mode) :
    mode(mode),
    brightness(0.0f),
    braking(false),
    smoothed_az(0.0f),
    breath_phase(0.0f),
    last_brake_ms(0)
{
}

void BrakeLight::begin()
{
    brightness    = 0.0f;
    braking       = false;
    smoothed_az   = 0.0f;
    breath_phase  = 0.0f;
    last_brake_ms = 0;
}

bool BrakeLight::step(float /*ax*/, float /*ay*/, float az)
{
    const unsigned long now = millis();

    if (!braking)
        smoothed_az = (smoothed_az * 31.0f + az) / 32.0f;

    const float z_delta = az - smoothed_az;
    if (fabs(z_delta) > DECEL_THRESHOLD)
    {
        braking = true;
        last_brake_ms = now;
    }
    else if (now - last_brake_ms > BRAKE_HOLD_MS)
    {
        braking = false;
    }

    breath_phase += BREATH_SPEED;
    if (breath_phase > 2.0f * M_PI)
        breath_phase -= 2.0f * M_PI;

    float target, speed;
    if (braking)
    {
        target = 1.0f;
        speed  = FADE_IN_SPEED;
    }
    else if (mode == 0)
    {
        target = 0.0f;
        speed  = FADE_OUT_SPEED;
    }
    else  // mode 1: breathing
    {
        const float s = (sinf(breath_phase) + 1.0f) / 2.0f;
        target = BREATH_MIN + (BREATH_MAX - BREATH_MIN) * s;
        speed  = FADE_OUT_SPEED * 3.0f;
    }

    brightness += (target - brightness) * speed;
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;

    return true;
}

void BrakeLight::draw(RGBMatrix &matrix)
{
    const int r = (int)(brightness * 255.0f);
    for (int row = 0; row < LED_ROWS; row++)
        for (int col = 0; col < LED_COLS; col++)
            matrix.set(row, col, r, 0, 0);
}
