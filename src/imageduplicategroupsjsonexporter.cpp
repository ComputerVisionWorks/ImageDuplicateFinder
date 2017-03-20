#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "imageduplicategroupsjsonexporter.h"

ImageDuplicateGroupsJsonExporter::ImageDuplicateGroupsJsonExporter(const ImageDuplicateGroups *duplicateGroups) :
    m_duplicateGroups(duplicateGroups)
{

}

QJsonObject ImageDuplicateGroupsJsonExporter::toJson() const {
    QJsonObject jsonDuplicates;

    QJsonArray directoriesArray;
    for (QStringList::const_iterator itDir = m_duplicateGroups->directories.begin(); itDir != m_duplicateGroups->directories.end(); ++itDir) {
        directoriesArray.append(*itDir);
    }

    QJsonArray duplicateGroupsArray;
    for (int i = 0; i < m_duplicateGroups->size(); i++) {
        QJsonArray groupArray;

        for (int j = 0; j < m_duplicateGroups->groupSize(i); j++) {
            groupArray.append(m_duplicateGroups->groupImage(i, j));
        }

        duplicateGroupsArray.append(groupArray);
    }

    jsonDuplicates["directories"] = directoriesArray;
    jsonDuplicates["includeSubDirectories"] = m_duplicateGroups->includeSubDirectories;
    jsonDuplicates["duplicates"] = duplicateGroupsArray;

    return jsonDuplicates;
}

bool ImageDuplicateGroupsJsonExporter::save(const QString& filePath) const {
    QJsonDocument document(toJson());

    document.toJson();

    QFile saveFile(filePath);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    if (saveFile.write(document.toJson()) < 0) {
        qWarning("Couldn't write save file.");
        return false;
    }

    return true;
}

