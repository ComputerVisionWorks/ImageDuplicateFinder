#include <algorithm>
#include <vector>

#include <QDir>
#include <QDirIterator>

#include "imageduplicatefinder.h"
#include "unionfind.h"
#include "dctperceptualhash.h"

const QStringList ImageDuplicateFinder::IMG_FORMATS = {"*.jpg", "*.png"};

ImageDuplicateFinder::ImageDuplicateFinder(QObject *parent) : QObject(parent), m_threshold(DEFAULT_THRESHOLD)
{
    m_imagesFutureWatcher = new QFutureWatcher<QStringList>(this);
    m_hashFutureWatcher = new QFutureWatcher<quint64>(this);
    m_duplicatesFutureWatcher = new QFutureWatcher<QList<QVector<int> > >();

    ConfigureFutureWatchers();
}

// Start the computation by listing all images
void ImageDuplicateFinder::start(const QStringList& directories, bool exploreSubDirectories) {
    emit progressTextChanged("Listing images to analyse");
    emit progressRangeChanged(0, 0);
    emit progressValueChanged(0);

    m_duplicateGroups.directories = directories;

    m_imagesFutureWatcher->setFuture(QtConcurrent::run(&ImageDuplicateFinder::ListImagesInDirectory, directories, exploreSubDirectories, IMG_FORMATS));
}

void ImageDuplicateFinder::cancel() {
    m_imagesFutureWatcher->cancel();
    m_hashFutureWatcher->cancel();
    m_duplicatesFutureWatcher->cancel();
    reset();
}

void ImageDuplicateFinder::reset() {
    m_duplicateGroups.clear();
}

void ImageDuplicateFinder::ConfigureFutureWatchers()
{
    // When the ListImages task finishes
    auto onListImageFinished = [this]() {
        m_duplicateGroups.images = m_imagesFutureWatcher->result();

        emit progressTextChanged("Analysing images");

        m_hashFutureWatcher->setFuture(QtConcurrent::mapped(m_duplicateGroups.images, DctPerceptualHash));
    };
    QObject::connect(m_imagesFutureWatcher, &QFutureWatcher<QStringList>::finished, onListImageFinished);

    // When the ComputeHashImages task finishes
    auto onComputeHashImageFinished = [this]() {
        std::vector<quint64> hashes = m_hashFutureWatcher->future().results().toVector().toStdVector();

        emit progressTextChanged("Finding duplicates");
        emit progressRangeChanged(0, 0);
        emit progressValueChanged(0);

        m_duplicatesFutureWatcher->setFuture(QtConcurrent::run(ImageDuplicateFinder::FindDuplicatesInHashes, hashes, m_threshold));
    };
    QObject::connect(m_hashFutureWatcher, &QFutureWatcher<quint64>::finished, onComputeHashImageFinished);
    // When the ComputeHashImages task reports progress, the signals are passed through.
    QObject::connect(m_hashFutureWatcher, &QFutureWatcher<quint64>::progressRangeChanged, this, &ImageDuplicateFinder::progressRangeChanged);
    QObject::connect(m_hashFutureWatcher, &QFutureWatcher<quint64>::progressValueChanged, this, &ImageDuplicateFinder::progressValueChanged);

    // When the FindDuplicates task finishes
    auto onFindDuplicatesFinished = [this]() {
        m_duplicateGroups.duplicateGroups = m_duplicatesFutureWatcher->future().result();

        emit finished();
    };
    QObject::connect(m_duplicatesFutureWatcher, &QFutureWatcher<QList<QVector<int> > >::finished, onFindDuplicatesFinished);
}

QStringList ImageDuplicateFinder::ListImagesInDirectory(const QStringList& directories, bool exploreSubDirectories, const QStringList &img_formats) {
    QSet<QString> imagesSet;

    QDirIterator::IteratorFlags iteratorFlags = QDirIterator::NoIteratorFlags;
    if (exploreSubDirectories) {
        iteratorFlags = QDirIterator::Subdirectories;
    }

    // List all files in all directories.
    for (QStringList::const_iterator itDirPath = directories.begin(); itDirPath != directories.end(); ++itDirPath) {
        QDirIterator itDir(*itDirPath, img_formats, QDir::Files, iteratorFlags);
        while (itDir.hasNext()) {
            imagesSet.insert(itDir.next());
        }
    }

    return imagesSet.toList();
}

QList<QVector<int> > ImageDuplicateFinder::FindDuplicatesInHashes(const std::vector<quint64> &hashes, int threshold) {
    QList<QVector<int> > duplicates;

    const unsigned int nb_hashes = static_cast<unsigned int>(hashes.size());

    UnionFind union_find(nb_hashes);

    for (unsigned int i = 0; i < hashes.size(); i++) {
        for (unsigned int j = i + 1; j < hashes.size(); j++) {
            if (DctPerceptualHashDistance(hashes[i], hashes[j]) <= threshold) {
                union_find.Union(i, j);
            }
        }
    }

    // List all non empty subsets indexed by root node.
    std::vector<unsigned int> connected_comp_roots;
    for (unsigned int i = 0; i < hashes.size(); i++) {
        if ((union_find.Find(i) == i) && (union_find.Size(i) > 1)) {
            connected_comp_roots.push_back(i);
        }
    }
    std::sort(connected_comp_roots.begin(), connected_comp_roots.end());

    // Add the roots to the connected components
    for (const unsigned int &connected_comp_root : connected_comp_roots) {
        // Add a QVector of size 1 whose unique value is i.
        duplicates.append(QVector<int>(1, connected_comp_root));
    }

    // Add every nodes to their connected components.
    for (unsigned int i = 0; i < hashes.size(); i++) {
        unsigned int root = union_find.Find(i);

        // If the node is not a root.
        if (root != i) {
            // Search for his connected component.
            std::vector<unsigned int>::iterator it;
            it = std::lower_bound(connected_comp_roots.begin(), connected_comp_roots.end(), root);
            if (it != connected_comp_roots.end()) {
                int connected_comp = std::distance(connected_comp_roots.begin(), it);
                duplicates[connected_comp].append(i);
            }
        }
    }

    return duplicates;
}
