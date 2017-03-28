#pragma once

#include <QDesktopWidget>
#include <QShortcut>
#include <QApplication>

class WidgetUt
{
    QWidget& widg;

public:

    WidgetUt(QWidget& w) : widg(w) 	{
    }

    ~WidgetUt() {
    }

    WidgetUt& show() {
        widg.show();
        return *this;
    }

    WidgetUt& minSize(int w, int h) {
        widg.setMinimumSize(w, h);
        return *this;
    }

    WidgetUt& resize(int w, int h) {
        widg.resize(w, h);
        return *this;
    }

    WidgetUt& resizePerc(double wPerc = 0.8, double hPerc = 0.8) {
        const QRect geom = QApplication::desktop()->availableGeometry(&widg);
        return resize(wPerc * geom.width(), hPerc * geom.height());
    }

    WidgetUt& center() {
        widg.move(QApplication::desktop()->screen()->rect().center() - widg.rect().center());
        return *this;
    }

    WidgetUt& bottomRight(QPoint delta = QPoint(20, 100)) {
        widg.move(QApplication::desktop()->screen()->rect().bottomRight() - widg.rect().bottomRight() - delta);
        return *this;
    }

    WidgetUt& top() {
        widg.setWindowFlags(widg.windowFlags() | Qt::WindowStaysOnTopHint);
        return *this;
    }

    WidgetUt& esc() {
        QShortcut *sh1 = new QShortcut(Qt::Key_Escape, &widg);
        QObject::connect(sh1, SIGNAL(activated()), qApp, SLOT(quit()));
        QShortcut *sh2 = new QShortcut(Qt::SHIFT + Qt::Key_Escape, &widg);
        QObject::connect(sh2, SIGNAL(activated()), qApp, SLOT(quit()));
        return *this;
    }

    WidgetUt& title(const QString &s) {
        widg.setWindowTitle(s);
        return *this;
    }

};
