#include <QPainter>
#include <QMutexLocker>
#include "altitudeindicator.h"
#include "imgres.h"
#include "logutil.h"
#include "config.h"

AltitudeIndicator::AltitudeIndicator(QWidget *parent)
    : QWidget(parent),
      FGDataConsumer(),
      FGDataUpdater(),
      alt_inhg(29.98),
      alt_ft(0.0),
      k_inhg(this, IMG_K_ALT_INHG)
{
    load_img();

    k_inhg.setGeometry(10, 240, 50, 50);
    QObject::connect(&k_inhg, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_inhg(Knob::direcrion)));
}

void AltitudeIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    alt_inhg = get_data_by_key(FGDATA_ALT_INHG, alt_inhg);
    alt_ft = get_data_by_key(FGDATA_ALT_FT, alt_ft);

    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw inhg
    painter.save();
    painter.rotate(angle_inhg());
    painter.drawImage(-w / 2, -h / 2, *img_inhg);
    painter.restore();

    // draw plate and frame
    painter.drawImage(-w / 2, -h / 2, *img_plate);
    painter.drawImage(-w / 2, -h / 2, *img_frame);

    // draw needle for 10000-feet
    painter.save();
    painter.rotate(angle_10000());
    painter.drawConvexPolygon(needle10000, NEEDLE_10000_POINTS);
    painter.restore();

    // draw needle for 1000-feet
    painter.save();
    painter.rotate(angle_1000());
    painter.drawConvexPolygon(needle1000, NEEDLE_1000_POINTS);
    painter.restore();

    // draw needle for 100-feet
    painter.save();
    painter.rotate(angle_100());
    painter.drawConvexPolygon(needle100, NEEDLE_100_POINTS);
    painter.restore();
}

void AltitudeIndicator::update_inhg(Knob::direcrion d)
{
    alt_inhg = get_data_by_key(FGDATA_ALT_INHG, alt_inhg);

    if (d == Knob::LEFT) {
        set_data_by_key(FGDATA_ALT_INHG, alt_inhg - 0.01);
    } else if (d == Knob::RIGHT) {
        set_data_by_key(FGDATA_ALT_INHG, alt_inhg + 0.01);
    }
}

double AltitudeIndicator::angle_inhg()
{
    return (30.0 - alt_inhg) * 100;
}

double AltitudeIndicator::angle_10000()
{
    double a = alt_ft / 100000;
    int b = a; //the integer part of a

    return (a - b) * 360 - 90;
}

double AltitudeIndicator::angle_1000()
{
    double a = alt_ft / 10000;
    int b = a; //the integer part of a

    return (a - b) * 360 - 90;
}

double AltitudeIndicator::angle_100()
{
    double a = alt_ft / 1000;
    int b = a; //the integer part of a

    return (a - b) * 360 - 90;
}

void AltitudeIndicator::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_inhg);
    *p = QImage(Config::img_prefix + IMG_ALT_INHG);
    if (p->isNull()) {
        LOGE("Failed to load alt inhg");
    }

    p = const_cast<QImage *>(img_frame);
    *p = QImage(Config::img_prefix + IMG_ALT_FRAME);
    if (p->isNull()) {
        LOGE("Failed to load alt frame");
    }

    p = const_cast<QImage *>(img_plate);
    *p = QImage(Config::img_prefix + IMG_ALT_PLATE);
    if (p->isNull()) {
        LOGE("Failed to load alt plate");
    }

    loaded = true;
}

QMutex AltitudeIndicator::img_lock;
bool AltitudeIndicator::loaded = false;
const QImage * const AltitudeIndicator::img_inhg = new QImage();
const QImage * const AltitudeIndicator::img_plate = new QImage();
const QImage * const AltitudeIndicator::img_frame = new QImage();
const QPoint AltitudeIndicator::needle10000[NEEDLE_10000_POINTS] = {
    QPoint(-3, -3),
    QPoint(28, -10),
    QPoint(44, 0),
    QPoint(28, 10),
    QPoint(-3, 3)
};

const QPoint AltitudeIndicator::needle1000[NEEDLE_1000_POINTS] = {
    QPoint(-20, -8),
    QPoint(0, -5),
    QPoint(60, -5),
    QPoint(65, 0),
    QPoint(60, 5),
    QPoint(0, 5),
    QPoint(-20, 8)
};

const QPoint AltitudeIndicator::needle100[NEEDLE_100_POINTS] = {
    QPoint(-30, -3),
    QPoint(90, -3),
    QPoint(100, 0),
    QPoint(90, 3),
    QPoint(-30, 3)
};
