#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    float roundx = (int)x + 0.5;
    float roundy = (int)y + 0.5;
    if (x < roundx) {
        if (y < roundy) {
            return im.data[c*im.w*im.h + ((int)y)*im.w + (int)x];
        } else {
            return im.data[c*im.w*im.h + ((int)y+1)*im.w + (int)x];
        }
    } else {
        if (y < roundy) {
            return im.data[c*im.w*im.h + ((int)y)*im.w + (int)x + 1];
        } else {
            return im.data[c*im.w*im.h + ((int)y+1)*im.w + (int)x + 1];
        }
    }
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image new_im = make_image(w, h, im.c);
    float a_w = im.w/w;
    float a_h = im.h/h;
    float b_w = 0.5 * (a_w - 1);
    float b_h = 0.5 * (a_h - 1);
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < w; k++) {
                new_im.data[i*im.w*im.h + j*im.w + k] = nn_interpolate(im, a_w*k + b_w, a_h*j + b_h, i);
            }
        }
    }
    return new_im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float d1, d2, d3, d4, q1, q2;
    d1 = x - (int)x;
    d2 = ceil(x) - x;
    d3 = y - (int)y;
    d4 = ceil(y) - y;
    q1 = im.data[c*im.w*im.h + ((int)y)*im.w + (int)x] * d2 + im.data[c*im.w*im.h + ((int)y)*im.w + (int)x + 1] * d1;
    q2 = im.data[c*im.w*im.h + ((int)y+1)*im.w + (int)x] * d2 + im.data[c*im.w*im.h + ((int)y+1)*im.w + (int)x + 1] * d1;
    return q1*d4 + q2*d3;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image new_im = make_image(w, h, im.c);
    float a_w = im.w/w;
    float a_h = im.h/h;
    float b_w = 0.5 * (a_w - 1);
    float b_h = 0.5 * (a_h - 1);
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < w; k++) {
                new_im.data[i*im.w*im.h + j*im.w + k] = nn_interpolate(im, a_w*k + b_w, a_h*j + b_h, i);
            }
        }
    }
    return new_im;
}

