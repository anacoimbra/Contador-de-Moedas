#ifndef IMAGEM_IO_H
#define IMAGEM_IO_H
//=================================================================================================
#include <cstdlib>
#include <climits>
#include <cstring>
#include <fstream>
#include "imagem.h"
//=================================================================================================
#define BUF_SIZE 256
//=================================================================================================
class pnm_error { };
//=================================================================================================
/* read PNM field, skipping comments */
static void pnm_read(std::ifstream &file, char *buf) {
    char doc[BUF_SIZE];
    char c;
    file >> c;
    while (c == '#') {
        file.getline(doc, BUF_SIZE);
        file >> c;
    }
    file.putback(c);
    file.width(BUF_SIZE);
    file >> buf;
    file.ignore();
}
//=================================================================================================
static image<rgb> *load_ppm(const char *name) {
    char buf[BUF_SIZE];
    /* read header */
    std::ifstream file(name, std::ios::in | std::ios::binary);
    pnm_read(file, buf);
    if (strncmp(buf, "P6", 2))
        throw pnm_error();
    pnm_read(file, buf);
    int width = atoi(buf);
    pnm_read(file, buf);
    int height = atoi(buf);
    pnm_read(file, buf);
    if (atoi(buf) > UCHAR_MAX)
        throw pnm_error();
    /* read data */
    image<rgb> *im = new image<rgb>(width, height);
    file.read((char *)imPtr(im, 0, 0), width * height * sizeof(rgb));
    return im;
}
//=================================================================================================
static void save_ppm(image<rgb> *im, const char *name) {
    int width = im->width();
    int height = im->height();
    std::ofstream file(name, std::ios::out | std::ios::binary);

    file << "P6\n" << width << " " << height << "\n" << UCHAR_MAX << "\n";
    file.write((char *)imPtr(im, 0, 0), width * height * sizeof(rgb));
}
//=================================================================================================
#endif // IMAGEM_IO_H
