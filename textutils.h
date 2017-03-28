#pragma once

class TxtUt {
public:
    static QString zeroPad3(int n) {
        if (n < 10) return QString("00%1").arg(n);
        if (n < 100) return QString("0%1").arg(n);
        return QString::number(n);
    }

    static QString intToColoredHtml(int n) {
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
        QString belRusUsd = QString("%1 ft = %2 m").arg(feet).arg(meters);
        return belRusUsd;
    }

#define S1 QString("qwertyuiop[] asdfghjkl;' zxcvbnm,./ ` QWERTYUIOP{} |ZXCVBNM<>? ~!@#$%^&*()_+ ASDFGHJKL:\"|")
#define S2 QString("йцукенгшщзхъ фывапролджэ ячсмитьбю. ё ЙЦУКЕНГШЩЗХЪ /ЯЧСМИТЬБЮ, Ё!\"№;%:?*()_+ ФЫВАПРОЛДЖЭ/")

    static QString qwerty(const QString &s) {
        static QString s1 = S1+S2;
        static QString s2 = S2+S1;
        return strTranslate(s, s1, s2);
    }

    static QString strTranslate(const QString &s, const QString &sFrom, const QString &sTo) {
        QString result;
        for (int i = 0, l = s.size(); i < l; ++i) {
            QChar ch = s[i];
            int j = sFrom.indexOf(ch);
//            if (j >= 0) s[i] = sTo[j];
            result += j >= 0 ? sTo[j] : ch;
        }
        return result;
    }

    static bool isSingleLine(const QString &s, int maxLen = 64) {
        return s.size() <= maxLen && s.indexOf("\n") == -1 && s.indexOf("\r") == -1;
    }
};
