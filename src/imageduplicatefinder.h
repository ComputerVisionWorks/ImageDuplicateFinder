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

    /**
     * @brief DEFAULT_THRESHOLD Default value for threshold.
     */
    static const int DEFAULT_THRESHOLD = 10;

    /**
     * @brief threshold Return the threshold value.
     * @return The threshold value.
     */
    int threshold() const { return m_threshold; }

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
     * @brief setThreshold Set the threshold value.
     * @param threshold The new threshold value.
     */
    void setThreshold(int threshold) { m_threshold = threshold; }

    /**
     * @brief cancel Cancel the asynchronous computation.
     */
    void cancel();

    /**
     * @brief reset Reset the ImageDuplicateFinder.
     */
    void reset();

private:
    /**
     * @brief Configure the QFutureWatchers
     */
    void ConfigureFutureWatchers();

    /**
     * @brief ListImagesInDirectory List all image files in a list of directories.
     * @param directories A list of directories.
     * @param exploreSubDirectories If true, also list image files recursively into subdirectories.
     * @param img_formats Valid image formats.
     * @return
     */
    static QStringList ListImagesInDirectory(const QStringList& directories, bool exploreSubDirectories, const QStringList &img_formats);

    /**
     * @brief FindDuplicatesInHashes Find couples of duplicate hashes.
     * @return A list of all indexes of the duplicates.
     */
    static QList<QVector<int> > FindDuplicatesInHashes(const std::vector<quint64> &hashes, int threshold);

    /**
     * @brief IMG_FORMATS Valid image formats.
     */
    static const QStringList IMG_FORMATS;

    /**
     * @brief m_threshold The threshold used to decide if two images are similar.
     */
    int m_threshold;

    ImageDuplicateGroups m_duplicateGroups;

    QFutureWatcher<QStringList> *m_imagesFutureWatcher;

    QFutureWatcher<quint64> *m_hashFutureWatcher;

    QFutureWatcher<QList<QVector<int> > > *m_duplicatesFutureWatcher;
};

#endif // IMAGEDUPLICATEFINDER_H
