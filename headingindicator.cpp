#include <QPainter>
#include <QMutexLocker>
#include "headingindicator.h"
#include "imgres.h"
#include "logutil.h"
#include "config.h"

HeadingIndicator::HeadingIndicator(QWidget *parent)
    : QWidget(parent),
      FGDataConsumer(),
      FGDataUpdater(),
      heading_deg(0.0),
      heading_offset(0.0),
      heading_bug_error(0.0),
      heading_bug_offset(0.0),
      k_heading(this, IMG_K_HI_HEAD),
      k_bug(this, IMG_K_HI_BUG)
{
    load_img();

    k_heading.setGeometry(10, 240, 50, 50);
    QObject::connect(&k_heading, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_heading(Knob::direcrion)));
    k_bug.setGeometry(240, 240, 50, 50);
    QObject::connect(&k_bug, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_bug(Knob::direcrion)));
}

void HeadingIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 0, 0, 255));
    const int h = this->height();
    const int w = this->width();

    heading_deg = get_data_by_key(FGDATA_HI_HEADING, heading_deg);
    heading_bug_error = get_data_by_key(FGDATA_HI_HEADING_BUG_ERROR, heading_bug_error);

    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw plate
    painter.save();
    painter.rotate(-heading_deg);
    painter.drawImage(-w / 2, -h / 2, *img_plate);
    painter.restore();

    // draw heading bug
    painter.save();
    painter.rotate(heading_bug_error);
    painter.drawConvexPolygon(HeadingIndicator::bug, 5);
    painter.restore();

    // draw plane and frame
    painter.drawImage(-w / 2, -h / 2, *img_plane);
    painter.drawImage(-w / 2, -h / 2, *img_frame);
}

void HeadingIndicator::update_heading(Knob::direcrion d)
{
    heading_offset = get_data_by_key(FGDATA_HI_OFFSET_DEG, heading_offset);
    if (d == Knob::LEFT) {
        if (heading_offset >= 359) {
            heading_offset -= 360;
        }
        set_data_by_key(FGDATA_HI_OFFSET_DEG, heading_offset + 1);
    } else if (d == Knob::RIGHT) {
        if (heading_offset < 1) {
            heading_offset += 360;
        }
        set_data_by_key(FGDATA_HI_OFFSET_DEG, heading_offset - 1);
    }
}

void HeadingIndicator::update_bug(Knob::direcrion d)
{
    heading_bug_offset = get_data_by_key(FGDATA_HI_BUG_OFFSET_DEG, heading_bug_offset);
    if (d == Knob::LEFT) {
        if (heading_bug_offset < 1) {
            heading_bug_offset += 360;
        }
        set_data_by_key(FGDATA_HI_BUG_OFFSET_DEG, heading_bug_offset - 1);
    } else if (d == Knob::RIGHT) {
        if (heading_bug_offset >= 359) {
            heading_bug_offset -= 360;
        }
        set_data_by_key(FGDATA_HI_BUG_OFFSET_DEG, heading_bug_offset + 1);
    }
}

void HeadingIndicator::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_plate);
    *p = QImage(Config::img_prefix + IMG_HI_PLATE);
    if (p->isNull()) {
        LOGE("Failed to load hi plate");
    }

    p = const_cast<QImage *>(img_plane);
    *p = QImage(Config::img_prefix + IMG_HI_PLANE);
    if (p->isNull()) {
        LOGE("Failed to load hi frame");
    }

    p = const_cast<QImage *>(img_frame);
    *p = QImage(Config::img_prefix + IMG_HI_FRAME);
    if (p->isNull()) {
        LOGE("Failed to load hi frame");
    }

    loaded = true;
}

QMutex HeadingIndicator::img_lock;
bool HeadingIndicator::loaded = false;
const QImage * const HeadingIndicator::img_plate = new QImage();
const QImage * const HeadingIndicator::img_plane = new QImage();
const QImage * const HeadingIndicator::img_frame = new QImage();
const QPoint HeadingIndicator::bug[5] = {
    QPoint(-10, -110),
    QPoint(10, -110),
    QPoint(3, -107),
    QPoint(0, -70),
    QPoint(-3, -107)
};
