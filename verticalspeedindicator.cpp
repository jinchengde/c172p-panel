#include <QPainter>
#include <QMutexLocker>
#include "verticalspeedindicator.h"
#include "imgres.h"
#include "logutil.h"
#include "config.h"

VerticalSpeedIndicator::VerticalSpeedIndicator(QWidget *parent)
    : QWidget(parent), FGDataConsumer(), vs_fpm(0.0)
{
    load_img();
}

void VerticalSpeedIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    vs_fpm = get_data_by_key(FGDATA_VSI_FPM, vs_fpm);
    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw plate and frame
    painter.drawImage(-w / 2, -h / 2, *img_plate);
    painter.drawImage(-w / 2, -h / 2, *img_frame);

    // draw needle
    painter.rotate(vs_to_angle());
    painter.drawConvexPolygon(needle, 4);
}

double VerticalSpeedIndicator::vs_to_angle()
{
    double vs_valid = vs_fpm;

    if (vs_valid < -1900) {
        vs_valid = -1900;
    } else if (vs_valid > 1900) {
        vs_valid = 1900;
    }

    return vs_valid * 0.09 + 180;
}

void VerticalSpeedIndicator::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_frame);
    *p = QImage(Config::img_prefix + IMG_VSI_FRAME);
    if (p->isNull()) {
        LOGE("Failed to load vsi frame");
    }

    p = const_cast<QImage *>(img_plate);
    *p = QImage(Config::img_prefix + IMG_VSI_PLATE);
    if (p->isNull()) {
        LOGE("Failed to load vsi plate");
    }

    loaded = true;
}

const QPoint VerticalSpeedIndicator::needle[4] = {QPoint(-5, 0),
                                             QPoint(0, 5),
                                             QPoint(85, 0),
                                             QPoint(0, -5)};
QMutex VerticalSpeedIndicator::img_lock;
bool VerticalSpeedIndicator::loaded = false;
const QImage * const VerticalSpeedIndicator::img_plate = new QImage();
const QImage * const VerticalSpeedIndicator::img_frame = new QImage();
