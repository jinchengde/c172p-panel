#include "fgdata.h"
#include "logutil.h"
#include <QMutexLocker>

FGData::FGData()
    : data_map()
{

}

double FGData::get_data_by_key(const QString &key, const double old_data)
{
    bool ok = false;
    double d = 0.0;
    QString o;
    QString v;

    o.setNum(old_data, 'f', 6);
    v = data_map.value(key, o);
    d = v.toDouble(&ok);
    if (!ok) {
        LOGE("Failed to convert string to double: %s, key: %s, old-data: %f",
             v.toUtf8().data(), key.toUtf8().data(), old_data);
    }

    return d;
}

void FGData::set_data_by_key(const QString &key, const double new_data)
{
    QString d;

    d.setNum(new_data, 'f', 6);
    data_map.insert(key, d);
}

QString FGData::get_data_by_key(const QString &key, const QString &old_data)
{
    return data_map.value(key, old_data);
}

void FGData::set_data_by_key(const QString &key, const QString &new_data)
{
    QString nd = new_data;

    nd.remove(QChar('\n'));
    data_map.insert(key, nd);
}

void FGData::set_raw_data(const QString &data)
{
    QString d;

    foreach (QString kv, data.split(',')) {
        QStringList kvl = kv.split('=');

        if (kvl.size() != 2) {
            LOGE("Data format invalid: %s", kv.toUtf8().constData());
            continue;
        }

        d = kvl[1];
        d.remove(QChar('\n'));
        data_map.insert(kvl[0], d);
    }
}

QString FGData::get_raw_data()
{
    QString raw;

    for (int i = 0; i < NUM_OF_NAMES; i++) {
        raw += data_map.value(out_data_names[i], "foobar");
        if (i != (NUM_OF_NAMES - 1)) {
            raw += ",";
        }
    }
    raw += "\n";

    return raw;
}

/////////////////
// FGDataConsumer

FGDataConsumer::FGDataConsumer()
    : fg_data(NULL)
{
    // empty body
}

void FGDataConsumer::set_fg_data_source_in(FGData *data)
{
    if (data) {
        fg_data = data;
    } else {
        LOGE("fg data in is NULL");
        return;
    }
}

double FGDataConsumer::get_data_by_key(const QString &key, const double old_data)
{
    if (fg_data != NULL) {
        return fg_data->get_data_by_key(key, old_data);
    } else {
        LOGE("fg_data is NULL");
        return old_data;
    }
}

QString FGDataConsumer::get_data_by_key(const QString &key, const QString &old_data)
{
    if (fg_data != NULL) {
        return fg_data->get_data_by_key(key, old_data);
    } else {
        LOGE("fg_data is NULL");
        return old_data;
    }
}


/////////////////
// FGDataUpdater

FGDataUpdater::FGDataUpdater()
    : fg_data(NULL)
{

}

void FGDataUpdater::set_fg_data_source_out(FGData *data)
{
    if (data) {
        fg_data = data;
    } else {
        LOGE("fg data out is NULL");
        return;
    }
}

void FGDataUpdater::set_data_by_key(const QString &key, const double new_data)
{
    if (fg_data != NULL) {
        fg_data->set_data_by_key(key, new_data);
    } else {
        LOGE("fg_data is NULL");
    }
}

void FGDataUpdater::set_data_by_key(const QString &key, const QString &new_data)
{
    if (fg_data != NULL) {
        fg_data->set_data_by_key(key, new_data);
    } else {
        LOGE("fg_data is NULL");
    }
}
