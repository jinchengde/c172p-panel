#include <QPainter>
#include <QMutexLocker>
#include "attitudeindicator.h"
#include "imgres.h"
#include "logutil.h"
#include "config.h"

AttitudeIndicator::AttitudeIndicator(QWidget *parent)
    : QWidget(parent),
      FGDataConsumer(),
      FGDataUpdater(),
      ai_pitch(0.0),
      ai_pitch_offset(0.0),
      ai_roll(0.0),
      k_offset(this, IMG_K_AI_OFFSET)
{
    load_img();
    k_offset.setGeometry(125, 240, 50, 50);
    QObject::connect(&k_offset, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_offset(Knob::direcrion)));
}

void AttitudeIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    ai_pitch = get_data_by_key(FGDATA_AI_PITCH, ai_pitch);
    ai_pitch_offset = get_data_by_key(FGDATA_AI_OFFSET, ai_pitch_offset);
    ai_roll = get_data_by_key(FGDATA_AI_ROLL, ai_roll);

    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.translate(QPoint(w / 2, h / 2));

    // draw background horizon
    painter.save();
    painter.rotate(-ai_roll);
    painter.fillRect(-w / 2, -h / 2, w, h / 2, QColor(90, 160, 250, 255)); // the blue sky
    painter.fillRect(-w / 2, 0, w, h / 2, QColor(200, 130, 70, 255)); // the brown ground
    painter.restore();

    // draw horizon2
    painter.save();
    painter.rotate(-ai_roll);
    painter.translate(0, pitch_to_translate());
    painter.drawImage(-w / 2, -h / 2, *img_horizon2);
    painter.restore();

    // draw horizon3
    painter.save();
    painter.rotate(-ai_roll);
    painter.drawImage(-w / 2, -h / 2, *img_horizon3);
    painter.restore();

    // draw plane and frame
    painter.drawImage(-w / 2, -h / 2, *img_plane);
    painter.drawImage(-w / 2, -h / 2, *img_frame);
}

void AttitudeIndicator::update_offset(Knob::direcrion d)
{
    ai_pitch_offset = get_data_by_key(FGDATA_AI_OFFSET, ai_pitch_offset);

    if (d == Knob::LEFT) {
        set_data_by_key(FGDATA_AI_OFFSET, ai_pitch_offset - 0.1);
    } else if (d == Knob::RIGHT) {
        set_data_by_key(FGDATA_AI_OFFSET, ai_pitch_offset + 0.1);
    }
}

void AttitudeIndicator::load_img()
{
    QImage *p = NULL;

    QMutexLocker l(&img_lock);
    if (loaded) {
        return;
    }

    p = const_cast<QImage *>(img_horizon2);
    *p = QImage(Config::img_prefix + IMG_AI_HORIZON2);
    if (p->isNull()) {
        LOGE("Failed to load ai h2");
    }

    p = const_cast<QImage *>(img_horizon3);
    *p = QImage(Config::img_prefix + IMG_AI_HORIZON3);
    if (p->isNull()) {
        LOGE("Failed to load ai h3");
    }

    p = const_cast<QImage *>(img_frame);
    *p = QImage(Config::img_prefix + IMG_AI_FRAME);
    if (p->isNull()) {
        LOGE("Failed to load ai frame");
    }

    p = const_cast<QImage *>(img_plane);
    *p = QImage(Config::img_prefix + IMG_AI_PLANE);
    if (p->isNull()) {
        LOGE("Failed to load ai plane");
    }

    loaded = true;
}

QMutex AttitudeIndicator::img_lock;
bool AttitudeIndicator::loaded = false;
const QImage * const AttitudeIndicator::img_horizon2 = new QImage();
const QImage * const AttitudeIndicator::img_horizon3 = new QImage();
const QImage * const AttitudeIndicator::img_plane = new QImage();
const QImage * const AttitudeIndicator::img_frame = new QImage();
