#include "outgoing_message.h"

OutgoingMessage::OutgoingMessage(const std::optional<qint64>& chat_id, QObject* parent, const QString& method, const QUrlQuery& params, QHttpMultiPart* multi_part) 
    : QObject(parent),
      chat_id_(chat_id),
      method_(method),
      params_(params),
      multi_part_(multi_part) {
  if (multi_part_) {
    multi_part_->setParent(this);
  }
}

void OutgoingMessage::SetReply(QNetworkReply* reply) {
  connect(reply, &QNetworkReply::finished, this, &OutgoingMessage::OnSendFinished);
}

void OutgoingMessage::OnSendFinished() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
  reply->deleteLater();
  if (!reply) {
    qCritical() << "Invalid call of finish";
    return;
  }

  if (reply->error() != QNetworkReply::NoError) {
    const QVariant http_status_code = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if (http_status_code.isValid()) {
      if (http_status_code.toInt() == 400) {
        qCritical() << "Received 400 on request: " << reply->url() << (reply->bytesAvailable() ? reply->readAll().left(1024) : "nOdata"_ba);
        EmitWasSendAndFinished(true, {});
        return;
      } else {
        qCritical() << "Received http status code: " << http_status_code;
      }
    }
    qCritical() << "SendMessage failed, params: " << reply->error() << reply->url() << (reply->bytesAvailable() ? reply->readAll().left(1024) : "nOdata"_ba);
    OnFail();
    return;
  }

  if (!reply->bytesAvailable()) {
    qCritical() << "No data available" << reply->error() << reply->url();
    OnFail();
    return;
  }

  const QByteArray data = reply->readAll();
  QJsonObject jo{QJsonDocument::fromJson(data).object()};
  if (!jo.contains(u"ok"_s)) {
    qCritical() << "Received invalid JSON: " << reply->url() << data.left(1024);
    EmitWasSendAndFinished(true, {});
    return;
  }

  if (!jo[u"ok"_s].toBool()) {
    qCritical() << "Received not OK: " << reply->url() << data.left(1024);
    EmitWasSendAndFinished(true, {});
    return;
  }

  EmitWasSendAndFinished(false, jo);
}

void OutgoingMessage::OnFail() {
  --index_of_try_to_send_;
  if (index_of_try_to_send_ <= 0) {
    qCritical() << "It was final try to send message";
    EmitWasSendAndFinished(true, {});
    return;
  }

  QTimer::singleShot(kDefIntervalBetweenSend, this, &OutgoingMessage::ResendMe);
}

void OutgoingMessage::EmitWasSendAndFinished(bool error, const QJsonObject& jo) {
  Q_EMIT WasSend(error, jo);
  Q_EMIT Finished();
}
