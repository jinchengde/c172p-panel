#include <QPainter>
#include <QMutexLocker>
#include "navigationindicator.h"
#include "imgres.h"
#include "logutil.h"
#include "config.h"

NavigationIndicator::NavigationIndicator(QWidget *parent)
    : QWidget(parent), FGDataConsumer(), FGDataUpdater(),
      prefix(""),
      has_gs(false), in_range(false), gs_in_range(false),
      to_flag(false), from_flag(false),
      radial_selected(0),
      heading_deflection(10),
      gs_deflection(3.5),
      old_heading_deflection(10),
      old_gs_deflection(3.5),
      k_obs(this, IMG_K_NAV_OBS)
{
    load_img();

    k_obs.setGeometry(10, 240, 50, 50);
    QObject::connect(&k_obs, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_obs(Knob::direcrion)));
}

void NavigationIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    has_gs = get_data_by_key(prefix + FGDATA_NAV_HAS_GS, has_gs);
    in_range = get_data_by_key(prefix + FGDATA_NAV_IN_RANGE, in_range);
    gs_in_range = get_data_by_key(prefix + FGDATA_NAV_GS_IN_RANGE, gs_in_range);
    gs_deflection = get_data_by_key(prefix + FGDATA_NAV_GS_DEFLECTION, gs_deflection);
    heading_deflection = get_data_by_key(prefix + FGDATA_NAV_HEADING_DEFLECTION, heading_deflection);
    radial_selected = get_data_by_key(prefix + FGDATA_NAV_RADIAL_SELECTED_DEG, radial_selected);
    to_flag = get_data_by_key(prefix + FGDATA_NAV_TO_FLAG, to_flag);
    from_flag = get_data_by_key(prefix + FGDATA_NAV_FROM_FLAG, from_flag);

    // make the needle move smoothly
    old_heading_deflection += (heading_deflection - old_heading_deflection) / 40;
    old_gs_deflection += (gs_deflection - old_gs_deflection) / 40;

    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw gs flag
    if (has_gs && gs_in_range) {
        painter.drawImage(-85, -25, *img_gs_flag);
    } else {
        painter.drawImage(-105, -25, *img_gs_flag);
    }

    // draw to from flag
    if (in_range) {
        if (to_flag) {
            painter.drawImage(18, -30, *img_to_from_flag);
        } else if (from_flag) {
            painter.drawImage(18, -70, *img_to_from_flag);
        } else {
            painter.drawImage(18, -50, *img_to_from_flag);
        }
    } else {
        painter.drawImage(18, -50, *img_to_from_flag);
    }

    // draw plate
    painter.drawImage(-w / 2, -h / 2, *img_plate);

    // TODO: draw heading needle
    painter.save();
    painter.translate(0, -60);
    painter.rotate(90 - old_heading_deflection * 4.5);
    painter.drawConvexPolygon(needle, 4);
    painter.restore();

    // TODO: draw gs needle
    painter.save();
    painter.translate(-60, 0);
    painter.rotate(-old_gs_deflection * 8);
    painter.drawConvexPolygon(needle, 4);
    painter.restore();

    // draw ring
    painter.save();
    painter.rotate(-radial_selected);
    painter.drawImage(-w / 2, -h / 2, *img_ring);
    painter.restore();

    // draw frame
    painter.drawImage(-w / 2, -h / 2, *img_frame);
}

void NavigationIndicator::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_frame);
    *p = QImage(Config::img_prefix + IMG_NAV_FRAME);
    if (p->isNull()) {
        LOGE("Failed to load vor frame");
    }

    p = const_cast<QImage *>(img_ring);
    *p = QImage(Config::img_prefix + IMG_NAV_RING);
    if (p->isNull()) {
        LOGE("Failed to load vor ring");
    }

    p = const_cast<QImage *>(img_plate);
    *p = QImage(Config::img_prefix + IMG_NAV_PLATE);
    if (p->isNull()) {
        LOGE("Failed to load vor plate");
    }

    p = const_cast<QImage *>(img_gs_flag);
    *p = QImage(Config::img_prefix + IMG_NAV_GS_FLAG);
    if (p->isNull()) {
        LOGE("Failed to load vor gs flag");
    }

    p = const_cast<QImage *>(img_to_from_flag);
    *p = QImage(Config::img_prefix + IMG_NAV_TO_FROM_FLAG);
    if (p->isNull()) {
        LOGE("Failed to load vor to from flag");
    }

    loaded = true;
}

void NavigationIndicator::update_obs(Knob::direcrion d)
{
    radial_selected = get_data_by_key(prefix + FGDATA_NAV_RADIAL_SELECTED_DEG, radial_selected);

    if (d == Knob::LEFT) {
        if (radial_selected >= 359) {
            radial_selected -= 360;
        }
        set_data_by_key(prefix + FGDATA_NAV_RADIAL_SELECTED_DEG, radial_selected + 1);
    } else if (d == Knob::RIGHT) {
        if (radial_selected < 1) {
            radial_selected += 360;
        }
        set_data_by_key(prefix + FGDATA_NAV_RADIAL_SELECTED_DEG, radial_selected - 1);
    }
}

QMutex NavigationIndicator::img_lock;
bool NavigationIndicator::loaded = false;
const QImage * const NavigationIndicator::img_ring = new QImage();
const QImage * const NavigationIndicator::img_frame = new QImage();
const QImage * const NavigationIndicator::img_plate = new QImage();
const QImage * const NavigationIndicator::img_to_from_flag = new QImage();
const QImage * const NavigationIndicator::img_gs_flag = new QImage();
const QPoint NavigationIndicator::needle[4] = {
    QPoint(0, -2),
    QPoint(120, -2),
    QPoint(120, 2),
    QPoint(0, 2)
};
