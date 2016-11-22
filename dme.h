#ifndef DME_H
#define DME_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include "fgdata.h"

class DME : public QWidget, public FGDataConsumer, public FGDataUpdater
{
    Q_OBJECT
public:
    explicit DME(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

public slots:
    void power_button();
    void mins_kts_button();
    void source_button();

private:
    QPushButton power_b;
    QPushButton mins_kts_b;
    QPushButton nav1_b;
    QPushButton nav2_b;
};

#endif // DME_H
