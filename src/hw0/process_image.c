#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    assert(c < 3 && c > -1);
    if (x < 0) {
        if (y < 0) {
            return im.data[c*im.w*im.h];
        } else if (y >= im.h) {
            return im.data[c*im.w*im.h + (im.h-1)*im.w];
        } else {
            return im.data[c*im.w*im.h + y*im.w];
        }
    } else if (x >= im.w) {
        if (y < 0) {
            return im.data[c*im.w*im.h + im.w - 1];
        } else if (y >= im.h) {
            return im.data[c*im.w*im.h + (im.h-1)*im.w + im.w - 1];
        } else {
            return im.data[c*im.w*im.h + y*im.w + im.w - 1];
        }
    } else {
        if (y < 0) {
            return im.data[c*im.w*im.h + x];
        } else if (y >= im.h) {
            return im.data[c*im.w*im.h + (im.h-1)*im.w + x];
        } else {
            return im.data[c*im.w*im.h + y*im.w + x];
        }
    }
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (!(x < 0 || x > im.w || y < 0 || y > im.h || c < 0 || c > im.c))
    {
        im.data[c*im.w*im.h + y*im.w + x] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w*im.h*im.c*sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            gray.data[i*im.h + j] = 0.299 * im.data[i*im.h + j] + 0.587 * im.data[im.w*im.h + i*im.h + j] + .114 * im.data[2*im.w*im.h + i*im.h + j];
        }
        
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    assert(c < im.c && c > -1);
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            im.data[c*im.w*im.h + i*im.w + j] = im.data[c*im.w*im.h + i*im.w + j] + v;
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.w; k++) {
                if (im.data[i*im.w*im.h + j*im.w + k] > 1) {
                    im.data[i*im.w*im.h + j*im.w + k] = 1;
                } else if (im.data[i*im.w*im.h + j*im.w + k] < 0) {
                    im.data[i*im.w*im.h + j*im.w + k] = 0;
                }
            }
        }
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
    // TODO Fill this in
    assert(im.c == 3);
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            float r, g, b, h, s, v, c;

            r = im.data[i*im.w + j];
            g = im.data[im.w*im.h + i*im.w + j];
            b = im.data[2*im.w*im.h + i*im.w + j];

            v = three_way_max(r, g, b);
            c = v - three_way_min(r, g, b);
            s = (v == 0) ? 0 : c / v;

            // hue calculation
            if (c == 0) {
                h = 0;
            } else if (v == r) {
                h = (g - b) / c;
            } else if (v == g) {
                h = (b - r) / c + 2;
            } else {
                h = (r - g) / c + 4;
            }

            if (h < 0) {
                h = h / 6 + 1;
            } else {
                h = h / 6;
            }

            // update image data
            im.data[i*im.w + j] = h;
            im.data[im.w*im.h + i*im.w + j] = s;
            im.data[2*im.w*im.h + i*im.w + j] = v;
        }
        
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    assert(im.c == 3);
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            float r, g, b, h, s, v, c;

            h = im.data[i*im.w + j];
            s = im.data[im.w*im.h + i*im.w + j];
            v = im.data[2*im.w*im.h + i*im.w + j];

            c = s * v;
            h = h * 6;

            // rgb calculation
            if (c == 0) {
                r = v;
                g = v;
                b = v;
            } else {
                if (h < 1 || h >= 5) {
                    // red is max
                    r = v;
                    h = (h >= 5) ? ((h - 6) * c) : (h * c);
                    if (h < 0) {
                        // green is min
                        g = v - c;
                        b = g - h;
                    } else {
                        // blue is min
                        b = v - c;
                        g = b + h;
                    }
                } else if (h < 3) {
                    // green is max
                    g = v;
                    h = h - 2;
                    if (h < 0) {
                        // blue is min
                        b = v - c;
                        r = b - h;
                    } else {
                        // red is min
                        r = v - c;
                        b = r + h;
                    }
                } else {
                    // blue is max
                    b = v;
                    h = h - 4;
                    if (h < 0) {
                        // red is min
                        r = v - c;
                        g = r - h;
                    } else {
                        // green is min
                        g = v - c;
                        r = g + h;
                    }
                }
            }

            // update image data
            im.data[i*im.w + j] = r;
            im.data[im.w*im.h + i*im.w + j] = g;
            im.data[2*im.w*im.h + i*im.w + j] = b;
        }
        
    }
}
