/**
 * @file myimage.cpp Definition of the MyImage class' functions.
 */

#include "myimage.h"

#ifdef MYIMAGE_USE_POINTER

QList<bool> MyImage::getPixelRow(const int row) const
{
    QList<bool> pixelrow;

    if(row < 0 || row >= height())
        return pixelrow;

    const uchar* pointer = scanLine(row);

    for(int i = 0; i < bytesPerLine(); i++) {
        for(int j = 7; j >= 0; j--) {
            pixelrow.append(*pointer & (1 << j));
        }
        pointer++;
    }

    return pixelrow;
}

bool MyImage::setPixelRow(const int row, const QList<bool>& pixelrow)
{
    int bits = pixelrow.length();

    if(row < 0 || row >= height())
        return false;

    if(bits != 32*width())
        return false;

    uchar* pointer = scanLine(row);

    for(int i = 0; i < bytesPerLine(); i++) {
        *pointer = 0;
        for(int j = 7; j >= 0; j--) {
            *pointer |= (1 << j) * pixelrow[(8 * i) + (7 - j)];
        }
        pointer++;
    }
    return true;
}

#else // MYIMAGE_USE_POINTER

QList<bool> MyImage::getPixelRow(const int row) const
{
    QList<bool> pixelrow;
    uint32_t pixel_argb;

    if(row < 0 || row >= height())
        return pixelrow;

    for(int i = 0; i < width(); i++) {
        pixel_argb = (uint32_t)pixel(i, row);
        for(int j = 31; j >= 0; j--) {
            pixelrow.append(pixel_argb & (1 << j));
        }
    }

    return pixelrow;
}

bool MyImage::setPixelRow(const int row, const QList<bool>& pixelrow)
{
    uint32_t pixel_argb;
    int bits = pixelrow.length();

    if(row < 0 || row >= height())
        return false;

    if(bits != 32*width())
        return false;

    for(int i = 0; i < width(); i++) {
        pixel_argb = 0;
        for(int j = 31; j >= 0; j--) {
            pixel_argb |= (1 << j) * pixelrow[(32 * i) + (31 - j)];
        }
        setPixel(i, row, pixel_argb);
    }
    return true;
}

#endif // !MYIMAGE_USE_POINTER
