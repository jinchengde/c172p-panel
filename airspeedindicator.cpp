#include <QPainter>
#include <QMutexLocker>
#include "airspeedindicator.h"
#include "imgres.h"
#include "logutil.h"
#include "config.h"

AirSpeedIndicator::AirSpeedIndicator(QWidget *parent)
    : QWidget(parent), FGDataConsumer(), asi_kt(0.0)
{
    load_img();
}

void AirSpeedIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    asi_kt = get_data_by_key(FGDATA_ASI_KT, asi_kt);
    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw plate and frame
    painter.drawImage(-w / 2, -h / 2, *img_plate);
    painter.drawImage(-w / 2, -h / 2, *img_frame);

    // draw needle
    painter.rotate(scale_angle() - 90);
    painter.drawConvexPolygon(needle, 4);
}

double AirSpeedIndicator::scale_angle()
{
    const int scale_as = 100;
    const int to_sub_script = scale_as * 10;
    const int milli_angle = scale_as * 10;
    const int max_as = 190 * scale_as;
    int asi_kt_i = asi_kt * scale_as;
    double l, u;

    if (asi_kt_i < 0) {
        asi_kt_i = 0;
    }
    if (asi_kt_i > max_as) {
        asi_kt_i = max_as;
    }
    l = AirSpeedIndicator::angle_scale_map[asi_kt_i / to_sub_script][0];
    u = AirSpeedIndicator::angle_scale_map[asi_kt_i / to_sub_script][1];

    return l + (u - l) / milli_angle * (asi_kt_i % (scale_as * 10));
}

void AirSpeedIndicator::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_frame);
    *p = QImage(Config::img_prefix + IMG_ASI_FRAME);
    if (p->isNull()) {
        LOGE("Failed to load asi frame");
    }

    p = const_cast<QImage *>(img_plate);
    *p = QImage(Config::img_prefix + IMG_ASI_PLATE);
    if (p->isNull()) {
        LOGE("Failed to load asi plate");
    }

    loaded = true;
}

const QPoint AirSpeedIndicator::needle[4] = {QPoint(-5, 0),
                                             QPoint(0, 5),
                                             QPoint(85, 0),
                                             QPoint(0, -5)};
QMutex AirSpeedIndicator::img_lock;
bool AirSpeedIndicator::loaded = false;
const QImage * const AirSpeedIndicator::img_plate = new QImage();
const QImage * const AirSpeedIndicator::img_frame = new QImage();

// Map air speed to angle.
//
//      angle_scale_map[i][0]: the angle for air speed (i * 10).
//      angle_scale_map[i][1]: the angle for air speed (i * 10 + 9).
//
// The angle for air speed from (i * 10) to (i * 10 + 10) exclusive is calculated by:
//
//      [i][0] + ([i][1] - [i][0]) / 1000 * (air_speed * 100 % 1000)
//
const double AirSpeedIndicator::angle_scale_map[20][2] = {
    {  0,   1}, // 0-9
    {  2,   5}, // 10-10
    {  6,  14}, // 20-29
    { 15,  39}, // 30-39
    { 40,  65}, // 40-49
    { 66,  91}, // 50-59
    { 92, 118}, // 60-69
    {119, 144}, // 70-79
    {145, 170}, // 80-89
    {171, 196}, // 90-99
    {197, 217}, // 100-109
    {218, 238}, // 110-119
    {239, 259}, // 120-129
    {260, 279}, // 130-139
    {280, 300}, // 140-149
    {301, 321}, // 150-159
    {322, 338}, // 160-169, the speed to see god
    {339, 351}, // 170-179, the speed to see god
    {352, 358}, // 180-189, the speed to see god
    {359, 360}  // 190-199, the speed to see god
};
