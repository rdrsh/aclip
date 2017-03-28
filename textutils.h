#pragma once

#include <QMimeData>
#include "htmlchunks.h"

class PluginTextLen {
protected:
    HtmlChunks *pHtmlChunks_;
    int maxTextLen_;

public:
    PluginTextLen(HtmlChunks *pHtmlChunks, int maxTextLen) : pHtmlChunks_(pHtmlChunks), maxTextLen_(maxTextLen) {
    }

    bool addMimeData(const QMimeData *pMimeData) {
        QString s = pMimeData->text();
        if (s.size() == 0 || s.size() > maxTextLen_) {
            return false;
        }
        return addText(s);
    }

    virtual bool addText(const QString &s) {
        pHtmlChunks_->addText(s);
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
        return QString("%1<br>\n%2").arg(currencyHtml(n)).arg(feetHtml(n));
//        .arg(intToColoredHtml(n))

    }

    static QString zeroPad3(int n) {
        if (n < 10) return QString("00%1").arg(n);
        if (n < 100) return QString("0%1").arg(n);
        return QString::number(n);
    }

    static QString intToColoredHtml(int n) {
//        return QString::number(n);
        QString html;
        for (int i = 0; n; ++i) {
            int d = n % 1000;
            n = n / 1000;
            QString color = colorSeq(i);
            html = QString("<span style=\"color:%1\">%2</span>").arg(color).arg(zeroPad3(d)) + html;
        }
        return html;
    }

    static QString colorSeq(int no) {
        static QStringList colors{"red", "green", "blue"};
        return colors.at(no % colors.size());
    }

    static QString currencyHtml(int n) {
        QString byd = intToColoredHtml(n);
        QString usd = intToColoredHtml((double)n/1.871);
        QString rub = intToColoredHtml((double)n/3.28);
        QString belRusUsd = QString("%1 byd = %2 usd = %3 rub").arg(byd).arg(usd).arg(rub);
        return belRusUsd;
    }

    static QString feetHtml(int n) {
        QString feet = intToColoredHtml(n);
        QString meters = intToColoredHtml((1./0.3048)*n);
        QString belRusUsd = QString("%1 feet = %2 m").arg(feet).arg(meters);
        return belRusUsd;
    }
};
