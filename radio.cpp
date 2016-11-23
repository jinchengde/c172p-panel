#include <QPainter>
#include "radio.h"
#include "logutil.h"

Radio::Radio(QWidget *parent)
    : QWidget(parent),
      FGDataConsumer(),
      FGDataUpdater(),
      k_mhz(this, IMG_K_MHZ),
      k_khz(this, IMG_K_KHZ),
      k_vol(this, IMG_K_VOL),
      swap("<->", this),
      prefix(""),
      type(NONE),
      power_button(false),
      a_mhz(0),
      s_mhz(0),
      a_mhz_fmt(""),
      s_mhz_fmt(""),
      volume(0),
      old_nav_volume(0)
{
    k_mhz.setGeometry(240, 0, 60, 60);
    k_khz.setGeometry(255, 25, 30, 30);
    k_khz.setCursor(Qt::SizeVerCursor);
    k_vol.setGeometry(25, 35, 20, 20);
    swap.setGeometry(100, 35, 40, 20);
    swap.setStyleSheet("background-color: rgb(255, 255, 255);");

    QObject::connect(&k_mhz, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_mhz(Knob::direcrion)));
    QObject::connect(&k_khz, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_khz(Knob::direcrion)));
    QObject::connect(&k_vol, SIGNAL(rotated(Knob::direcrion)),
                     this, SLOT(update_volume(Knob::direcrion)));
    QObject::connect(&swap, SIGNAL(clicked(bool)),
                     this, SLOT(swap_frq()));
}

void Radio::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = QPen(QColor(255, 0, 0, 255));
    QBrush brush = QBrush(QColor(255, 0, 0, 255));

    // com1 and nav1 share the same power button, so as com2 and nav2
    power_button = get_data_by_key(prefix + FGDATA_COMM_POWER_BUTTON, power_button);

    if (type == COM) {
        a_mhz_fmt = get_data_by_key(prefix + FGDATA_COMM_SELECTED_MHZ_FMT, a_mhz_fmt);
        s_mhz_fmt = get_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ_FMT, s_mhz_fmt);
    } else if (type == NAV) {
        a_mhz_fmt = get_data_by_key(prefix + FGDATA_NAV_SELECTED_MHZ_FMT, a_mhz_fmt);
        s_mhz_fmt = get_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ_FMT, s_mhz_fmt);
    } else {
        LOGE("Invalid radio type.");
        return;
    }

    // setup painter
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    QFont f = painter.font();
    f.setBold(true);
    f.setStyleHint(QFont::Monospace);
    f.setFamily("Monospace");
    f.setPixelSize(25);
    painter.setFont(f);
    painter.setBrush(brush);

    painter.fillRect(0, 0, width(), height(), QColor(20, 0, 0));

    if (!power_button) {
        return;
    }

    painter.drawText(10, 30, a_mhz_fmt);
    painter.drawText(130, 30, s_mhz_fmt);
}

void Radio::update_mhz(Knob::direcrion d)
{
    if (type == COM) {
        a_mhz = get_data_by_key(prefix + FGDATA_COMM_SELECTED_MHZ, a_mhz);
        s_mhz = get_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, s_mhz);
    } else if (type == NAV) {
        a_mhz = get_data_by_key(prefix + FGDATA_NAV_SELECTED_MHZ, a_mhz);
        s_mhz = get_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, s_mhz);
    }  else {
        LOGE("Invalid radio type.");
        return;
    }

    int hzi = s_mhz; // integer part
    double hzf = (int)(s_mhz * 1000) % 1000 / 1000.0; // fraction part

    if (d == Knob::LEFT) {
        if (type == COM) {
            if (hzi == 118) {
                hzi = 137;
            }
            set_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, hzi + hzf - 1);
        } else if (type == NAV) {
            if (hzi == 108) {
                hzi = 118;
            }
            set_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, hzi + hzf - 1);
        } else {
            LOGE("Invalid radio type.");
        }
    } else if (d == Knob::RIGHT) {
        if (type == COM) {
            if (hzi == 136) {
                hzi = 117;
            }
            set_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, hzi + hzf + 1);
        } else if (type == NAV) {
            if (hzi == 117) {
                hzi = 107;
            }
            set_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, hzi + hzf + 1);
        } else {
            LOGE("Invalid radio type.");
        }
    }
}

void Radio::update_khz(Knob::direcrion d)
{
    if (type == COM) {
        a_mhz = get_data_by_key(prefix + FGDATA_COMM_SELECTED_MHZ, a_mhz);
        s_mhz = get_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, s_mhz);
    } else if (type == NAV) {
        a_mhz = get_data_by_key(prefix + FGDATA_NAV_SELECTED_MHZ, a_mhz);
        s_mhz = get_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, s_mhz);
    } else {
        LOGE("Invalid radio type.");
        return;
    }

    double mhz = s_mhz;

    if (d == Knob::LEFT) {
        if (type == COM) {
            int hz = s_mhz;
            int nhz = s_mhz - 0.025;

            if (hz > nhz ) {
                mhz += 1;
            }
            set_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, mhz - 0.025);
        } else if (type == NAV) {
            int hz = s_mhz;
            int nhz = s_mhz - 0.05;

            if (hz > nhz ) {
                mhz += 1;
            }
            set_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, mhz - 0.05);
        } else {
            LOGE("Invalid radio type.");
        }
    } else if (d == Knob::RIGHT) {
        if (type == COM) {
            int hz = s_mhz;
            int nhz = s_mhz + 0.025;

            if (hz < nhz ) {
                mhz -= 1;
            }
            set_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, mhz + 0.025);
        } else if (type == NAV) {
            int hz = s_mhz;
            int nhz = s_mhz + 0.05;

            if (hz < nhz ) {
                mhz -= 1;
            }
            set_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, mhz + 0.05);
        } else {
            LOGE("Invalid radio type.");
        }
    }
}

void Radio::update_volume(Knob::direcrion d)
{


    double vol = 0;

    if (d == Knob::LEFT) {
        if (type == COM) {
            volume = get_data_by_key(prefix + FGDATA_COMM_VOLUME, volume);

            if (get_data_by_key(prefix + FGDATA_COMM_POWER_BUTTON , 0) == 0) {
                return;
            }

            vol = volume - 0.1;

            // if rotate the knob to left when volume is already 0,
            // power off the radio.
            if (vol < 0) {
                vol = 0;
                set_data_by_key(prefix + FGDATA_COMM_POWER_BUTTON , 0);

                // mute nav sound
                old_nav_volume = get_data_by_key(prefix + FGDATA_NAV_VOLUME, 1);
                set_data_by_key(prefix + FGDATA_NAV_VOLUME, 0);
            }
            set_data_by_key(prefix + FGDATA_COMM_VOLUME, vol);
        } else if (type == NAV) {
            volume = get_data_by_key(prefix + FGDATA_NAV_VOLUME, volume);
            vol = volume - 0.1;
            if (vol < 0) {
                vol = 0;
            }
            set_data_by_key(prefix + FGDATA_NAV_VOLUME, vol);
        } else {
            LOGE("Invalid radio type.");
            return;
        }
    } else if (d == Knob::RIGHT) {
        if (type == COM) {
            volume = get_data_by_key(prefix + FGDATA_COMM_VOLUME, volume);
            power_button = get_data_by_key(prefix + FGDATA_COMM_POWER_BUTTON , power_button);

            // from power-off to power-on
            if (!power_button) {
                // restore old nav volume
                set_data_by_key(prefix + FGDATA_NAV_VOLUME, old_nav_volume);
            }

            vol = volume + 0.1;
            if (vol > 1) {
                vol = 1;
            }
            set_data_by_key(prefix + FGDATA_COMM_VOLUME, vol);
            set_data_by_key(prefix + FGDATA_COMM_POWER_BUTTON , 1);
        } else if (type == NAV) {
            volume = get_data_by_key(prefix + FGDATA_NAV_VOLUME, volume);
            vol = volume + 0.1;
            if (vol > 1) {
                vol = 1;
            }
            set_data_by_key(prefix + FGDATA_NAV_VOLUME, vol);
        } else {
            LOGE("Invalid radio type.");
            return;
        }
    } else {
        LOGW("Unknown knob action");
    }
}

void Radio::swap_frq()
{
    if (type == COM) {
        a_mhz = get_data_by_key(prefix + FGDATA_COMM_SELECTED_MHZ, a_mhz);
        s_mhz = get_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, s_mhz);
        set_data_by_key(prefix + FGDATA_COMM_STANDBY_MHZ, a_mhz);
        set_data_by_key(prefix + FGDATA_COMM_SELECTED_MHZ, s_mhz);
    } else if (type == NAV) {
        a_mhz = get_data_by_key(prefix + FGDATA_NAV_SELECTED_MHZ, a_mhz);
        s_mhz = get_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, s_mhz);
        set_data_by_key(prefix + FGDATA_NAV_STANDBY_MHZ, a_mhz);
        set_data_by_key(prefix + FGDATA_NAV_SELECTED_MHZ, s_mhz);
    } else {
        LOGE("Invalid radio type.");
    }
}
