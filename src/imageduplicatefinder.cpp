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
    // When the ListImages task finishes, call the ListImagesEnd slot.
    QObject::connect(&m_imagesFutureWatcher, &QFutureWatcher<QStringList>::finished, this, &ImageDuplicateFinder::ListImagesEnd);
    // When the ComputeHashImages task finishes, call the ComputeHashImagesEnd slot.
    QObject::connect(&m_hashFutureWatcher, &QFutureWatcher<quint64>::finished, this, &ImageDuplicateFinder::ComputeHashImagesEnd);
    // When the ComputeHashImages task reports progress, the signals are passed through.
    QObject::connect(&m_hashFutureWatcher, &QFutureWatcher<quint64>::progressRangeChanged, this, &ImageDuplicateFinder::progressRangeChanged);
    QObject::connect(&m_hashFutureWatcher, &QFutureWatcher<quint64>::progressValueChanged, this, &ImageDuplicateFinder::progressValueChanged);
    // When the FindDuplicates task finishes, call the FindDuplicatesEnd slot.
    QObject::connect(&m_duplicatesFutureWatcher, &QFutureWatcher<QList<QVector<int> > >::finished, this, &ImageDuplicateFinder::FindDuplicatesEnd);
}

ImageDuplicateFinder::~ImageDuplicateFinder() {
}

// Start the computation by listing all images
void ImageDuplicateFinder::start(const QStringList& directories, bool exploreSubDirectories) {
    ListImages(directories, exploreSubDirectories);
}

void ImageDuplicateFinder::cancel() {
    m_imagesFutureWatcher.cancel();
    m_hashFutureWatcher.cancel();
    m_duplicatesFutureWatcher.cancel();
    reset();
}

void ImageDuplicateFinder::reset() {
    m_duplicateGroups.clear();
    m_hashes.clear();
}

void ImageDuplicateFinder::ListImages(const QStringList &directories, bool exploreSubDirectories) {
    emit progressTextChanged("Listing images to analyse");
    emit progressRangeChanged(0, 0);
    emit progressValueChanged(0);

    m_duplicateGroups.directories = directories;

    m_imagesFutureWatcher.setFuture(QtConcurrent::run(this, &ImageDuplicateFinder::ListImagesInDirectory, directories, exploreSubDirectories));
}

void ImageDuplicateFinder::ListImagesEnd() {
    m_duplicateGroups.images = m_imagesFutureWatcher.result();

    ComputeHashImages();
}

void ImageDuplicateFinder::ComputeHashImages() {
    emit progressTextChanged("Analysing images");

    m_hashFutureWatcher.setFuture(QtConcurrent::mapped(m_duplicateGroups.images, DctPerceptualHash));
}

void ImageDuplicateFinder::ComputeHashImagesEnd() {
    m_hashes = m_hashFutureWatcher.future().results().toVector();

    FindDuplicates();
}

void ImageDuplicateFinder::FindDuplicates() {
    emit progressTextChanged("Finding duplicates");
    emit progressRangeChanged(0, 0);
    emit progressValueChanged(0);

    m_duplicatesFutureWatcher.setFuture(QtConcurrent::run(this, &ImageDuplicateFinder::FindDuplicatesInHashes));
}

void ImageDuplicateFinder::FindDuplicatesEnd() {
    m_duplicateGroups.duplicateGroups = m_duplicatesFutureWatcher.future().result();

    emit finished();
}

QStringList ImageDuplicateFinder::ListImagesInDirectory(const QStringList& directories, bool exploreSubDirectories) const {
    QSet<QString> imagesSet;

    QDirIterator::IteratorFlags iteratorFlags = QDirIterator::NoIteratorFlags;
    if (exploreSubDirectories) {
        iteratorFlags = QDirIterator::Subdirectories;
    }

    // List all files in all directories.
    for (QStringList::const_iterator itDirPath = directories.begin(); itDirPath != directories.end(); ++itDirPath) {
        QDirIterator itDir(*itDirPath, IMG_FORMATS, QDir::Files, iteratorFlags);
        while (itDir.hasNext()) {
            imagesSet.insert(itDir.next());
        }
    }

    return imagesSet.toList();
}

QList<QVector<int> > ImageDuplicateFinder::FindDuplicatesInHashes() const {
    QList<QVector<int> > duplicates;

    UnionFind union_find(m_hashes.size());

    for (int i = 0; i < m_hashes.size(); i++) {
        for (int j = i + 1; j < m_hashes.size(); j++) {
            if (DctPerceptualHashDistance(m_hashes[i], m_hashes[j]) <= m_threshold) {
                union_find.Union(i, j);
            }
        }
    }

    // List all non empty subsets indexed by root node.
    std::vector<int> connected_comp_roots;
    for (int i = 0; i < m_hashes.size(); i++) {
        if ((union_find.Find(i) == i) && (union_find.Size(i) > 1)) {
            connected_comp_roots.push_back(i);
        }
    }
    std::sort(connected_comp_roots.begin(), connected_comp_roots.end());

    // Add the roots to the connected components
    for (unsigned int i = 0; i < connected_comp_roots.size(); i++) {
        // Add a QVector of size 1 whose unique value is i.
        duplicates.append(QVector<int>(1, connected_comp_roots[i]));
    }

    // Add every nodes to their connected components.
    for (int i = 0; i < m_hashes.size(); i++) {
        int root = union_find.Find(i);

        // If the node is not a root.
        if (root != i) {
            // Search for his connected component.
            std::vector<int>::iterator it;
            it = std::lower_bound(connected_comp_roots.begin(), connected_comp_roots.end(), root);
            if (it != connected_comp_roots.end()) {
                int connected_comp = std::distance(connected_comp_roots.begin(), it);
                duplicates[connected_comp].append(i);
            }
        }
    }

    return duplicates;
}
