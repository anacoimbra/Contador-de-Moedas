#ifndef UTIL_H
#define UTIL_H
//=================================================================================================
#include "imagem.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
//=================================================================================================
using namespace std;
using namespace cv;
//=================================================================================================
image<rgb> *Mat2image(Mat img){

    int width =  img.cols;
    int height = img.rows;

    image<rgb> *img_result = new image<rgb>(width,height);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            Vec3b intensity = img.at<Vec3b>(y, x);
            int blue = intensity.val[0];
            int green = intensity.val[1];
            int red = intensity.val[2];
            imRef(img_result, x, y).r = red;
            imRef(img_result, x, y).g = green;
            imRef(img_result, x, y).b = blue;
        }
    }

    return img_result;
}
//=================================================================================================
Mat image2Mat(image<rgb> *img){

    int height = img->height();
    int width = img->width();

    Mat img_result(height,width, CV_8UC3);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            uchar blue =imRef(img, x, y).b;
            uchar green = imRef(img, x, y).g;
            uchar red = imRef(img, x, y).r;
            img_result.at<Vec3b>(y,x) = Vec3b(blue,green,red);
        }
    }

    return img_result;
}
//=================================================================================================
#endif // UTIL_H
