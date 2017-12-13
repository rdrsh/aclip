#include "widg_vplayer.h"

#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>

WidgVPlayer::WidgVPlayer(QWidget *parent)
    : QWidget(parent)
    , mediaPlayer(0, QMediaPlayer::VideoSurface)
    , playButton(0)
    , positionSlider(0)
    , errorLabel(0)
{
    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open..."));
    connect(openButton, &QAbstractButton::clicked, this, &WidgVPlayer::openFile);

    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, &QAbstractButton::clicked,
            this, &WidgVPlayer::play);

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, &QAbstractSlider::sliderMoved,
            this, &WidgVPlayer::setPosition);

    errorLabel = new QLabel;
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);
    layout->addWidget(errorLabel);

    setLayout(layout);

    mediaPlayer.setVideoOutput(videoWidget);
    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &WidgVPlayer::mediaStateChanged);
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &WidgVPlayer::positionChanged);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &WidgVPlayer::durationChanged);
    typedef void (QMediaPlayer::*ErrorSignal)(QMediaPlayer::Error);
    connect(&mediaPlayer, static_cast<ErrorSignal>(&QMediaPlayer::error),
            this, &WidgVPlayer::handleError);
}

WidgVPlayer::~WidgVPlayer()
{
}

void WidgVPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    QStringList supportedMimeTypes = mediaPlayer.supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty())
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        setUrl(fileDialog.selectedUrls().constFirst());
}

void WidgVPlayer::setUrl(const QUrl &url)
{
    errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mediaPlayer.setMedia(url);
    playButton->setEnabled(true);
}

void WidgVPlayer::play()
{
    switch(mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}

void WidgVPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void WidgVPlayer::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void WidgVPlayer::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void WidgVPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void WidgVPlayer::handleError()
{
    playButton->setEnabled(false);
    const QString errorString = mediaPlayer.errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(mediaPlayer.error()));
    else
        message += errorString;
    errorLabel->setText(message);
}
