#pragma once

#include "common.h"

class BOT_EXPORT OutgoingMessage : public QObject {
  Q_OBJECT
public:
  OutgoingMessage(const std::optional<qint64>& chat_id, QObject* parent, const QString& method, const QUrlQuery& params, QHttpMultiPart* multi_part);
  std::optional<qint64> ChatId() const { return chat_id_; }
  QString Method() const { return method_; }
  QHttpMultiPart* MultiPart() const { return multi_part_; }
  QUrlQuery Params() const { return params_; }
  void SetReply(QNetworkReply* reply);

Q_SIGNALS:
  void WasSend(bool error, const QJsonObject& jo);
  void Finished(OutgoingMessage* om);
  void ResendMe(OutgoingMessage* om);

private:
  const int kDefCountTriesToSend{3};
  const int kDefIntervalBetweenSend{333};

  void OnSendFinished();
  void OnFail();
  void EmitWasSendAndFinished(bool error, const QJsonObject& jo);

  const std::optional<qint64> chat_id_;
  const QString method_;
  const QUrlQuery params_;
  int index_of_try_to_send_{kDefCountTriesToSend};
  QHttpMultiPart* multi_part_{};
};
