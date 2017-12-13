#pragma once

#include <QCoreApplication>
#include <QGuiApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include <QClipboard>
#include <QMessageBox>
#include "icorender.h"
#include "htmlchunks.h"
//#include "strplayer.h"
#include "pluguns.h"
#include "widg_vplayer.h"

class TrayMenu : public QMenu {
    QMainWindow *mainWin_;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

public:
    TrayMenu(QMainWindow *mainWin) : QMenu(mainWin), mainWin_(mainWin) {
        populate();
    }

    void populate() {
        minimizeAction = new QAction(tr("Mi&nimize"), this);
        connect(minimizeAction, &QAction::triggered, mainWin_, &QWidget::hide);

        maximizeAction = new QAction(tr("Ma&ximize"), this);
        connect(maximizeAction, &QAction::triggered, mainWin_, &QWidget::showMaximized);

        restoreAction = new QAction(tr("&Restore"), this);
        connect(restoreAction, &QAction::triggered, mainWin_, &QWidget::showNormal);

        quitAction = new QAction(tr("&Quit"), this);
        connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

        addAction(minimizeAction);
        addAction(maximizeAction);
        addAction(restoreAction);
        addSeparator();
        addAction(quitAction);
    }
};

class ClipWindow : public QMainWindow {
    Q_OBJECT

private:
    QSystemTrayIcon *pTrayIcon_;
    TrayMenu *pTrayMenu_;
    QTabWidget *pTabs_;
    HtmlChunks *pEd1_, *pEd2_, *pEd3_;
    WidgVPlayer *pPlay_;
    QClipboard *pClip_;

public:
    ClipWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        pTabs_ = new QTabWidget();
        setCentralWidget(pTabs_);
        pEd1_ = new HtmlChunks(10);
        pEd2_ = new HtmlChunks(10);
        pEd3_ = new HtmlChunks(10);
        pPlay_ = new WidgVPlayer();

        pTabs_->addTab(pPlay_, "voice");
        pTabs_->addTab(pEd1_, "txt");
        pTabs_->addTab(pEd2_, "num");
        pTabs_->addTab(pEd3_, "qwe");

        pClip_ = QGuiApplication::clipboard();
        connect(pClip_, &QClipboard::dataChanged, this, &ClipWindow::onClip);
    }

    virtual ~ClipWindow() {
    }

    void switchVisible() {
        setVisible(!isVisible());
    }

    void createTrayIcon() {
        pTrayMenu_ = new TrayMenu(this);
        pTrayIcon_ = new QSystemTrayIcon(this);
        pTrayIcon_->setContextMenu((QMenu*)pTrayMenu_);
        trayIconByMimeData(pClip_->mimeData());
        pTrayIcon_->show();
        connect(pTrayIcon_, &QSystemTrayIcon::messageClicked, this, &ClipWindow::messageClicked);
        connect(pTrayIcon_, &QSystemTrayIcon::activated, this, &ClipWindow::iconActivated);
    }

    void trayIconText(const QString &s) {
        IcoRender icoRend;
        QIcon icon(icoRend.text(s));
        pTrayIcon_->setIcon(icon);
    }

    static QString mimeDataToStr(const QMimeData *pMimeData) {
        if (pMimeData->hasColor()) return "C";
        if (pMimeData->hasUrls()) return "U";
        if (pMimeData->hasImage()) return "I";
        if (pMimeData->hasHtml()) return "H";
        if (pMimeData->hasText()) return "T";
        return "0";
    }
    void trayIconByMimeData(const QMimeData *pMimeData) {
        trayIconText(mimeDataToStr(pMimeData));
    }

    void iconActivated(QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            switchVisible();
            break;
        case QSystemTrayIcon::MiddleClick:
            qApp->quit();
        default:
            ;
        }
    }

    void messageClicked() {
        QMessageBox::information(0, tr("Systray"),
                                 tr("Sorry, I already gave what help I could.\n"
                                    "Maybe you should try asking a human?"));
    }

    void addMime(const QMimeData *pMimeData) {
        trayIconByMimeData(pMimeData);
        PluginTextLen p(pEd1_, 1000);
        p.addMimeData(pMimeData);
        PluginInt p2(pEd2_);
        p2.addMimeData(pMimeData);
        PluginQwerty p3(pEd3_);
        p3.addMimeData(pMimeData);
        PluginVoice p4(pPlay_);
        p4.addMimeData(pMimeData);
    }

public slots:
    void onClip() { addMime(pClip_->mimeData()); }
};
