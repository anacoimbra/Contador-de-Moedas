#ifndef IMAGE_H
#define IMAGE_H
//=================================================================================================
typedef unsigned char uchar;
typedef struct { uchar r, g, b; } rgb;
//=================================================================================================
/* use imRef to access image data. */
#define imRef(im, x, y) (im->access[y][x])
/* use imPtr to get pointer to image data. */
#define imPtr(im, x, y) &(im->access[y][x])
//=================================================================================================
template <class T>
class image {
private:
    int w, h;
public:
    /* create an image */
    image(const int width, const int height, const bool init = true);
    /* delete an image */
    ~image();
    /* copy an image */
    image<T> *copy() const;
    /* get the width of an image. */
    int width() const { return w; }
    /* get the height of an image. */
    int height() const { return h; }
    /* image data. */
    T *data;
    /* row pointers. */
    T **access;
};
//=================================================================================================
template <class T>
image<T>::image(const int width, const int height, const bool init) {
    w = width;
    h = height;
    data = new T[w * h];  // allocate space for image data
    access = new T*[h];   // allocate space for row pointers

    // initialize row pointers
    for (int i = 0; i < h; i++)
        access[i] = data + (i * w);

    if (init)
        memset(data, 0, w * h * sizeof(T));
}
//=================================================================================================
template <class T>
image<T>::~image() {
    delete [] data;
    delete [] access;
}
//=================================================================================================
template <class T>
image<T> *image<T>::copy() const {
    image<T> *im = new image<T>(w, h, false);
    memcpy(im->data, data, w * h * sizeof(T));
    return im;
}
//=================================================================================================
#endif

