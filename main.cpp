#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "icorender.h"
#include "clipwindow.h"
#include "widgut.h"

QString _(const char *s) {
    return QObject::tr(s);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, _("Systray"), _("I couldn't detect any system tray on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    ClipWindow w;
    WidgetUt(w).title("AClip").resizePerc(0.4, 0.7).bottomRight().top().esc();
    w.createTrayIcon();

//    QMimeData m;
//    m.setText("10000000");
//    w.addMime(&m);

    IcoRender icoRend;
    w.setWindowIcon(icoRend.text("AC"));

    return a.exec();
}
