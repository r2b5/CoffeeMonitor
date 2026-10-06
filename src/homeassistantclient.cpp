#include "homeassistantclient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QUrl>

HomeAssistantClient::HomeAssistantClient(const QString &baseUrl, const QString &token, const QString &entityId, QObject *parent)
    : QObject(parent), m_baseUrl(baseUrl), m_token(token),m_entityid(entityId)
{
    // Basis-URL bereinigen, falls am Ende der Schrägstrich fehlt
    if (!m_baseUrl.endsWith("/")) {
        m_baseUrl.append("/");
    }

    // Zentraler Slot für alle beendeten Netzwerk-Anfragen
    connect(&m_manager, &QNetworkAccessManager::finished, this, &HomeAssistantClient::onReplyFinished);
}

void HomeAssistantClient::fetchEntityState(const QString &entityId) {
    // URL zusammenbauen (z.B. http://fwd101:8123/api/states/sensor.kanne_1_fullstand)
    QUrl url(m_baseUrl + "api/states/" + entityId);
    QNetworkRequest request(url);

    // Header setzen
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + m_token.toUtf8());

    // Anfrage abschicken (wird von m_manager im Hintergrund verarbeitet)
    QNetworkReply *reply = m_manager.get(request);

    // Die EntityId im Reply-Objekt mitspeichern, damit wir im Slot wissen, wer geantwortet hat
    reply->setProperty("entityId", entityId);
}

void HomeAssistantClient::onReplyFinished(QNetworkReply *reply) {
    // EntityID wieder auslesen
    QString entityId = reply->property("entityId").toString();

    // Fehlerprüfung
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(QString("Fehler bei %1: %2").arg(entityId, reply->errorString()));
        reply->deleteLater();
        return;
    }

    // JSON-Antwort parsen
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        QString state = jsonObj.value("state").toString();
        QJsonObject attributes = jsonObj.value("attributes").toObject();

        // Signalisieren, dass die Daten bereitstehen
        emit entityStateReceived(entityId, state, attributes);
    } else {
        emit errorOccurred(QString("Ungültiges JSON von Entität %1 empfangen.").arg(entityId));
    }

    // Speicher freigeben
    reply->deleteLater();
}

