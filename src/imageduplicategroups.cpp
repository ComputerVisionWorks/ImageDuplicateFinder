#include <algorithm>

#include "imageduplicategroups.h"

ImageDuplicateGroups::ImageDuplicateGroups()
{

}

void ImageDuplicateGroups::clear() {
    directories.clear();
    includeSubDirectories = false;
    images.clear();
    duplicateGroups.clear();
}

int ImageDuplicateGroups::size() const {
    return duplicateGroups.size();
}

QStringList ImageDuplicateGroups::group(int group) const {
    QStringList extractedGroup;

    for (int i = 0; i < duplicateGroups[group].size(); i++) {
        extractedGroup.append(images[duplicateGroups[group][i]]);
    }

    return extractedGroup;
}

int ImageDuplicateGroups::groupSize(int group) const {
    return duplicateGroups[group].size();
}

const QString& ImageDuplicateGroups::groupImage(int group, int image) const {
    return images[duplicateGroups[group][image]];
}

int ImageDuplicateGroups::findGroup(const QString& image) const {

    // Search for the image number.
    QStringList::const_iterator itImage = std::find(images.cbegin(), images.cend(), image);

    if (itImage != images.end()) {
        int imageNumber = std::distance(images.cbegin(), itImage);

        // Search the other images in the group.
        for (int i = 0; i < duplicateGroups.size(); i++) {
            for (int j = 0; j < duplicateGroups[i].size(); j++) {
                if (imageNumber == duplicateGroups[i][j]) {
                    return i;
                }
            }
        }
    }

    return 0;
}
