#ifndef IMAGEDUPLICATEFINDER_H
#define IMAGEDUPLICATEFINDER_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QtConcurrent>

#include "imageduplicategroups.h"

class ImageDuplicateFinder : public QObject
{
    Q_OBJECT
public:
    explicit ImageDuplicateFinder(QObject *parent = 0);

    virtual ~ImageDuplicateFinder();

    /**
     * @brief images Return the images considered for the duplicate check.
     * @return A list of the images
     */
    const QStringList& images() const { return m_duplicateGroups.images; }

    /**
     * @brief duplicates Return the duplicates among the images. The result is available only after the finished() signal is emitted.
     * @return A list of all indexes of the duplicates.
     */
    const QList<QVector<int> >& duplicates() const { return m_duplicateGroups.duplicateGroups; }

    /**
     * @brief duplicateGroups Return the duplicates among the images. The result is available only after the finished() signal is emitted.
     * @return An object containing the duplicate groups.
     */
    const ImageDuplicateGroups& duplicateGroups() const { return m_duplicateGroups; }

    /**
     * @brief start Start the computation of image duplicates in a list of directories.
     * @param directories A list of directories.
     * @param exploreSubDirectories If true, also compute the duplicates recursively into subdirectories.
     */
    void start(const QStringList& directories, bool exploreSubDirectories = false);

signals:
    /**
     * This signal is emitted when the computation finishes.
     */
    void finished();

    /**
     * @brief progressTextChanged This signal is emitted when the ImageDuplicateFinder reports textual progress information.
     * @param progressText A textual progress information.
     */
    void progressTextChanged(const QString &progressText);

    /**
     * @brief progressRangeChanged The progress range for the ImageDuplicateFinder has changed to minimum and maximum.
     * @param minimum The minimum progress value.
     * @param maximum The maximum progress value.
     */
    void progressRangeChanged(int minimum, int maximum);

    /**
     * @brief progressValueChanged This signal is emitted when the ImageDuplicateFinder reports progress.
     * @param progressValue The current progress.
     */
    void progressValueChanged(int progressValue);

public slots:
    /**
     * @brief cancel Cancel the asynchronous computation.
     */
    void cancel();

    /**
     * @brief reset Reset the ImageDuplicateFinder.
     */
    void reset();

private slots:
    void ListImagesEnd();

    void ComputeHashImagesEnd();

    void FindDuplicatesEnd();

private:
    void ListImages(const QStringList& directories, bool exploreSubDirectories);

    void ComputeHashImages();

    void FindDuplicates();

    /**
     * @brief ListImagesInDirectory List all image files in a list of directories.
     * @param directories A list of directories.
     * @param exploreSubDirectories If true, also list image files recursively into subdirectories.
     * @return
     */
    QStringList ListImagesInDirectory(const QStringList& directories, bool exploreSubDirectories = false) const;

    /**
     * @brief FindDuplicatesInHashes Find couples of duplicate hashes.
     * @return A list of all indexes of the duplicates.
     */
    QList<QVector<int> > FindDuplicatesInHashes() const;

    /**
     * @brief IMG_FORMATS Valid image formats.
     */
    static const QStringList IMG_FORMATS;

    /**
     * @brief THRESHOLD The threshold used to decide if two images are similar.
     */
    const int THRESHOLD;

    ImageDuplicateGroups m_duplicateGroups;

    QVector<quint64> m_hashes;

    QFutureWatcher<QStringList> m_imagesFutureWatcher;

    QFutureWatcher<quint64> m_hashFutureWatcher;

    QFutureWatcher<QList<QVector<int> > > m_duplicatesFutureWatcher;
};

#endif // IMAGEDUPLICATEFINDER_H
