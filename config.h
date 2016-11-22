#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    Config();
    static unsigned short port_read; // the port for input
    static unsigned short port_write; // the port for output
    static unsigned short hz; // fg data packets sent per second
    static QString img_prefix; // image resource path prefix
    static QString fgfs_host; // fgfs host name or ip
    static QString local_ip;
};

#endif // CONFIG_H
