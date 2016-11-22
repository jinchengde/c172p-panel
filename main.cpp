#include <QApplication>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "mainwindow.h"
#include "logutil.h"
#include "config.h"

using namespace std;

#define ERR_INVALID_OPT 1

// r: the port to read
// w: the port to write
// s: fgfs ip: x.x.x.x
// l: ip address this app will bind to
// f: frequency to send fg data
// p: images path prefix
// h: print help message
char option[] = "r:w:hf:p:s:l:";

int main(int argc, char *argv[])
{
    // parse command line options
    int opt = 0;
    while ((opt = getopt(argc, argv, option)) != -1) {
        switch (opt) {
        case 'r':
            Config::port_read = atoi(optarg);
            break;
        case 'w':
            Config::port_write = atoi(optarg);
            break;
        case 's':
            Config::fgfs_host = optarg;
            break;
        case 'l':
            Config::local_ip = optarg;
            break;
        case 'f':
            Config::hz = atoi(optarg);
            break;
        case 'p':
            Config::img_prefix = optarg;
            break;
            break;
        case 'h':
            cout << "Usage:\n"
                 << "    c172p-panel <options>\n"
                 << "\nOption\n"
                 << "    -r    the port to read, default to 5500\n"
                 << "    -w    the port to write, default to 5501\n"
                 << "    -l    local IP (x.x.x.x) to bind to, default to 127.0.0.1\n"
                 << "    -s    fgfs IP (x.x.x.x), default to 127.0.0.1\n"
                 << "    -f    frequency to send fg data, default to 20hz\n"
                 << "    -p    image path prefix, default to current dir\n"
                 << "    -h    print help message\n"
                 << endl;
            exit(0);
        default: /* '?' */
            cerr << "Usage: "
                 << argv[0]
                 << "[-r port_read] [-w port_write] [-p prefix] [-s fgfs_host] [-f frequency to send data]"
                 << endl;
            exit(ERR_INVALID_OPT);
        }
    }

    QApplication app(argc, argv);
	MainWindow w;
	w.show();

    return app.exec();
}
