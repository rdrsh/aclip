#pragma once

#include <algorithm>
#include <QTextEdit>
#include <QShortcut>

class HtmlChunks : public QTextEdit {
//    Q_OBJECT

private:
    int lenLimit_;
    bool ascending_;
    QStringList lst_;

public:
    HtmlChunks(int lenLimit, bool ascending = true, QWidget *parent = nullptr) : QTextEdit(parent), lenLimit_(lenLimit), ascending_(ascending) {
        setReadOnly(true);
        QShortcut *sh1 = new QShortcut(Qt::Key_F3, this);
        connect(sh1, &QShortcut::activated, this, &HtmlChunks::switchOrder);
    }

    void addHtml(const QString &html) {
        if (lst_.size() >= lenLimit_) {
            lst_.removeFirst();
        }
        lst_.push_back(html);
        update();
    }

    void addText(const QString &text) {
        addHtml(text.toHtmlEscaped());
    }

    void update() {
        if (lst_.empty()) {
            setHtml("");
            return;
        }

        QString html;
        QString sep;
        QString hr("\n<hr>\n");

        int i = 0, i2 = lst_.size()-1, delta = 1;
        if (!ascending_) {
            std::swap(i, i2);
            delta = -1;
        }

        while (true) {
            html += sep;
            html += lst_.at(i);
            sep = hr;
            if (i == i2) {
                break;
            }
            i += delta;
        }
        setHtml(html);
    }

public slots:
    void switchOrder() {
        ascending_ = !ascending_;
        update();
    }
};
