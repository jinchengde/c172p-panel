#include <QObject>
#include <QPainter>
#include "dme.h"
#include "logutil.h"

DME::DME(QWidget *parent)
    : QWidget(parent),
      power_b("Power", this),
      mins_kts_b("Mins-Kts", this),
      nav1_b("Nav1", this),
      nav2_b("Nav2", this)
{
    power_b.setGeometry(10, 35, 40, 20);
    power_b.setStyleSheet("background-color: rgb(255, 255, 255);");
    mins_kts_b.setGeometry(200, 35, 70, 20);
    mins_kts_b.setStyleSheet("background-color: rgb(255, 255, 255);");
    nav1_b.setGeometry(60, 10, 40, 20);
    nav1_b.setStyleSheet("background-color: rgb(255, 255, 255);");
    nav1_b.setObjectName("nav1");
    nav2_b.setGeometry(60, 35, 40, 20);
    nav2_b.setStyleSheet("background-color: rgb(255, 255, 255);");
    nav2_b.setObjectName("nav2");

    QObject::connect(&power_b, SIGNAL(clicked(bool)),
                     this, SLOT(power_button()));
    QObject::connect(&mins_kts_b, SIGNAL(clicked(bool)),
                     this, SLOT(mins_kts_button()));
    QObject::connect(&nav1_b, SIGNAL(clicked(bool)),
                     this, SLOT(source_button()));
    QObject::connect(&nav2_b, SIGNAL(clicked(bool)),
                     this, SLOT(source_button()));
}

void DME::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 0, 0, 255));
    QBrush brush = QBrush(QColor(255, 0, 0, 255));

    bool on = get_data_by_key(FGDATA_DME_POWER_BUTTON, 0);
    bool mk = get_data_by_key(FGDATA_DME_SWITCH_MIN_KTS, 0);
    int p = get_data_by_key(FGDATA_DME_SWITCH_POSITION, 2);
    double miles = get_data_by_key(FGDATA_DME_MILES_DISPLAY, 0);
    double mins_kts = get_data_by_key(FGDATA_DME_MIN_KTS_DISPLAY, 0);
    QString miles_s;
    QString mins_kts_s;
    QString nav_name;
    QString mk_name;

    miles_s.setNum(miles / 10, 'f', 1);
    mins_kts_s.setNum(mins_kts, 'f', 0);

    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    QFont f = painter.font();
    f.setBold(true);
    f.setStyleHint(QFont::Monospace);
    f.setFamily("Monospace");
    f.setPixelSize(20);
    painter.setFont(f);
    painter.setBrush(brush);

    // dark red, LED-like background
    painter.fillRect(0, 0, width(), height(), QColor(20, 0, 0));

    // device is off
    if (!on) {
        return;
    }

    painter.drawText(130, 30, miles_s);
    painter.drawText(240, 30, mins_kts_s);

    // draw nav source name and mins-kts label.
    QPen y = QPen(QColor(200, 200, 0));
    QFont f_n = painter.font();
    f_n.setBold(false);
    f_n.setPixelSize(13);
    painter.setFont(f_n);
    painter.setPen(y);
    if (p == 1) {
        nav_name = "N1";
    } else if (p == 3) {
        nav_name = "N2";
    }
    painter.drawText(110, 30, nav_name);

    if (mk) {
        mk_name = "Kts";
    } else {
        mk_name = "Mins";
    }
    painter.drawText(200, 30, mk_name);
}

void DME::power_button()
{
    bool on = get_data_by_key(FGDATA_DME_POWER_BUTTON, 0);

    if (on) {
        set_data_by_key(FGDATA_DME_POWER_BUTTON, 0);
    } else {
        set_data_by_key(FGDATA_DME_POWER_BUTTON, 1);
    }
}

void DME::mins_kts_button()
{
    bool status = get_data_by_key(FGDATA_DME_SWITCH_MIN_KTS, 0);

    if (status) {
        set_data_by_key(FGDATA_DME_SWITCH_MIN_KTS, 0);
    } else {
        set_data_by_key(FGDATA_DME_SWITCH_MIN_KTS, 1);
    }
}

#define SOURCE_NAV1 "instrumentation/nav/frequencies/selected-mhz"
#define SOURCE_NAV2 "instrumentation/nav[1]/frequencies/selected-mhz"

void DME::source_button()
{
    QObject *s = sender();

    if (s->objectName() == "nav1") {
        set_data_by_key(FGDATA_DME_SWITCH_POSITION, 1);
        set_data_by_key(FGDATA_DME_SOURCE, SOURCE_NAV1);
    } else if (s->objectName() == "nav2") {
        set_data_by_key(FGDATA_DME_SWITCH_POSITION, 3);
        set_data_by_key(FGDATA_DME_SOURCE, SOURCE_NAV2);
    } else {
        LOGE("Invalid nav source sender");
    }
}
