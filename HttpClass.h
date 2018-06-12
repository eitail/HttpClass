#ifndef HTTPCLASS_H
#define HTTPCLASS_H

#include "httpclass_global.h"
#include <QCoreApplication>
#include <QTimer>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QtNetwork>
#include <QDebug>

class HTTPCLASSSHARED_EXPORT HttpClass : QObject
{
    public:
        static HttpClass*   getInstance();
        QByteArray          post_json(const QString &url, QJsonObject &obj);
        void                parse_json(QByteArray &json_data, QString key_name);

    private:
        HttpClass();

    public:
        int         m_Return_Int;
        QString     m_Return_QString;
        QJsonArray  m_Return_Array;

    private:
        static  HttpClass*      p;

    signals:

    public slots:

};

#endif // HTTPCLASS_H
