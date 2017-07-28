#pragma once

#include <algorithm>
#include <QTextEdit>
#include <QShortcut>

inline QString styleTag(const QString &css) {
    return QString("<style type='text/css'>\n%1\n</style>\n").arg(css.toHtmlEscaped());
}

inline QString preTag(const QString &text) {
    return QString("<pre>%1</pre>\n").arg(text.toHtmlEscaped());
}

class HtmlChunks : public QTextEdit {
//    Q_OBJECT

private:
    int lenLimit_;
    bool ascending_;
    QString separator_;
    QString css_;
    QStringList lst_;

public:
    HtmlChunks(int lenLimit, bool ascending = true, QWidget *parent = nullptr) : QTextEdit(parent), lenLimit_(lenLimit), ascending_(ascending) {
        setSeparator("\n");
        setCss("pre { background:#efefef; }");
        setReadOnly(true);
        QShortcut *sh1 = new QShortcut(Qt::Key_F3, this);
        connect(sh1, &QShortcut::activated, this, &HtmlChunks::switchOrder);
    }

    void setSeparator(const QString &separator) { separator_ = separator; }
    void setCss(const QString &css) { css_ = css; }

    void addHtml(const QString &html) {
        if (lst_.size() >= lenLimit_) lst_.removeFirst();
        lst_.push_back(html);
        update();
    }

    void addText(const QString &text) { addHtml(preTag(text)); }

    void delHtml(const QString &html) { lst_.removeAll(html); }
    void delText(const QString &text) { delHtml(preTag(text)); }

    void update() {
        QString html = styleTag(css_);
        if (lst_.isEmpty()) {
            setHtml(html);
            return;
        }

        int i = 0, i2 = lst_.size()-1, delta = 1;
        if (!ascending_) {
            std::swap(i, i2);
            delta = -1;
        }

        QString sep;
        while (true) {
            html += sep;
            html += lst_.at(i);
            sep = separator_;
            if (i == i2) {
                break;
            }
            i += delta;
        }
        setHtml(html);
//        setPlainText(html);
    }

public slots:
    void switchOrder() {
        ascending_ = !ascending_;
        if (lst_.size() > 1) update();
    }
};
