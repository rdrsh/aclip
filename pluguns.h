#pragma once

#include <QMimeData>
#include "htmlchunks.h"
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
