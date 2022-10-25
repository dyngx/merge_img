#include <math.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"



    unsigned char *uc_arrayNew_1d(int _size)
    {
        return (unsigned char *)calloc(_size, sizeof(unsigned char));
    }
int count = 0;

unsigned char *change_background(unsigned char *weather_forecast, unsigned char *background, unsigned char *foreground, int width, int height, int channel, int channel_bg, int channel_fg)
{
    unsigned char *SubtractImg = uc_arrayNew_1d(width * height * channel);
    int sum_abs;
    int i,j,k;

    for ( i = 0; i < height; i++)
    {
        for ( j = 0; j < width; j++)
        {
            count = 0;
            for ( k = 0; k < channel; k++)
                {
                    if (abs(background[i * width * channel_bg + j * channel_bg + k] - foreground[i * width * channel_fg + j * channel_fg + k] < 40))
                    {
                        count++;
                    }
                }
            if (count < 3)
            {
                for( k = 0; k < channel; k++){
                    weather_forecast[i * width * channel + j * channel + k] = foreground[i * width * channel_fg + j * channel_fg + k];
                }
            }
        }
    }
    return weather_forecast;
}


int main()
{
    // declare variables
    int width, height, channel;
    int width_bg, height_bg, channel_bg;
    int width_fg, height_fg, channel_fg;

    char background_img[] = "./images/background.png";
    char foreground_img[] = "./images/foreground.png";
    char weather_forecast_img[] = "./images/new_background.png";
    char save_path[] = "./images/result.png";

    // read image data
    unsigned char *background = stbi_load(background_img, &width_bg, &height_bg, &channel_bg, 0);
    unsigned char *foreground = stbi_load(foreground_img, &width_fg, &height_fg, &channel_fg, 0);
    unsigned char *weather_forecast = stbi_load(weather_forecast_img, &width, &height, &channel, 0);

    // Output ERROR if NO IMAGE
    if (background == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    if (foreground == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    if (weather_forecast == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    //Show properties
    printf("Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel);
    printf("\nWidth_bg = %d\nHeight_bg = %d\nChannel_bg = %d\n", width_bg, height_bg, channel_bg);
    printf("\nWidth_fg = %d\nHeight_fg = %d\nChannel_fg = %d\n", width_fg, height_fg, channel_fg);
    
    //Call Func and SAVE FILE
    unsigned char *cimage = change_background(weather_forecast, background, foreground, width, height,channel, channel_bg, channel_fg);
    stbi_write_png(save_path, width, height, 3, cimage, width * channel);
    printf("New image saved to %s\n", save_path);
}