#ifndef IMAGEDUPLICATEGROUPS_H
#define IMAGEDUPLICATEGROUPS_H

#include <QString>
#include <QList>
#include <QStringList>

class ImageDuplicateGroups
{
public:
    ImageDuplicateGroups();

    void clear();

    int size() const;

    QStringList group(int group) const;

    int groupSize(int group) const;

    const QString& groupImage(int group, int image) const;

    int findGroup(const QString& image) const;

    QStringList directories;

    bool includeSubDirectories;

    /**
     * @brief images List of all images in the directories.
     */
    QStringList images;

    /**
     * @brief duplicateGroups List of all duplicate groups. Duplicates groups
     */
    QList<QVector<int> > duplicateGroups;
};

#endif // IMAGEDUPLICATEGROUPS_H
