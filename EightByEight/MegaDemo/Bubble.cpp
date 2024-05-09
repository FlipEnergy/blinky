/** \file
 * Bubble level demo
 */

#include "Bubble.h"
#include <math.h>

// CUSTOMIZE: Set the bubble's color here, in hex format (similar to coloring HTML.)
static const uint32_t bubble_color = 0x00ffff;
static const uint32_t negative_color = 0xff00ff;

void Bubble::begin()
{
}

bool Bubble::step(float nx, float ny, float nz)
{
	// smooth the values
	const float smooth = 1;
	ax = (ax * smooth + nx) / (smooth + 1);
	ay = (ay * smooth + ny) / (smooth + 1);
	az = (az * smooth + nz) / (smooth + 1);

	// never force redraw
	return false;
}

void Bubble::draw(RGBMatrix &matrix)
{
	// rotate the x y by 45 degrees
	const float rx = ax * -0.851 - ay * -0.525;
	const float ry = ay * -0.851 + ax * -0.525;

	// decay everything to negative color
	for(int x = 0 ; x < LED_ROWS ; x++)
		for(int y = 0 ; y < LED_COLS ; y++)
			matrix.blend(x, y, 50, negative_color);

	for(int x = 0 ; x < LED_ROWS ; x++)
	{
		for(int y = 0 ; y < LED_COLS ; y++)
		{
			float dx = rx - (x - LED_ROWS/2);
			float dy = ry - (y - LED_COLS/2);
			int dist = 255 - sqrt(dx*dx + dy*dy) * 42;
			if (dist < 0)
				continue;

			// bubble color
			matrix.blend(x, y, dist, bubble_color);
		}
	}
}
