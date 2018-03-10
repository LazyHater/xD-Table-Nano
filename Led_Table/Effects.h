#ifndef EFFECTS_H
#define EFFECTS_H

#include "Arduino.h"
#include "Graphics.h"
#include "IRPanel.h"

extern byte threshold;

class Effect {
protected:
	Led_Table_Pixels* strip;
	IRPanel* irPanel;
public:
	Effect(Led_Table_Pixels* _strip, IRPanel* _irPanel) {
		strip = _strip;
		irPanel = _irPanel;
	}

	void show() {
		strip->show();
	}

	virtual void nextFrame() = 0;
};

class Rainbow: public Effect {
private:
	int rainbow_counter = 0;
public:
	Rainbow(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
	}

	void nextFrame() {
		for (byte i = 0; i < strip->numPixels(); i++) {
			if (irPanel->getValueAt(i))
				strip->setPixelColor(i, Wheel((i + rainbow_counter + 128)));
			else
				strip->setPixelColor(i, Wheel((i + rainbow_counter)));
		}
		rainbow_counter++;
		if (rainbow_counter > 255)
			rainbow_counter = 0;
		//delay(20);
	}
};

class RainbowRows: public Effect {
private:
	int rainbow_counter = 0;
public:
	RainbowRows(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
	}

	void nextFrame() {
		for (int i = 0; i < 10; i++) {
			if (irPanel->getValueAt(99 - i))
				setRow(strip, i, Wheel((i * 10 + rainbow_counter)));
			else
				setRow(strip, i, Wheel((i * 10 + rainbow_counter + 128)));
		}
		rainbow_counter++;
		if (rainbow_counter > 255)
			rainbow_counter = 0;
		delay(5);
	}
};

class RainbowSoundBars: public Effect {

private:
	int rainbow_counter = 0;
	int highestValues[10] = { 0 };
public:
	RainbowSoundBars(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
		for (byte x = 0; x < 10; x++) {
			highestValues[x] = 10;
		}
	}

	void nextFrame() {
		updateHighestValues();
		for (byte x = 0; x < 10; x++) {
			for (byte y = 0; y < 10; y++) {
				if (highestValues[x] <= y)
					strip->setPixelColor(x, y,
							Wheel((IDX(x, y) + rainbow_counter + 128) & 255));
				else
					strip->setPixelColor(x, y,
							Wheel((x + y * 10 + rainbow_counter)));
			}
		}
		rainbow_counter++;
		if (rainbow_counter > 255)
			rainbow_counter = 0;
		// delay(5);
	}

	void updateHighestValues() {
		for (byte x = 0; x < 10; x++) {
			for (byte y = 0; y < 10; y++) {
				if (irPanel->getValueAt(x, y)) {
					highestValues[x] = y;
				}
			}
		}
	}
};

class RainbowLine: public Effect {
public:
	class Line {
		//uint32_t color = 0x00ffffff;
		uint32_t color;
		float dx0, dy0, dx1, dy1;
		float x0, x1, y0, y1;
	public:

		Line(uint32_t color) {
			this->color = color;
			randomSeed(analogRead(A3));
			dx0 = random(1, 10) / 30.0;
			dy0 = random(1, 10) / 30.0;
			dx1 = random(1, 10) / 30.0;
			dy1 = random(1, 10) / 30.0;
			x0 = random(10);
			x1 = random(10);
			y0 = random(10);
			y1 = random(10);
		}

		void show(Led_Table_Pixels *strip) {
			drawLine(strip, x0, y0, x1, y1, color);
		}
		void update() {
			x0 += dx0;
			x1 += dx1;
			y0 += dy0;
			y1 += dy1;
			checkBounds();
			delay(5);
		}
		void checkBounds() {
			if (x0 > 9) {
				x0 = 9;
				dx0 = -dx0;
			}
			if (x1 > 9) {
				x1 = 9;
				dx1 = -dx1;
			}
			if (y0 > 9) {
				y0 = 9;
				dy0 = -dy0;
			}
			if (y1 > 9) {
				y1 = 9;
				dy1 = -dy1;
			}
			if (x0 < 0) {
				x0 = 0;
				dx0 = -dx0;
			}
			if (x1 < 0) {
				x1 = 0;
				dx1 = -dx1;
			}
			if (y0 < 0) {
				y0 = 0;
				dy0 = -dy0;
			}
			if (y1 < 0) {
				y1 = 0;
				dy1 = -dy1;
			}
		}
	};
	Line lineR;
	Line lineG;
	Line lineB;
private:
	int rainbow_counter = 0;

public:
	RainbowLine(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel), lineG(0x0000ff00), lineB(0x000000ff), lineR(
					0x00ff0000) {
	}

	void nextFrame() {
		for (byte i = 0; i < strip->numPixels(); i++) {
			if (irPanel->getValueAt(i))
				strip->setPixelColor(i, 0x00999999);
			else
				strip->setPixelColor(i, 0);
		}

		//     if (irPanel->getValueAt(i))
		//      strip->setPixelColor(i, Wheel((i + rainbow_counter + 128) ));
		//     else
		//       strip->setPixelColor(i, Wheel((i + rainbow_counter)));
		//    }
		//   rainbow_counter++;
		//    if (rainbow_counter > 255) rainbow_counter = 0;
		lineR.update();
		lineG.update();
		lineB.update();
		lineR.show(strip);
		lineG.show(strip);
		lineB.show(strip);
	}
};

class RainbowFade: public Effect {
private:
	int rainbow_counter = 0;
	float fade_strength = 0.992;
	byte minimal_brightness = 10;
public:
	RainbowFade(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
	}

	void nextFrame() {
		for (byte i = 0; i < 100; i++) {
			if (irPanel->getValueAt(i) == 1) {
				strip->setPixelColor(i, Wheel((i + rainbow_counter) & 0xff));

				//strip->setPixelColor(0,255,255,255);
			} else {
				byte pix_bright = strip->getPixelBrightness(i);
				if (pix_bright < minimal_brightness)
					pix_bright = minimal_brightness;
				float debuff = (pix_bright / (255.0 / 3.0)) * fade_strength;
				uint32_t c = Wheel((i + rainbow_counter) & 0xff);
				byte r = (byte) (((c & 0x00ff0000) >> 16) * debuff);
				byte g = (byte) (((c & 0x0000ff00) >> 8) * debuff);
				byte b = (byte) ((c & 0x000000ff) * debuff);
				strip->setPixelColor(i, r, g, b);
			}
		}
		rainbow_counter++;
		if (rainbow_counter > 255)
			rainbow_counter = 0;
	}
};

#endif

class RandomPixels: public Effect {
private:
	float chance_for_spawn_pixel = 0.003;
public:
	RandomPixels(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
		for (byte i = 0; i < strip->numPixels(); i++)
			strip->setPixelColor(i, 0);
	}

	void nextFrame() {

		for (byte i = 0; i < strip->numPixels(); i++) {

			if (irPanel->getValueAt(i)) {
				strip->setPixelColor(i, Wheel(random(256)));
				continue;
			}

			if ((random(1000) / 1000.0) < chance_for_spawn_pixel)
				//   if (irPanel->getValueAt(i));
				strip->setPixelColor(i, Wheel(random(256)));
			//else
			//strip->setPixelColor(i, 255);

		}
		strip->fade(0.95);
	}
};

class Rain: public Effect {
private:
	class Drop {
	public:
		int x;
		float y;
		float yspeed;
		Drop() {
			reset();
		}

		void update(IRPanel* irPanel) {
			y += yspeed;
			if (irPanel->getValueAt(x, y) != 0)
				reset();

			if ((byte) y > 9)
				reset();
		}

		void reset() {
			x = random(10);
			y = 0;
			yspeed = random(5) / 10.0 + 0.2;
		}

		void show(Led_Table_Pixels* strip) {
			if ((byte) y >= 0 && (byte) y <= 9)
				strip->setPixelColor(x, y, 0x000000ff);
		}
	};
#define NUMBER_OF_DROPS 10
	Drop drops[NUMBER_OF_DROPS];

public:
	Rain(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
	}

	void nextFrame() {
		for (int i = 0; i < NUMBER_OF_DROPS; i++) {
			drops[i].update(irPanel);
			drops[i].show(strip);
		}
		for (byte i = 0; i < strip->numPixels(); i++)
			if (irPanel->getValueAt(i))
				strip->setPixelColor(i, 0x00ffffff);

		strip->fade(0.82);
	}
};

class MetaBalls: public Effect {
private:
	class Ball {
	public:
		float x;
		float y;
		float yspeed;
		float xspeed;
		byte r = 255;
		byte g = 255;
		byte b = 255;

		Ball() {
			x = random(10);
			y = random(10);

			yspeed = random(8) / 10.0 + 0.2;
			xspeed = random(8) / 10.0 + 0.2;
		}

		void update(IRPanel* irPanel) {
			y += yspeed;
			x += xspeed;
			checkEdges();
			checkIRCollision(irPanel);
		}

		void checkEdges() {
			if (y > 9) {
				yspeed = -yspeed;
				y = 9;
			}
			if (y < 0) {
				yspeed = -yspeed;
				y = 0;
			}
			if (x > 9) {
				xspeed = -xspeed;
				x = 9;
			}
			if (x < 0) {
				xspeed = -xspeed;
				x = 0;
			}
		}

		void checkIRCollision(IRPanel* irPanel) {
			if (irPanel->getValueAt((byte) (x + xspeed), y) != 0) {
				xspeed = -xspeed;
			}

			if (irPanel->getValueAt(x, (byte) (y + yspeed)) != 0) {
				yspeed = -yspeed;
			}

			if (irPanel->getValueAt((byte) (x + xspeed), (byte) (y + yspeed))
					!= 0) {
				xspeed = -xspeed;
				yspeed = -yspeed;
			}
		}

		void setColor(byte r, byte g, byte b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}
		void show(Led_Table_Pixels* strip) {
			for (int xx = 0; xx < 10; xx++)
				for (int yy = 0; yy < 10; yy++) {
					float dist = distSqr(x, y, xx, yy);
					float factor;
					if (dist != 0)
						factor = 1.0 / dist;
					else
						factor = 1.0;

					uint32_t c = strip->getPixelColor(xx, yy);

					int tmp_r = ((c & 0x00ff0000) >> 16);
					tmp_r += r * factor;
					if (tmp_r > 255)
						tmp_r = 255;

					int tmp_g = ((c & 0x00ff00) >> 8);
					tmp_g += g * factor;
					if (tmp_g > 255)
						tmp_g = 255;

					int tmp_b = (c & 0x00ff);
					tmp_b += b * factor;
					if (tmp_b > 255)
						tmp_b = 255;

					strip->setPixelColor(xx, yy, tmp_r, tmp_g, tmp_b);
				}

		}
	};
#define NUMBER_OF_BALLS 2
	Ball balls[NUMBER_OF_BALLS];

public:
	MetaBalls(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
		balls[0].setColor(255, 0, 0);
		//balls[1].setColor(0,255,0);
		balls[1].setColor(0, 0, 255);
	}

	void nextFrame() {
		strip->clear();
		for (int i = 0; i < NUMBER_OF_BALLS; i++) {
			balls[i].update(irPanel);
			balls[i].show(strip);
		}
		for (byte i = 0; i < strip->numPixels(); i++)
			if (irPanel->getValueAt(i))
				strip->setPixelColor(i, 0x00ffffff);

		//	strip->fade(0.82);
	}
};

class Tennis: public Effect {
private:
	class Paddle {
	public:
		uint32_t color;
		byte x;
		byte y;

		Paddle(byte y, uint32_t color) :
				y(y), color(color) {
			x = 4;
		}

		int calcAverageOnIRRow(IRPanel* irPanel, byte row) {
			int sum = 0;
			byte numb_of_actived = 0;
			for (int i = 0; i < 10; i++) {
				if (irPanel->getValueAt(i, row)) {
					sum += i;
					numb_of_actived++;
				}
			}

			if (numb_of_actived != 0)
				return sum / numb_of_actived;
			else
				return -1;
		}

		int updatePosition(IRPanel* irPanel) {
			int pos_x = calcAverageOnIRRow(irPanel, y);
			if (pos_x > 0)
				x = pos_x;
			return pos_x;
		}

		void show(Led_Table_Pixels* strip) {
			strip->setPixelColor(x, y, color);

			if (x - 1 >= 0)
				strip->setPixelColor(x - 1, y, color);

			if (x + 1 < 10)
				strip->setPixelColor(x + 1, y, color);
		}
	};
	class Ball {
	public:
		float x;
		float y;
		float yspeed;
		float xspeed;

		Ball() {
			reset();
		}

		void reset() {
			x = 4;			// random(10);
			y = 4; // random(10);
			yspeed = 0.1;			//random(8) / 10.0 + 0.2;
			xspeed = 0.1;			//random(8) / 10.0 + 0.2;
		}

		void update(IRPanel* irPanel) {
			y += yspeed;
			x += xspeed;
			checkEdges();
		}

		void checkEdges() {
			if ((int) x > 9) {
				xspeed = -xspeed;
				x = 9;
			}
			if ((int) x < 0) {
				xspeed = -xspeed;
				x = 0;
			}
		}

		bool checkWin() {
			if ((int) y >= 9) {
				return true;
			}
			if ((int) y <= 0) {
				return true;
			}
			return false;
		}

		void checkCollisionWithPaddle(Paddle p) {
			//Serial.println(abs((int)(x + xspeed) - (int)(p.x)));
			if ((abs((int)(x + xspeed) - (int)(p.x)) < 2)
					&& ((int) (y + yspeed) == p.y)) {
				yspeed = -yspeed;
				collided();
			}

			if ((abs((int)(x + xspeed) - (int)(p.x)) == 2)
					&& ((int) (y + yspeed) == p.y)) {
				yspeed = -yspeed;
				xspeed = -xspeed;
				collided();
			}
		}

		void collided() {
			if(abs(xspeed*1.05) < 1)
				xspeed *= 1.05;
			if(abs(yspeed*1.05) < 1)
				yspeed *= 1.05;
		}
		void show(Led_Table_Pixels* strip) {
			strip->setPixelColor(x, y, 0x00ff00ff);
		}
	};

	Ball ball;
	Paddle paddle_1, paddle_2;
public:
	Tennis(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel), paddle_1(0, 0x00ff0000), paddle_2(9,
					0x000000ff) {
	}

	void nextFrame() {
		strip->clear();

		ball.update(irPanel);

		if(paddle_1.updatePosition(irPanel) < 0)
			paddle_1.x = ball.x;

		if(paddle_2.updatePosition(irPanel) < 0)
					paddle_2.x = ball.x;

		ball.checkCollisionWithPaddle(paddle_1);
		ball.checkCollisionWithPaddle(paddle_2);

		paddle_1.show(strip);
		paddle_2.show(strip);
		ball.show(strip);

		if (ball.checkWin()) {

			strip->background(0x00ffffff);

			if (abs(ball.y - paddle_1.y) < abs(ball.y - paddle_2.y))
				strip->background(paddle_2.color);
			else
				strip->background(paddle_1.color);

			ball.reset();

			while (strip->getPixelBrightness(11) != 0) {
				strip->fade(0.95);
				strip->show();
			}
		}
	}
};

class Blank: public Effect {

private:
	int rainbow_counter = 0;
	int highestValues[10] = { 0 };
public:
	Blank(Led_Table_Pixels* strip, IRPanel* irPanel) :
			Effect(strip, irPanel) {
	}

	void nextFrame() {
		strip->clear();
		delay(20);
		//blank...
	}
};
