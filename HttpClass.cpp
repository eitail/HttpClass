#include "HttpClass.h"

/*****************************************************
 *
 *     test
 *     QJsonObject obj;
       obj.insert("terminal_key","4D9662E9D0AAC255");
       obj.insert("order_date","2018-01-24 15:19:22");
       obj.insert("vip_num",123);
       obj.insert("order_num","ICE_000_01_0001_Ali_180124192111");
       obj.insert("pre_qty","5");
       obj.insert("kiosk_initstatus","1");
       obj.insert("pay_way","13");
       obj.insert("kiosk_temperature","5");
       obj.insert("kiosk_humidity","50");
       obj.insert("kiosk_ozone","5");

       QJsonArray arry;
       QJsonObject objc1;
       objc1.insert("flavor_id", "5");
       objc1.insert("qty","1");
       objc1.insert("status","0");
       arry.append(objc1);
       QJsonObject objc2;
       objc2.insert("flavor_id", "5");
       objc2.insert("qty","1");
       objc2.insert("status","0");
       arry.append(objc2);
       obj.insert("sales_info",arry);

       QString url = "http://www.aaa.com/";
       qDebug() << m_pHttpClass->post_json(url,obj);
 *
 *****************************************************/
HttpClass::HttpClass()
{

}

HttpClass* HttpClass::p = NULL;
HttpClass* HttpClass::getInstance()
{
    if( p == NULL )
    {
        p = new HttpClass();
    }
    return p;
}

/*****************************************************
 *
 * The post method sends the json data.
 *
 *****************************************************/
QByteArray HttpClass::post_json(const QString &url,QJsonObject &obj)
{
    QUrl urls( url );

//    qDebug() << "<<<<>>>><<<<>>>>" << url;
//    qDebug() << "<<<<>>>><<<<>>>>" << QJsonDocument(obj);

    QNetworkRequest request(urls);
    QNetworkAccessManager nam;

    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QNetworkReply*    reply;
    reply= nam.post(request,QJsonDocument(obj).toJson());

    if (reply == NULL)
    {
        return QByteArray();
    }

    QTime t;
    t.start();
    while (!reply->isFinished())
    {
        if (t.elapsed() > 20000)
        {
            reply->deleteLater();
            return QByteArray();
        }

        if (reply->error() != QNetworkReply::NoError)
        {
            reply->deleteLater();
            return QByteArray();
        }
        QCoreApplication::processEvents();
    }

    QByteArray bytes = reply->readAll();
    reply->deleteLater();

    QString receive;
    receive.prepend(bytes);
    return bytes;
}

/*****************************************************
 *
 * Parse the retrieved json format data.
 * return type: QString int and array
 *
 *****************************************************/
void HttpClass::parse_json(QByteArray &json_data,QString key_name)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(json_data, &jsonError);// 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)){// 解析未发生错误
        if (doucment.isObject()) {  // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains(key_name)) {
                QJsonValue value = object.take(key_name);
                if (value.isString())
                {
                    m_Return_QString = value.toString();
                }
                else if(value.isArray())
                {
                    m_Return_Array = value.toArray();
                }
                else
                {
                    m_Return_Int = value.toInt();
                }
            }
        }
    }
}
