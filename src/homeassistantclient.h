#ifndef HOMEASSISTANTCLIENT_H
#define HOMEASSISTANTCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>


class HomeAssistantClient : public QObject
{
    Q_OBJECT
public:
    explicit HomeAssistantClient(const QString &baseUrl, const QString &token, const QString &entityId, QObject *parent = nullptr);

    // Methode zum Abfragen einer beliebigen Entität (z.B. "sensor.kanne_1_fullstand")
    void fetchEntityState(){fetchEntityState(m_entityid);};
    void fetchEntityState(const QString &entityId);

signals:
    // Signal wird gefeuert, wenn die Daten erfolgreich da sind
    void entityStateReceived(const QString &entityId, const QString &state, const QJsonObject &attributes);

    // Signal wird gefeuert, wenn ein Fehler auftritt
    void errorOccurred(const QString &errorString);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager m_manager;
    QString m_baseUrl;
    QString m_token;
    QString m_entityid;
};

#endif // HOMEASSISTANTCLIENT_H
