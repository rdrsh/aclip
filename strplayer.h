#ifndef STRPLAYER_H
#define STRPLAYER_H

#include <QWidget>
#include <QMediaPlayer>

//#include <QAbstractButton>
//#include <QSlider>
//#include <QLabel>
//#include <QUrl>
//#include <QTableView>

class QTableView;
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;

class StrPlayer : public QWidget
{
    Q_OBJECT
    QStringList strList_;
    QMediaPlayer mediaPlayer;
    QTableView *pTbl_;
    QAbstractButton *playButton;
    QSlider *positionSlider;
    QLabel *errorLabel;

public:
    explicit StrPlayer(QWidget *parent = nullptr);

    void playStr(const QString &s);
    QString wordToPath(const QString &s);

signals:

public slots:
    void addStr(const QString &s);
};

#endif // STRPLAYER_H
