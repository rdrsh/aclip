#ifndef WIDG_VPLAYER_H
#define WIDG_VPLAYER_H

#include <qmediaplayer.h>

#include <QtGui/QMovie>
#include <QtWidgets/QWidget>

class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;

class WidgVPlayer : public QWidget
{
    Q_OBJECT
public:
    WidgVPlayer(QWidget *parent = 0);
    ~WidgVPlayer();

    void setUrl(const QUrl &url);

public slots:
    void openFile();
    void play();

private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();

private:
    QMediaPlayer mediaPlayer;
    QAbstractButton *playButton;
    QSlider *positionSlider;
    QLabel *errorLabel;
};

#endif // WIDG_VPLAYER_H
