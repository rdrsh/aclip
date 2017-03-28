#pragma once

#include <QCoreApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include "icorender.h"

class TrayMenu : QMenu {
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
    QTextEdit *pEd1_, *pEd2_;

public:
    void createTrayIcon() {
        pTrayMenu_ = new TrayMenu(this);

        IcoRender icoRend;
        QIcon icon(icoRend.text("X"));
        pTrayIcon_ = new QSystemTrayIcon(icon, this);
        pTrayIcon_->setContextMenu((QMenu*)pTrayMenu_);
        pTrayIcon_->show();
    }

    ClipWindow(QWidget *parent = 0) : QMainWindow(parent) {
        pTabs_ = new QTabWidget();
        setCentralWidget(pTabs_);
        pEd1_ = new QTextEdit("t1");
        pEd2_ = new QTextEdit("t2");
        pTabs_->addTab(pEd1_, "t1");
        pTabs_->addTab(pEd2_, "t2");
    }

    virtual ~ClipWindow() {
    }
};
