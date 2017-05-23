#ifndef IMAGEDUPLICATEGROUPSJSONEXPORTER_H
#define IMAGEDUPLICATEGROUPSJSONEXPORTER_H

#include <QString>
#include <QJsonObject>

#include "imageduplicategroups.h"

class ImageDuplicateGroupsJsonExporter
{
public:
    ImageDuplicateGroupsJsonExporter(const ImageDuplicateGroups* duplicateGroups);

    QJsonObject toJson() const;

    bool save(const QString& filePath) const;

private:
    const ImageDuplicateGroups* m_duplicateGroups;
};

#endif // IMAGEDUPLICATEGROUPSJSONEXPORTER_H
