
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define PAGE_SIZE 4096

int main9(int argc, char **argv) {
    char *fbname;
    int fbdev;
    struct fb_fix_screeninfo fixed_info;
    struct fb_var_screeninfo var_info;
    struct fb_cmap colormap;
    u_int16_t r, g, b;
    u_int8_t pixel_r, pixel_g, pixel_b;
    int x, y;
    u_int32_t pixel_value;
    void *framebuffer;
    int framebuffer_size;
    int ppc_fix;
    
    if (argc >= 2) {
        fbname = argv[1];
    } else {
        fbname = "/dev/fb0";
    }
    
    printf("Using framebuffer device %s.\n", fbname);
    fbdev = open(fbname, O_RDWR);
    if (fbdev < 0) {
        printf("Error opening %s.\n", fbname);
        return 1;
    }
    if (ioctl(fbdev, FBIOGET_FSCREENINFO, &fixed_info) < 0) {
        printf("Unable to retrieve fixed screen info: %s.\n", strerror(errno));
        close(fbdev);
        return 1;
    }
    printf("Framebuffer ID: %s\n", fixed_info.id);
    printf("Framebuffer type:");
    switch (fixed_info.type) {
        case FB_TYPE_PACKED_PIXELS:
            printf("packed pixels\n");
            break;
        case FB_TYPE_INTERLEAVED_PLANES:
            printf("planar (interleaved)\n");
            break;
        case FB_TYPE_TEXT:
            printf("text (not a framebuffer)\n");
            break;
        case FB_TYPE_VGA_PLANES:
            printf("planar (EGA/VGA)\n");
            break;
        default:
            printf("no idea what this is\n");
            break;
    }
    
    printf("Bytes per scanline: %i\n", fixed_info.line_length);
    
    printf("Visual type:");
    switch(fixed_info.visual) {
        case FB_VISUAL_TRUECOLOR:
            printf("truecolor\n");
            break;
        case FB_VISUAL_PSEUDOCOLOR:
            printf("pseudocolor\n");
            break;
        case FB_VISUAL_DIRECTCOLOR:
            printf("directcolor\n");
            break;
        case FB_VISUAL_STATIC_PSEUDOCOLOR:
            printf("fixed pseudocolor\n");
            break;
        default:
            printf("other (mono perhaps)\n");
            break;
    }
    if (ioctl(fbdev, FBIOGET_VSCREENINFO, &var_info) < 0) {
        printf("Unable to retrieve variable screen info: %s\n", strerror(errno));
        close(fbdev);
        return 1;
    }
    
    printf("Bits per pixel: %i\n", var_info.bits_per_pixel);
    printf("Resolution: %ix%i (virtual %ix%i)\n",
        var_info.xres, var_info.yres,
        var_info.xres_virtual, var_info.yres_virtual);
    printf("Scrolling offset: (%i,%i)\n",
        var_info.xoffset, var_info.yoffset);
    printf("Red channel: %i bits at offset %i\n",
        var_info.red.length, var_info.red.offset);
    printf("Green channel: %i bits at offset %i\n",
        var_info.green.length, var_info.green.offset);
    printf("Blue channel: %i bits at offset %i\n",
        var_info.blue.length, var_info.blue.offset);
    
    ppc_fix = (((long)fixed_info.smem_start) - ((long)fixed_info.smem_start & ~(PAGE_SIZE - 1)));
    framebuffer_size = fixed_info.smem_len + ppc_fix;
    framebuffer = mmap(NULL, framebuffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbdev, 0);
    
    if (framebuffer == NULL) {
        printf("Unable to mmap framebuffer: %s\n", strerror(errno));
        close(fbdev);
        return 1;
    }
    
    printf("Mapped framebuffer.\n");
    
    if ((fixed_info.visual == FB_VISUAL_PSEUDOCOLOR) ||
        (fixed_info.visual == FB_VISUAL_DIRECTCOLOR)) {
            r = 0xFFFF;
            g = 0xFFFF;
            b = 0xFFFF;
            
            colormap.start = 255;
            colormap.red = &r;
            colormap.green = &g;
            colormap.blue = &b;
            colormap.transp = NULL;
            
            if (ioctl(fbdev, FBIOPUTCMAP, &colormap) < 0) {
                printf("WARNING: unable to set colormap.\n");
            }
            pixel_r = 255;
            pixel_g = 255;
            pixel_b = 255;
            pixel_value = 0xFFFFFFFF;
        } else if (fixed_info.visual == FB_VISUAL_TRUECOLOR) {
            pixel_r = 0xFF;
            pixel_g = 0xFF;
            pixel_b = 0xFF;
            pixel_value = (((pixel_r >> (8 - var_info.red.length)) << var_info.red.offset) +
                ((pixel_g >> (8 - var_info.green.length)) << var_info.green.offset) +
                ((pixel_b >> (8 - var_info.blue.length)) << var_info.blue.offset));
        } else {
            printf("Unsupported visual.\n");
            pixel_value = 0;
        }
    
    x = var_info.xres / 2 + var_info.xoffset;
    y = var_info.yres / 2 + var_info.yoffset;
    
    switch(var_info.bits_per_pixel) {
        case 8:
            *((u_int8_t*)framebuffer + fixed_info.line_length * y + x) = (u_int8_t)pixel_value;
            break;
        case 16:
            *((u_int16_t*)framebuffer + fixed_info.line_length / 2 * y + x) = (u_int16_t)pixel_value;
            break;
        case 24:
            *((u_int8_t*)framebuffer + (fixed_info.line_length * y + 3 * x)) = (u_int8_t)pixel_r;
            *((u_int8_t*)framebuffer + (fixed_info.line_length * y + 3 * x) + 1) = (u_int8_t)pixel_g;
            *((u_int8_t*)framebuffer + (fixed_info.line_length * y + 3 * x) + 2) = (u_int8_t)pixel_b;
            break;
        case 32:
            *((u_int32_t*)framebuffer + fixed_info.line_length / 4 * y + x) = (u_int32_t)pixel_value;
            break;
        default:
            printf("Unsupported depth.\n");
            break;
    }
    munmap(framebuffer, framebuffer_size);
    close(fbdev);
    return 0;
}