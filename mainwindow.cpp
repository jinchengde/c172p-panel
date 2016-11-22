#include <QByteArray>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logutil.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::MainWindow),
    fg_socket_read(this),
    fg_socket_write(this),
    can_send_data(false),
    refresh_timer(),
    send_data_timer(),
    fg_data_in(),
    fg_data_out()
{
	ui->setupUi(this);
    ui->ai->set_fg_data_source_in(&fg_data_in);
    ui->alt->set_fg_data_source_in(&fg_data_in);
    ui->asi->set_fg_data_source_in(&fg_data_in);
    ui->hi->set_fg_data_source_in(&fg_data_in);
    ui->nav1->set_fg_data_source_in(&fg_data_in);
    ui->nav2->set_fg_data_source_in(&fg_data_in);
    ui->tsi->set_fg_data_source_in(&fg_data_in);
    ui->vsi->set_fg_data_source_in(&fg_data_in);
    ui->rpm->set_fg_data_source_in(&fg_data_in);
    ui->radio_c1->set_fg_data_source_in(&fg_data_in);
    ui->radio_c2->set_fg_data_source_in(&fg_data_in);
    ui->radio_n1->set_fg_data_source_in(&fg_data_in);
    ui->radio_n2->set_fg_data_source_in(&fg_data_in);
    ui->dme->set_fg_data_source_in(&fg_data_in);

    ui->ai->set_fg_data_source_out(&fg_data_out);
    ui->alt->set_fg_data_source_out(&fg_data_out);
    ui->hi->set_fg_data_source_out(&fg_data_out);
    ui->nav1->set_fg_data_source_out(&fg_data_out);
    ui->nav2->set_fg_data_source_out(&fg_data_out);
    ui->radio_c1->set_fg_data_source_out(&fg_data_out);
    ui->radio_c2->set_fg_data_source_out(&fg_data_out);
    ui->radio_n1->set_fg_data_source_out(&fg_data_out);
    ui->radio_n2->set_fg_data_source_out(&fg_data_out);
    ui->dme->set_fg_data_source_out(&fg_data_out);

    ui->nav1->set_prefix("");
    ui->nav2->set_prefix("1");
    ui->radio_c1->set_prefix("");
    ui->radio_c2->set_prefix("1");
    ui->radio_n1->set_prefix("");
    ui->radio_n2->set_prefix("1");

    ui->radio_c1->set_type(Radio::COM);
    ui->radio_c2->set_type(Radio::COM);
    ui->radio_n1->set_type(Radio::NAV);
    ui->radio_n2->set_type(Radio::NAV);

    if (fg_socket_read.bind(QHostAddress(Config::local_ip), Config::port_read)) {
        LOGI("read_socket bind to %s:%hu", Config::local_ip.toUtf8().data(), Config::port_read);
    } else {
        LOGE("read_socket failed to bind to %hu", Config::port_read);
    }

    QObject::connect(&fg_socket_read, SIGNAL(readyRead()), this, SLOT(read_fg_data()));

    // setup the timer for updating display
    refresh_timer.setInterval(50); // 20 fps
    QObject::connect(&refresh_timer, SIGNAL(timeout()), this, SLOT(update()));
    refresh_timer.start();

    // setup the timer for sending fg data
    send_data_timer.setInterval(1000 / Config::hz);
    QObject::connect(&send_data_timer, SIGNAL(timeout()), this, SLOT(send_fg_data()));
    send_data_timer.start();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::send_fg_data()
{
    if (!can_send_data) {
        LOGI("can not send data");
        return;
    }

    QString raw = fg_data_out.get_raw_data();
    QByteArray a = raw.toUtf8();

    int ret = fg_socket_write.writeDatagram(a, QHostAddress(Config::fgfs_host), Config::port_write);
    if (ret == -1) {
        LOGE("Failed to output fg data");
    }
}

void MainWindow::read_fg_data()
{
    while (fg_socket_read.hasPendingDatagrams()) {
        QByteArray datagram;
        QString data_str;

        datagram.resize(fg_socket_read.pendingDatagramSize());
        fg_socket_read.readDatagram(datagram.data(), datagram.size());
        data_str = QString(datagram);
        fg_data_in.set_raw_data(data_str);
    }

    if (!can_send_data) {
        // initialize fg_data_out
        QString d;

        for (int i = 0; i < NUM_OF_NAMES; i++) {
            d = fg_data_in.get_data_by_key(out_data_names[i], "magic");
            if (d == "magic") {
                LOGE("No valid data for key: %s", out_data_names[i]);
            }
            fg_data_out.set_data_by_key(out_data_names[i], d);
        }
        can_send_data = true;
    }
}
