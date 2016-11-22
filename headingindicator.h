#ifndef HEADINGINDICATOR_H
#define HEADINGINDICATOR_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QMutex>
#include "fgdata.h"
#include "knob.h"

class HeadingIndicator : public QWidget, public FGDataConsumer, public FGDataUpdater
{
	Q_OBJECT
public:
	explicit HeadingIndicator(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

public slots:
    void update_heading(Knob::direcrion d);
    void update_bug(Knob::direcrion d);

private:
    void load_img();

    double heading_deg;
    double heading_offset;
    double heading_bug_error;
    double heading_bug_offset;
    Knob k_heading;
    Knob k_bug;

    static QMutex img_lock;
    static bool loaded;
    static const QImage * const img_plate;
    static const QImage * const img_plane;
    static const QImage * const img_frame;

    static const QPoint bug[5];
};

#endif // HEADINGINDICATOR_H
