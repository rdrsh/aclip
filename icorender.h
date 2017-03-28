#pragma once

#include <QPainter>
#include <QIcon>

class IcoRender {
    QSize size_;
    QFont font_;
    QTextOption textOpts;

public:
    IcoRender() : size_(64, 64), font_("Arial", 40), textOpts(Qt::AlignCenter | Qt::AlignVCenter) {}

    QIcon text(const QString &s) {
        QPixmap pix(size_);
        pix.fill();
        QPainter painter(&pix);
        painter.setFont(font_);
        painter.drawText(pix.rect(), s,  textOpts);
        return QIcon(pix);
    }
};
