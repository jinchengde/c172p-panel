#ifndef RADIO_H
#define RADIO_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QPaintEvent>
#include "knob.h"
#include "fgdata.h"
#include "imgres.h"

class Radio : public QWidget, public FGDataConsumer, public FGDataUpdater
{
    Q_OBJECT
public:
    explicit Radio(QWidget *parent = 0);
    void set_prefix(const QString &p) { prefix = p; }
    void paintEvent(QPaintEvent *);
    enum TYPE {NONE, COM, NAV};
    void set_type(Radio::TYPE t) { type = t; }

signals:

public slots:
    void update_mhz(Knob::direcrion d);
    void update_khz(Knob::direcrion d);
    void update_volume(Knob::direcrion d);
    void swap_frq();

private:
    Knob k_mhz;
    Knob k_khz;
    Knob k_vol;
    QPushButton swap;
    QString prefix;
    int type;
    bool power_button;
    double a_mhz;
    double s_mhz;
    QString a_mhz_fmt;
    QString s_mhz_fmt;
    double volume;
    double old_nav_volume;
};

#endif // RADIO_H
