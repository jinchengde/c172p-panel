#ifndef RPM_H
#define RPM_H

#include <QObject>
#include <QWidget>
#include <QMutex>
#include <QImage>
#include "fgdata.h"

class RPM : public QWidget, public FGDataConsumer
{
    Q_OBJECT
public:
    explicit RPM(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

private:
    double rpm_to_angle();
    void load_img();

    double rpm;
    bool running;
    static const QPoint needle[4];

    static QMutex img_lock;
    static bool loaded;
    static const QImage * const img_plate;
};

#endif // RPM_H
