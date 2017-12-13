#pragma once

#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include "htmlchunks.h"
#include "widg_vplayer.h"
#include "textutils.h"

class PluginTextLen {
protected:
    HtmlChunks *pHtmlChunks_;
    int maxTextLen_;

public:
    PluginTextLen(HtmlChunks *pHtmlChunks, int maxTextLen) : pHtmlChunks_(pHtmlChunks), maxTextLen_(maxTextLen) {}

    bool addMimeData(const QMimeData *pMimeData) {
        QString s = pMimeData->text();
        if (s.size() == 0 || s.size() > maxTextLen_) {
            return false;
        }
        return addText(s);
    }

    virtual bool addText(const QString &s) {
        pHtmlChunks_->delText(s);
        pHtmlChunks_->addText(s);
        return true;
    }
};

class PluginQwerty : public PluginTextLen {
public:
    PluginQwerty(HtmlChunks *pHtmlChunks) : PluginTextLen(pHtmlChunks, 32) {}

    bool addText(const QString &s) override {
//        if (!TxtUt::isSingleLine(s)) return false;
        QString qwe = TxtUt::qwerty(s);
        if (s == qwe) return false;
        pHtmlChunks_->addText(qwe);
        return true;
    }

};

class PluginInt : public PluginTextLen {
public:
    PluginInt(HtmlChunks *pHtmlChunks) : PluginTextLen(pHtmlChunks, 32) {
    }

    bool addText(const QString &s) override {
        QString s2 = s.simplified().replace(' ', "");
        bool ok;
        int n = s2.toInt(&ok);
        return ok ? addInt(n) : false;
    }

    bool addInt(int n) {
        pHtmlChunks_->addHtml(allHtml(n));
        return true;
    }

    static QString allHtml(int n) {
//        .arg(intToColoredHtml(n))
        QString curr = TxtUt::currencyHtml(n);
        QString ft = TxtUt::feetHtml(n);
        return QString("%1<br>\n%2").arg(curr).arg(ft);
    }
};

class PluginVoice : public PluginTextLen {
    WidgVPlayer *pPlay_;

public:
    PluginVoice(WidgVPlayer *pPlay) : PluginTextLen(nullptr, 32), pPlay_(pPlay) {
    }

    bool addText(const QString &s) override {
        return addWord(s.simplified().toLower());
    }

    bool addWord(const QString &s) {
        if (s.isEmpty()) return false;
//        QString tmpl("~/db/mp3-en/%1/%2.mp3");
//        QString tmpl("/Volumes/db.zim/dicts/Sound_En_mp3_128_mono/%1/%2.mp3");
        QString tmpl("/Users/Apple/mp3_en/%1/%2.mp3");
        QString fileName = tmpl.arg(s.at(0)).arg(s);
        QFileInfo fi(fileName);
        if (fi.isFile()) {
//            qDebug() << "Found: " << fileName;
            auto url = QString("file://")+fileName;
            pPlay_->setUrl(url);
            pPlay_->play();
        }
        else {
            qDebug() << "Not found: " << fileName;
        }
        return true;
    }
};
