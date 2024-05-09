/** \file
 * Combine multiple demos for the Two Sigma / Blinklabs EightByEight badge.
 *
 * The button is used to cycle between the different demos.
 * 
 * The pixels one is not really a separate demo; we switch to it
 * automatically if it tells us that it has received a packet.
 * If we go more than ten seconds without a packet, the current demo
 * is restarted.
 * 
 * If there are three button clicks that are 500ms apart from each other, 
 * we will toggle whether the "Pixels" demo runs (which connects to the
 * wifi network and shows the frame requested).
 */
#include "Badge.h"
#include "Bubble.h"
#include "Pov.h"
#include "TextScroll.h"
#include "TSLogo.h"

Badge badge;

TextScroll textscroll;
Bubble bubble;
Pov pov;

Demo * demos[] = {
	// &textscroll,
	&bubble,
};

const unsigned num_demos = sizeof(demos) / sizeof(*demos);
static unsigned demo_num = 0;
static Demo * demo;
static unsigned clicks = 0;
static char mac_buf[6*3+1];
static uint32_t last_draw_millis;
const unsigned brightnessDivisor = 4;
static unsigned brightness = 128 * brightnessDivisor - 1;

void nudgeBrightness() {
  //slowly growing brightness with some catch at the apex to easily stop
  unsigned x =  ((brightness++ / brightnessDivisor) % 288);
  badge.matrix.setBrightness(_min(255, x)); // for some reason min is undefined. Seems to be A Thing (https://github.com/esp8266/Arduino/issues/263)
}

void setup()
{
	badge.begin();
	badge.matrix.clear();
	badge.matrix.show();
	// nudgeBrightness();
	badge.matrix.setBrightness(255);

	pov.begin();


	// Initialize all of the demos and start at 0
	for(int i = 0 ; i < num_demos ; i++)
		demos[i]->begin();

	demo_num = 0;
	demo = demos[demo_num];
}


void loop()
{
	if (badge.poll())
	{
		// they have tapped fairly hard, should send this to demo
#if 0
		pov.enabled = true;
		badge.matrix.clear();
		badge.matrix.show();
#endif
	}

	if (pov.enabled)
	{
		if (badge.button())
		{
			pov.enabled = false;
			pov.begin();
			return;
		}

		// pov is the highest priority; we don't do anything
		// else until it disables itself
		pov.step(badge.ax, badge.ay, badge.az);
		pov.draw(badge.matrix);
		return;
	}

	const uint32_t now = millis();

	if (badge.button_edge())
	{
		// should cycle to the next demo
		demo_num = (demo_num + 1) % num_demos;
		demo = demos[demo_num];
	}

	// if (badge.button())
	// {
	// 	Serial.print(badge.nx); Serial.print(' ');
	// 	Serial.print(badge.ny); Serial.print(' ');
	// 	Serial.print(badge.nz); Serial.print(' ');
	// 	Serial.println(badge.g);
	// 	Serial.print("  demo_num="); Serial.print(demo_num);
	// 	Serial.println();
	// }

	// if (badge.button())
	// {
	// 	nudgeBrightness();
	// }

	bool do_draw = demo->step(badge.ax, badge.ay, badge.az);

	// only draw the LEDs at 30Hz
	if (!do_draw && now - last_draw_millis < 30)
		return;
	last_draw_millis = now;

	demo->draw(badge.matrix);

	badge.matrix.show();
	delay(2);
}
