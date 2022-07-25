#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
	if (x > im.w) {
		x = im.w - 1;
	}
	if (y > im.h) {
		y = im.h - 1;
	}
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	float pixel = im.data[x + im.w*y + im.w*im.h*c];
	return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
	if (x > im.w) {
		x = im.w - 1;
	}
	if (y > im.h) {
		y = im.h - 1;
	}
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	im.data[x + im.w*y + im.w*im.h*c] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
	float v;
	for (int k = 0; k < im.c; k++)
		for (int j = 0; j < im.h; j++)
			for (int i = 0; i < im.w; i++) {
				v = get_pixel(im, i, j, k);
				set_pixel(copy, i, j, k, v);
			}
    return copy;
}

image rgb_to_grayscale(image im) {
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
	float v;
		for (int j = 0; j < im.h; j++)
			for (int i = 0; i < im.w; i++) {//red
				v = get_pixel(im, i, j, 0);
				v = v*.299;
				set_pixel(gray, i, j, 0, v);
				}
		for (int j = 0; j < im.h; j++)
			for (int i = 0; i < im.w; i++) {//red
				v = get_pixel(im, i, j, 1);
				v = v*.587 + get_pixel(gray,i,j,0);
				set_pixel(gray, i, j, 0, v);
			}
		for (int j = 0; j < im.h; j++)
			for (int i = 0; i < im.w; i++) {//red
				v = get_pixel(im, i, j, 2);
				v = v*.114 + get_pixel(gray, i, j, 0);
				set_pixel(gray, i, j, 0, v);
			}
    return gray;
}

void shift_image(image im, int c, float v)
{
	float p;
	for (int j = 0; j < im.h; j++)
		for (int i = 0; i < im.w; i++) {
			p = get_pixel(im, i, j, c);
			p = p + v;
			set_pixel(im, i, j, c, p);
		}
}

void clamp_image(image im)
{
	float v;
	for (int k = 0; k < im.c; k++)
		for (int j = 0; j < im.h; j++)
			for (int i = 0; i < im.w; i++) {
				v = get_pixel(im, i, j, k);
				if (v < 0) {
					v = 0;
				}
				if (v > 1) {
					v = 1;
				}
				set_pixel(im, i, j, k, v);
			}
    
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
	float R, G, B;
	float H1, H2, S, V;
	float m, C;
	for (int i = 0; i < im.w*im.h; i++) {
		R = im.data[0 * (im.h*im.w) + i];
		G = im.data[1 * (im.h*im.w) + i];
		B = im.data[2 * (im.h*im.w) + i];
		V = three_way_max(R, G, B);
		m = three_way_min(R, G, B);
		C = V - m;
		if (V == 0)
			S = 0;
		else
			S = C / V;
		
		if (C == 0)
			H2 = 0;
		else {
			if (V == R)
				H1 = (G - B) / C;
			if (V == G)
				H1 = (B - R) / C + 2;
			if (V == B)
				H1 = (R - G) / C + 4;
			if (H1 < 0)
				H2 = H1 / 6 + 1;
			else
				H2 = H1 / 6;
		}
		im.data[0 * (im.w*im.h) + i] = H2;
		im.data[1 * (im.w*im.h) + i] = S;
		im.data[2 * (im.w*im.h) + i] = V;
	}

}


void hsv_to_rgb(image im)
{
	float H1, H2, H3, S, V;
	float m, c;
	for (int i = 0; i < im.h*im.w; i++) {
		V = im.data[2 * (im.h*im.w) + i];
		S = im.data[1 * (im.h*im.w) + i];
		c = S*V;
		m = V - c;
		H1 = im.data[0 * (im.h*im.w) + i];
		H2 = H1 * 6;
		H3 = (H1 - 1) * 6;
		if (c == 0) {
			im.data[0 * (im.h*im.w) + i] = m;
			im.data[1 * (im.h*im.w) + i] = m;
			im.data[2 * (im.h*im.w) + i] = m;
		}
		else if (H2 >= 0 && H2 <= 1) {
			im.data[0 * (im.h*im.w) + i] = V;
			im.data[1 * (im.h*im.w) + i] = (c*H2) + m;
			im.data[2 * (im.h*im.w) + i] = m;
		}
		else if (H2 > 1 && H2 <= 2) {
			im.data[0 * (im.h*im.w) + i] = -1 * c*(H2 - 2) + m;
			im.data[1 * (im.h*im.w) + i] = V;
			im.data[2 * (im.h*im.w) + i] = m;
		}
		else if (H2 > 2 && H2 <= 3) {
			im.data[0 * (im.h*im.w) + i] = m;
			im.data[1 * (im.h*im.w) + i] = V;
			im.data[2 * (im.h*im.w) + i] = c*(H2 - 2) + m;
		}
		else if (H2 > 3 && H2 <= 4) {
			im.data[0 * (im.h*im.w) + i] = m;
			im.data[1 * (im.h*im.w) + i] = -1 * c*(H2 - 4) + m;
			im.data[2 * (im.h*im.w) + i] = V;
		}
		else if (H2 > 4 && H2 <= 5) {
			im.data[0 * (im.h*im.w) + i] = c*(H2 - 4) + m;
			im.data[1 * (im.h*im.w) + i] = m;
			im.data[2 * (im.h*im.w) + i] = V;
		}
		else if (H2 > 5 && H2 <= 6) {
			im.data[0 * (im.h*im.w) + i] = V;
			im.data[1 * (im.h*im.w) + i] = m;
			im.data[2 * (im.h*im.w) + i] = -1 * c*H3 + m;
		}
	}
}
