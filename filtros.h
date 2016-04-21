#ifndef FILTROS_H
#define FILTROS_H
//=================================================================================================
#include "imagem.h"
#include <stdio.h>
//=================================================================================================
image<rgb>* filtro_media(image<rgb>* img, int n_mask){

    int mask = n_mask/2;

    image<rgb>*  img_media = img->copy();
    int width = img->width();
    int heigth = img->height();
    int media_r = 0;
    int media_g = 0;
    int media_b = 0;

    for(int y=mask; y<heigth-mask; y++)
        for(int x=mask; x<width-mask; x++){

            media_r = 0;
            media_g = 0;
            media_b = 0;

            for(int m=-mask; m<=mask ;m++)
                for(int n=-mask; n<=mask ;n++){

                    media_r += imRef(img,x+n,y+m).r;
                    media_g += imRef(img,x+n,y+m).g;
                    media_b += imRef(img,x+n,y+m).b;
                }

            imRef(img_media,x,y).r = media_r / (n_mask*n_mask);
            imRef(img_media,x,y).g = media_g / (n_mask*n_mask);
            imRef(img_media,x,y).b = media_b / (n_mask*n_mask);
        }
    return img_media;
}
//=================================================================================================
#endif // FILTROS_H
