#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i=0;i<height;i++){
        for(int j=0; j<width;j++){
            int mean=round((image[i][j].rgbtBlue+image[i][j].rgbtGreen+image[i][j].rgbtRed)/3.0);
            image[i][j].rgbtBlue=mean;
            image[i][j].rgbtGreen=mean;
            image[i][j].rgbtRed=mean;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i=0;i<height;i++){
        for(int j=0;j<width/2;j++){
            RGBTRIPLE a=image[i][j];
            RGBTRIPLE b=image[i][width-1-j];
            image[i][j]=b;
            image[i][width-1-j]=a;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE im[height][width];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            im[i][j]=image[i][j];
        }
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            int meanBlue=0x00;
            int meanGreen=0x00;
            int meanRed=0x00;
            float ctr=0;
            for(int k=0;k<3;k++){
                for(int l=0;l<3;l++){
                    if((i-1+k)>=0 && (j-1+l)>=0 && (i-1+k)<height && (j-1+l) < width){
                        ctr=ctr+1.0;
                        meanBlue=meanBlue+im[i-1+k][j-1+l].rgbtBlue;
                        meanGreen=meanGreen+im[i-1+k][j-1+l].rgbtGreen;
                        meanRed=meanRed+im[i-1+k][j-1+l].rgbtRed;
                    }
                }
            }

            image[i][j].rgbtBlue=round(meanBlue/ctr);
            image[i][j].rgbtGreen=round(meanGreen/ctr);
            image[i][j].rgbtRed=round(meanRed/ctr);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3]={{-1 , 0 , 1}, {-2 , 0 , 2}, {-1 , 0 , 1}};
    int Gy[3][3]={{-1 , -2 , -1} , { 0 , 0 , 0} , {1 , 2 , 1}};

    RGBTRIPLE im[height+2][width+2];
    for(int i=0;i<height+2;i++){
        for(int j=0;j<width+2;j++){
            if(i==0||j==0|| i==(height+1) || j==(width+1)){
                im[i][j].rgbtBlue=0;
                im[i][j].rgbtGreen=0;
                im[i][j].rgbtRed=0;
            }

            else{
                im[i][j]=image[i][j];
            }
        }
    }

    for(int i=1; i<height+1;i++){
        for(int j=1;j<width+1;j++){

            int Gxrgb[3]={0,0,0};
            int Gyrgb[3]={0,0,0};

            for(int k=0;k<3;k++){
                for(int l=0;l<3;l++){
                    int pixrgb[3]={im[i+(k-1)][j+(l-1)].rgbtRed, im[i+(k-1)][j+(l-1)].rgbtGreen, im[i+(k-1)][j+(l-1)].rgbtBlue};
                    for(int color=0;color<3;color++){
                        Gxrgb[color]=Gxrgb[color]+(pixrgb[color]*Gx[k][l]);
                        Gyrgb[color]=Gyrgb[color]+(pixrgb[color]*Gy[k][l]);
                    }
                }
            }
            float magnitudeRed = sqrt(Gxrgb[0] * Gxrgb[0] + Gyrgb[0] * Gyrgb[0]);
            float magnitudeGreen = sqrt(Gxrgb[1] * Gxrgb[1] + Gyrgb[1] * Gyrgb[1]);
            float magnitudeBlue = sqrt(Gxrgb[2] * Gxrgb[2] + Gyrgb[2] * Gyrgb[2]);

            image[i - 1][j - 1].rgbtRed = (magnitudeRed <= 255) ? round(magnitudeRed) : 255;
            image[i - 1][j - 1].rgbtGreen = (magnitudeGreen <= 255) ? round(magnitudeGreen) : 255;
            image[i - 1][j - 1].rgbtBlue = (magnitudeBlue <= 255) ? round(magnitudeBlue) : 255;

        }
    }



    return;
}
