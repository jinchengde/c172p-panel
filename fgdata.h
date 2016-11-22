#ifndef FGDATA_H
#define FGDATA_H

#include <QString>
#include <QMap>
#include <QMutex>
#include "datanames.h"

class FGData
{
public:
    FGData();
    double get_data_by_key(const QString &, const double);
    void set_data_by_key(const QString &key, const double new_data);
    QString get_data_by_key(const QString &, const QString &);
    void set_data_by_key(const QString &key, const QString &new_data);
    void set_raw_data(const QString &);
    QString get_raw_data();

private:
    QMap<QString, QString> data_map; // key-value pairs of the flight data
};


////////////////////////
// The FGDataConsumer class
// All instruments are consumers.

class FGDataConsumer
{
public:
    FGDataConsumer();
    ~FGDataConsumer() {}
    void set_fg_data_source_in(FGData *data);

protected:
    double get_data_by_key(const QString &, const double);
    QString get_data_by_key(const QString &, const QString &);

private:
    FGData *fg_data;
};


////////////////////////
// The FGDataUpdater class
// All instrument knobs are updaters and consumers.

class FGDataUpdater
{
public:
    FGDataUpdater();
    ~FGDataUpdater() {}
    void set_fg_data_source_out(FGData *data);

protected:
    void set_data_by_key(const QString &, const double);
    void set_data_by_key(const QString &key, const QString &new_data);

private:
    FGData *fg_data;
};

#endif // FGDATA_H
