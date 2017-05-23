#include "picturelabel.h"

PictureLabel::PictureLabel(QWidget *parent) :
    QLabel(parent)
{

}

void PictureLabel::setPixmap(const QPixmap& picture)
{
    m_originalPixmap = picture;

    UpdateDisplayedPixmap();
}

void PictureLabel::resizeEvent(QResizeEvent*)
{
    UpdateDisplayedPixmap();
}

void PictureLabel::UpdateDisplayedPixmap()
{
    if (!m_originalPixmap.isNull())
    {
        int w = std::min(m_originalPixmap.width(), width());
        int h = std::min(m_originalPixmap.height(), height());

        QLabel::setPixmap(m_originalPixmap.scaled(QSize(w, h), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
