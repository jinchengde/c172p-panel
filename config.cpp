#include "config.h"

Config::Config()
{
    // empty body
}

unsigned short Config::port_read = 5500;
unsigned short Config::port_write = 5501;
unsigned short Config::hz = 20;
QString Config::img_prefix("./");
QString Config::fgfs_host("127.0.0.1");
QString Config::local_ip("127.0.0.1");
