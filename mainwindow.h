#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include "fgdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
    void read_fg_data(); // read the new coming fg data

public slots:
    void send_fg_data();

private:
	Ui::MainWindow *ui;
    QUdpSocket fg_socket_read;
    QUdpSocket fg_socket_write;
    bool can_send_data;
    QTimer refresh_timer; // to update all the instruments' display
    QTimer send_data_timer; // to send fg data peroidically
    FGData fg_data_in; // fg data incoming
    FGData fg_data_out; // fg data sending to fg
};

#endif // MAINWINDOW_H
