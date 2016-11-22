#include <QPainter>
#include <QMutexLocker>
#include "rpm.h"
#include "logutil.h"
#include "config.h"
#include "imgres.h"

RPM::RPM(QWidget *parent)
    : QWidget(parent),
      rpm(0.0),
      running(false)
{
    load_img();
}

void RPM::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    rpm = get_data_by_key(FGDATA_ENGINE_RPM, rpm);
    running = get_data_by_key(FGDATA_ENGINE_RUNNING, running);
    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw plate and frame
    painter.drawImage(-w / 2, -h / 2, *img_plate);

    // draw needle
    painter.rotate(rpm_to_angle());
    painter.drawConvexPolygon(needle, 4);
}

double RPM::rpm_to_angle()
{
    if ((!running) || (rpm < 300)) {
        return 151;
    }

    return (rpm - 300) / 100 * 7.33 + 151;
}

void RPM::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_plate);
    *p = QImage(Config::img_prefix + IMG_RPM_PLATE);
    if (p->isNull()) {
        LOGE("Failed to load rpm plate");
    }

    loaded = true;
}

const QPoint RPM::needle[4] = {QPoint(-3, 0),
                                             QPoint(0, 3),
                                             QPoint(55, 0),
                                             QPoint(0, -3)};
QMutex RPM::img_lock;
bool RPM::loaded = false;
const QImage * const RPM::img_plate = new QImage();
