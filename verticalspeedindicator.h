#ifndef VERTICALSPEEDINDICATOR_H
#define VERTICALSPEEDINDICATOR_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QImage>
#include <QMutex>
#include "fgdata.h"

class VerticalSpeedIndicator : public QWidget, public FGDataConsumer
{
	Q_OBJECT
public:
	explicit VerticalSpeedIndicator(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

private:
    double vs_to_angle();
    void load_img();

    double vs_fpm;
    static const QPoint needle[4];

    static QMutex img_lock;
    static bool loaded;
    static const QImage * const img_plate;
    static const QImage * const img_frame;
};

#endif // VERTICALSPEEDINDICATOR_H
