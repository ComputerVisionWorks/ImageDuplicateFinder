#ifndef DCTPERCEPTUALHASH_H
#define DCTPERCEPTUALHASH_H

#include <QtGlobal>
#include <QString>

/**
 * @brief DctPerceptualHash
 * @param file_path
 * @return
 */
quint64 DctPerceptualHash(const QString& file_path);

/**
 * @brief DctPerceptualHashDistance Compute a Hamming distance between two DCT perceptual hashes.
 * @param x A 64 bits DCT perceptual hash.
 * @param y A 64 bits DCT perceptual hash.
 * @return An integer distance between 0 and 64.
 */
int DctPerceptualHashDistance(quint64 x, quint64 y);

#endif // DCTPERCEPTUALHASH_H
