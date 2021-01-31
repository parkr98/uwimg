#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float sum = 0;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                sum += get_pixel(im, x, y, c);
            }
        }
    }
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                set_pixel(im, x, y, c, get_pixel(im, x, y, c) / sum);
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    image filter = make_image(w, w, 1);
    for (int y = 0; y < w; y++) {
        for (int x = 0; x < w; x++) {
            set_pixel(filter, x, y, 0, 1);
        }
    }
    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == 1 || filter.c == im.c);
    image conv;
    if (im.c == 1) {
        conv = make_image(im.w, im.h, 1);
        // 1 1 1
        // 1 1 0
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float sum = 0;
                int offset = (int)((filter.w-1) / 2);
                for (int offsetY = 0; offsetY < filter.h; offsetY++) {
                    for (int offsetX = 0; offsetX < filter.w; offsetX++) {
                        sum += get_pixel(im, x + offsetX - offset, y + offsetY - offset, 0) * get_pixel(filter, offsetX, offsetY, 0);
                    }
                }
                set_pixel(conv, x, y, 0, sum);
            }
        }
    } else if (preserve == 0) {
        conv = make_image(im.w, im.h, 1);
        if (filter.c == 1) {
            // c 1 0
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0;
                    int offset = (int)((filter.w-1) / 2);
                    for (int c = 0; c < im.c; c++) {
                        for (int offsetY = 0; offsetY < filter.h; offsetY++) {
                            for (int offsetX = 0; offsetX < filter.w; offsetX++) {
                                sum += get_pixel(im, x + offsetX - offset, y + offsetY - offset, c) * get_pixel(filter, offsetX, offsetY, 0);
                            }
                        }
                    }
                    set_pixel(conv, x, y, 0, sum);
                }
            }
        } else {
            // c c 0
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0;
                    int offset = (int)((filter.w-1) / 2);
                    for (int c = 0; c < im.c; c++) {
                        for (int offsetY = 0; offsetY < filter.h; offsetY++) {
                            for (int offsetX = 0; offsetX < filter.w; offsetX++) {
                                sum += get_pixel(im, x + offsetX - offset, y + offsetY - offset, c) * get_pixel(filter, offsetX, offsetY, c);
                            }
                        }
                    }
                    set_pixel(conv, x, y, 0, sum);
                }
            }
        }
    } else {
        conv = make_image(im.w, im.h, im.c);
        if (filter.c == 1) {
            // c 1 1
            for (int c = 0; c < im.c; c++) {
                for (int y = 0; y < im.h; y++) {
                    for (int x = 0; x < im.w; x++) {
                        float sum = 0;
                        int offset = (int)((filter.w-1) / 2);
                        for (int offsetY = 0; offsetY < filter.h; offsetY++) {
                            for (int offsetX = 0; offsetX < filter.w; offsetX++) {
                                float imPixel = get_pixel(im, x + offsetX - offset, y + offsetY - offset, c);
                                float fPixel = get_pixel(filter, offsetX, offsetY, 0);
                                sum += (imPixel * fPixel);
                            }
                        }
                        //printf("%f", sum);
                        set_pixel(conv, x, y, c, sum);
                    }
                }
            }
        } else {
            // c c 1
            for (int c = 0; c < im.c; c++) {
                for (int y = 0; y < im.h; y++) {
                    for (int x = 0; x < im.w; x++) {
                        float sum = 0;
                        int offset = (int)((filter.w-1) / 2);
                        for (int offsetY = 0; offsetY < filter.h; offsetY++) {
                            for (int offsetX = 0; offsetX < filter.w; offsetX++) {
                                sum += get_pixel(im, x + offsetX - offset, y + offsetY - offset, c) * get_pixel(filter, offsetX, offsetY, c);
                            }
                        }
                        set_pixel(conv, x, y, c, sum);
                    }
                }
            }
        }
    }
    return conv;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We should use preserve with sharpen but not with higpass and emboss because shapening is often used bring out the edges in
// an image without modifying the original image too much, whereas emboss and highpass exaggerate the edges more aggressively, and grayscale
// images highlight this effect better.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: We should do some type of normalization for all of these because the returned images likely have some overflow/underflow.

image make_gaussian_filter(float sigma)
{
    // TODO
    int w = (int)(sigma * 6);
    if (w % 2 == 0) {
        w += 1;
    }
    int offset = (int)(w/2);
    image filter = make_image(w, w, 1);
    for (int y = 0; y < w; y++) {
        for (int x = 0; x < w; x++) {
            float guassian = 1 / (2 * M_PI * pow(sigma, 2) * pow(M_E, (pow(x-offset, 2) + pow(y-offset, 2)) / (2 * pow(sigma, 2))));
            set_pixel(filter, x, y, 0, guassian);
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im = make_image(a.w, a.h, a.c);
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float pixel = get_pixel(a, x, y, c) + get_pixel(b, x, y, c);
                set_pixel(im, x, y, c, pixel);
            }
        }
    }
    return im;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im = make_image(a.w, a.h, a.c);
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float pixel = get_pixel(a, x, y, c) - get_pixel(b, x, y, c);
                set_pixel(im, x, y, c, pixel);
            }
        }
    }
    return im;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float min = get_pixel(im, 0, 0, 0);
    float max = get_pixel(im, 0, 0, 0);
    // Find min & max
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float pixel = get_pixel(im, x, y, c);
                min = (pixel < min) ? pixel : min;
                max = (pixel > max) ? pixel : max;
            }
        }
    }

    // Normalize
    float range = max - min;
    if (range == 0) {
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    set_pixel(im, x, y, c, 0);
                }
            }
        }
    } else {
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float pixel = get_pixel(im, x, y, c);
                    set_pixel(im, x, y, c, (pixel - min) / range);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image gxFilter = make_gx_filter();
    image gyFilter = make_gy_filter();
    image gx = convolve_image(im, gxFilter, 0);
    image gy = convolve_image(im, gyFilter, 0);
    free_image(gxFilter);
    free_image(gyFilter);
    image grad = make_image(im.w, im.h, 1);
    image dir = make_image(im.w, im.h, 1);
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float gradient = sqrt(pow(get_pixel(gx, x, y, 0), 2) + pow(get_pixel(gy, x, y, 0), 2));
            float direction = atan2(get_pixel(gy, x, y, 0), get_pixel(gx, x, y, 0));
            set_pixel(grad, x, y, 0, gradient);
            set_pixel(dir, x, y, 0, direction);
        }
    }
    free_image(gx);
    free_image(gy);
    image *ret = calloc(2, sizeof(image));
    ret[0] = grad;
    ret[1] = dir;
    return ret;
}

image colorize_sobel(image im)
{
    // TODO
    image *sobel = sobel_image(im);
    image grad = sobel[0];
    image dir = sobel[1];
    feature_normalize(grad);
    feature_normalize(dir);
    free(sobel);
    image colorized = make_image(im.w, im.h, 3);
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            set_pixel(colorized, x, y, 0, get_pixel(dir, x, y, 0));
            set_pixel(colorized, x, y, 1, get_pixel(grad, x, y, 0));
            set_pixel(colorized, x, y, 2, get_pixel(grad, x, y, 0));
        }
    }
    hsv_to_rgb(colorized);
    return colorized;
}
