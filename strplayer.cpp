#include "strplayer.h"

#include <QStringListModel>
#include <QTableView>
#include <QVBoxLayout>

// from docs
//#include <QtWidgets>
//#include <qvideowidget.h>
//#include <qvideosurfaceformat.h>

#include <QtWidgets>
//#include <QVideoWidget>
#include <qvideosurfaceformat.h>


StrPlayer::StrPlayer(QWidget *parent)
    : QWidget(parent)
    , strList_()
    , mediaPlayer(0, QMediaPlayer::VideoSurface)
    , pTbl_(0)
    , playButton(0)
    , positionSlider(0)
    , errorLabel(0)
{
    pTbl_ = new QTableView();
    strList_ << "word" << "python";
    auto pModel = new QStringListModel(strList_);
    pTbl_->setModel(pModel);

//    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open..."));

    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    errorLabel = new QLabel;
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    auto pLay = new QVBoxLayout(this);
    pLay->setMargin(0);
    pLay->addWidget(pTbl_);
    pLay->addLayout(controlLayout);

//    connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);
//    connect(playButton, &QAbstractButton::clicked, this, &VideoPlayer::play);
//    connect(positionSlider, &QAbstractSlider::sliderMoved, this, &VideoPlayer::setPosition);
    wordToPath("python");
}

void StrPlayer::playStr(const QString &s)
{

}

QString StrPlayer::wordToPath(const QString &s)
{
    if (s.isEmpty()) return s;
    QString tmpl("~/db/mp3-en/%1/%2.mp3");
    QString fileName = tmpl.arg(s.at(0)).arg(s);
    errorLabel->setText(fileName);
    return fileName;
}

void StrPlayer::addStr(const QString &s)
{
    strList_ << s;
}
