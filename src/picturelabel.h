#ifndef PICTURELABEL_H
#define PICTURELABEL_H

#include <QLabel>
#include <QPixmap>

class PictureLabel : public QLabel
{
public:
    PictureLabel(QWidget *parent);
    void setPixmap(const QPixmap& picture);
    void resizeEvent(QResizeEvent*);

private:
    void UpdateDisplayedPixmap();

    QPixmap m_originalPixmap;
};

#endif // PICTURELABEL_H
