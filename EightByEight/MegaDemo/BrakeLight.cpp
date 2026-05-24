/** \file
 * Brake light demo implementation.
 */

#include "BrakeLight.h"
#include <Arduino.h>
#include <math.h>

static const float BREATH_MIN           = 20.0f / 255.0f;
static const float BREATH_MAX           = 1.0f;
static const float BREATH_SPEED         = 0.021f;   // ~10-second cycle at 30 fps
static const float FADE_IN_SPEED        = 0.25f;    // fast: ~4 frames 0→1
static const float FADE_OUT_SPEED       = 0.015f;   // slow: ~2 s from 1→0

BrakeLight::BrakeLight() :
    brightness(0.0f),
    smoothed_az(0.0f),
    breath_phase(0.0f)
{
}

void BrakeLight::begin()
{
    brightness    = 0.0f;
    smoothed_az   = 0.0f;
    breath_phase  = 0.0f;
}

bool BrakeLight::step(float /*ax*/, float /*ay*/, float az)
{
    breath_phase += BREATH_SPEED;
    if (breath_phase > 2.0f * M_PI)
        breath_phase -= 2.0f * M_PI;

    float target, speed;

    const float s = (sinf(breath_phase) + 1.0f) / 2.0f;
    target = BREATH_MIN + (BREATH_MAX - BREATH_MIN) * s;
    speed  = FADE_OUT_SPEED * 3.0f;

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
