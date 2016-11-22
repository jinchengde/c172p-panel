#ifndef ALTITUDEINDICATOR_H
#define ALTITUDEINDICATOR_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QMutex>
#include "fgdata.h"
#include "knob.h"

#define NEEDLE_10000_POINTS 5
#define NEEDLE_1000_POINTS  7
#define NEEDLE_100_POINTS   5

class AltitudeIndicator : public QWidget, public FGDataConsumer, public FGDataUpdater
{
	Q_OBJECT
public:
	explicit AltitudeIndicator(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

public slots:
    void update_inhg(Knob::direcrion);

private:
    double angle_inhg();
    double angle_10000();
    double angle_1000();
    double angle_100();
    void load_img();

    double alt_inhg;
    double alt_ft;
    Knob k_inhg;

    static QMutex img_lock;
    static bool loaded;
    static const QImage * const img_inhg;
    static const QImage * const img_plate;
    static const QImage * const img_frame;

    static const QPoint needle10000[NEEDLE_10000_POINTS];
    static const QPoint needle1000[NEEDLE_1000_POINTS];
    static const QPoint needle100[NEEDLE_100_POINTS];
};

#endif // ALTITUDEINDICATOR_H
