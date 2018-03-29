/**
 * @file myimage.h
 * @brief Declaration of the MyImage class.
 */

#pragma once

#include <QImage>
#include <QList>

#define MYIMAGE_USE_POINTER

/**
 * @brief The MyImage class inherits from QImage and adds the functionality to get and set pixelrows as QLists directly.
 */
class MyImage : public QImage
{

public:

    /**
     * @brief Inherit constructors from QImage class.
     */
    using QImage::QImage;

    /**
     * @brief This function gets a row of pixels from the image.
     * @param row The index of the row to read.
     * @return QList of bool which represent each bit of the row.
     */
    QList<bool> getPixelRow(const int row) const;

    /**
     * @brief This function sets a row of pixels in the image.
     * @param row The index of the row to write.
     * @param pixelrow The QList of bool which represent each bit of the row.
     * @return true if successful, falso otherwise.
     */
    bool setPixelRow(const int row, const QList<bool>& pixelrow);

};
