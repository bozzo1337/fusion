/*
	Using the Panasonic Grid-EYE Sensor
	By: Nick Poole
	SparkFun Electronics
	Date: January 12th, 2018

	MIT License: Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the Software), to deal in the Software without
	restriction, including without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or
	substantial portions of the Software.

	THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
	BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	Feel like supporting our work? Buy a board from SparkFun!
	https://www.sparkfun.com/products/14568

	This example implements a rudimentary heat camera in the serial terminal, using a matrix of ascii
	characters to represent the temperature of each pixel in the device. Start your terminal at 115200
	and make the window as small as possible for best effect.

	Hardware Connections:
	Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
	Plug the sensor onto the shield
*/

#include "SparkFun_GridEYE_Arduino_Library.h"
//#include <Wire.h>
#include <stdio.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Use these values (in degrees C) to adjust the contrast
#define HOT 40
#define COLD 20

// This table can be of type int because we map the pixel
// temperature to 0-3. Temperatures are reported by the
// library as floats
float pixelTable[64];
int inc = 0;

GridEYE grideye;

void setup() {

	// Start your preferred I2C object
	//Wire.begin();
	// Library assumes Wire for I2C but you can pass something else with begin() if you like
	grideye.begin();
	//system("raspistill -o init -v");

}

void loop() {

	system("raspistill -w 256 -h 256 -e bmp -o test.bmp -t 1 --colfx 128:128 -br 55");

	// loop through all 64 pixels on the device and map each float value to a number
	// between 0 and 3 using the HOT and COLD values we set at the top of the sketch
	for(unsigned char i = 0; i < 64; i++){
		pixelTable[i] = grideye.getPixelTemperature(i);
	}


	int x, y, n;
	n = 3;
	x = y= 256;
	unsigned char *data = stbi_load("test.bmp", &x, &y, &n, 0);

	if (data == NULL) printf("Loading failed\n");

	// loop through the table of mapped values and print a character corresponding to each
	// pixel's temperature. Add a space between each. Start a new line every 8 in order to
	// create an 8x8 grid
	for(unsigned char i = 0; i < 64; i++) {
		printf("%f ",pixelTable[i]);

		//int _x = 224 - ((i % 8) * 32);
		int _x = 224 - ((i/8) * 32);
		int maxX = _x + 32;
		int _y = (i % 8) * 32;
		//int _y = 224 - ((i % 8) * 32);
		int maxY = _y + 32;
		int value = (int) (17 * (pixelTable[i] - 15));
		//printf("%i ", value);
		if ((i + 1) % 8 == 0) printf("\n");


		for (; _x < maxX; _x++) {
			for (; _y < maxY; _y++) {
				if (pixelTable[i] > 15 &&  pixelTable[i] < 30 ) {
					data[((_y*256)+_x)*3] = value;
				}
				//if (pixelTable[i] >= 20 &&  pixelTable[i] < 40 )
				//	data[((_x*256)+_y)*3] = 30 + (int) (11 * (pixelTable[i]) - 20);
			}
			_y = (i % 8) * 32;
			//_y = 224 - ((i % 8) * 32);
		}
	}
	stbi_write_bmp("IR.bmp", 8, 8, 1, pixelTable);
	stbi_write_bmp("testIR.bmp", x, y, n, data);
	//stbi_write_bmp("output/"+inc+".bmp", x, y, n, data);

	// in between updates, throw a few linefeeds to visually separate the grids. If you're using
	// a serial terminal outside the Arduino IDE, you can replace these linefeeds with a clearscreen
	// command
	printf("\n");
	printf("\n");


	// toss in a delay because we don't need to run all out
	sleep(1);
	inc++;

}

int main() {
	setup();
	/*while(1){
		loop();
	}*/
	loop();
	return 0;
}
