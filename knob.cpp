#include <QPoint>
#include <QPainter>
#include <QCursor>
#include "knob.h"
#include "logutil.h"
#include "config.h"
#include "imgres.h"

Knob::Knob(QWidget *parent, const QString &img)
    : QWidget(parent),
      angle(0),
      img_knob(Config::img_prefix + img)
{
    // use sizehorcursor to mimic fg
    setCursor(QCursor(Qt::SizeHorCursor));

    if (img_knob.isNull()) {
        LOGE("Failed to load knob: %s", img.toUtf8().data());
    }
}

void Knob::wheelEvent(QWheelEvent *we)
{
    QPoint rotate = we->angleDelta() / 120;

    if (rotate.y() < 0) {
        angle -= 10;
        emit rotated(Knob::LEFT);
    } else if (rotate.y() > 0) {
        angle += 10;
        emit rotated(Knob::RIGHT);
    }
}

void Knob::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 255, 255, 255));
    QBrush brush = QBrush(QColor(255, 255, 255, 255));
    const int h = this->height();
    const int w = this->width();

    // draw knob
    painter.translate(w / 2, h / 2);
    painter.rotate(angle);
    painter.drawImage(-w / 2, -h / 2, img_knob);
}
