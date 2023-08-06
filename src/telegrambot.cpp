#include "telegrambot.h"

QMap<qint16, HttpServer*> TelegramBot::webHookWebServers = QMap<qint16, HttpServer*>();

TelegramKeyboardRequest TelegramBot::ConstructInlineMenu(const QList<std::tuple<QString, QString>>& menu) {
  TelegramKeyboardRequest keyboard;
  for (const std::tuple<QString, QString>& menu_entry : menu) {
    keyboard.append(QList<TelegramBotKeyboardButtonRequest>());
    keyboard.last().append(TelegramBot::ConstructInlineButton(std::get<0>(menu_entry), std::get<1>(menu_entry)));
  }

  return keyboard;
}

TelegramBot::TelegramBot(QString apikey, QObject *parent) : QObject(parent), apiKey(apikey) { }

TelegramBot::~TelegramBot() {
    qDeleteAll(messageRoutes);
}

/*
 * Bot Functions
 */
TelegramBotUser TelegramBot::getMe()
{
    return TelegramBotUser(this->callApiJson("getMe").value("result").toObject());
}

void TelegramBot::sendChatAction(QVariant chatId, TelegramBotChatAction action, bool *response)
{
    return this->sendChatAction(chatId, action == TelegramBotChatAction::Typing            ? "typing" :
                                        action == TelegramBotChatAction::UploadPhoto       ? "upload_photo" :
                                        action == TelegramBotChatAction::RecordVideo       ? "record_video" :
                                        action == TelegramBotChatAction::UploadVideo       ? "upload_video" :
                                        action == TelegramBotChatAction::RecordAudio       ? "record_audio" :
                                        action == TelegramBotChatAction::UploadAudio       ? "upload_audio" :
                                        action == TelegramBotChatAction::UploadDocument    ? "upload_document" :
                                        action == TelegramBotChatAction::FindLocation      ? "find_location" :
                                        action == TelegramBotChatAction::RecordVideoNote   ? "record_video_note" :
                                        action == TelegramBotChatAction::UploadVideoNote   ? "upload_video_note" : "", response);
}

void TelegramBot::sendChatAction(QVariant chatId, QString action, bool *response)
{
    // param check
    if(action.isEmpty()) return;

    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("action", action);

    // call api
    CallApiTemplate("sendChatAction", params, response);
}

TelegramBotFile TelegramBot::getFile(QString fileId, bool generateAbsoluteLink)
{
    // prepare
    QDateTime validUntil = QDateTime::currentDateTime().addSecs(3600);

    // build params
    QUrlQuery params;
    params.addQueryItem("file_id", fileId);

    // construct TelegramBotFile
    TelegramBotFile file(this->callApiJson("getFile", params).value("result").toObject());
    file.validUntil = validUntil;
    if(generateAbsoluteLink && !file.filePath.isEmpty()) file.link = QString("https://api.telegram.org/file/bot%1/%2").arg(this->apiKey, file.filePath);
    return file;
}

/*
 * User Functions
 */
TelegramBotUserProfilePhotos TelegramBot::getUserProfilePhotos(qint32 userId, int offset, int limit)
{
    QUrlQuery params;
    params.addQueryItem("user_id", QString::number(userId));
    if(offset > 0) params.addQueryItem("offset", QString::number(offset));
    if(limit > 0) params.addQueryItem("limit", QString::number(limit));

    // call api and return constructed data
    return TelegramBotUserProfilePhotos(this->callApiJson("getUserProfilePhotos", params).value("result").toObject());
}

/*
 * Chat Functions
 */
void TelegramBot::kickChatMember(QVariant chatId, qint32 userId, bool *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("user_id", QString::number(userId));

    CallApiTemplate("kickChatMember", params, response);
}

void TelegramBot::unbanChatMember(QVariant chatId, qint32 userId, bool *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("user_id", QString::number(userId));

    CallApiTemplate("unbanChatMember", params, response);
}

void TelegramBot::leaveChat(QVariant chatId, bool *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());

    CallApiTemplate("leaveChat", params, response);
}

TelegramBotChat TelegramBot::getChat(QVariant chatId)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());

    return TelegramBotChat(this->callApiJson("getChat", params).value("result").toObject());
}

QList<TelegramBotChatMember> TelegramBot::getChatAdministrators(const QVariant& chat_id) {
  QUrlQuery params;
  params.addQueryItem(u"chat_id"_s, chat_id.toString());

  QList<TelegramBotChatMember> chat_memebers;
  JsonHelper::PathGet(callApiJson(u"getChatAdministrators"_s, params), u"result"_s, chat_memebers);
  return chat_memebers;
}

int TelegramBot::getChatMembersCount(const QVariant& chat_id) {
  QUrlQuery params;
  params.addQueryItem(u"chat_id"_s, chat_id.toString());
  return callApiJson(u"getChatMembersCount"_s, params).value(u"result"_s).toInt();
}

TelegramBotChatMember TelegramBot::getChatMember(QVariant chatId, qint32 userId)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("user_id", QString::number(userId));

    return TelegramBotChatMember(this->callApiJson("getChatMember", params).value("result").toObject());
}

/*
 * Callback Query Functions
 */
void TelegramBot::answerCallbackQuery(QString callbackQueryId, QString text, bool showAlert, int cacheTime, QString url, bool *response)
{
    QUrlQuery params;
    params.addQueryItem("callback_query_id", callbackQueryId);
    if(!text.isNull()) params.addQueryItem("text", text);
    if(showAlert) params.addQueryItem("show_alert", "true");
    if(!url.isNull()) params.addQueryItem("url", url);
    if(cacheTime > 0) params.addQueryItem("cache_time", QString::number(cacheTime));

    CallApiTemplate("answerCallbackQuery", params, response);
}


QNetworkReply* TelegramBot::SendMessage(const QVariant& chat_id, const QString& text, int reply_to_message_id, TelegramFlags flags,
                                        const TelegramKeyboardRequest& keyboard) {
  QUrlQuery params;
  params.addQueryItem(u"chat_id"_s, chat_id.toString());
  params.addQueryItem(u"text"_s, text);
  if (flags && TelegramFlags::Markdown) params.addQueryItem(u"parse_mode"_s, u"Markdown"_s);
  else if (flags && TelegramFlags::Html) params.addQueryItem(u"parse_mode"_s, u"HTML"_s);
  if (flags && TelegramFlags::DisableWebPagePreview) params.addQueryItem(u"disable_web_page_preview"_s, u"true"_s);
  if (flags && TelegramFlags::DisableNotfication) params.addQueryItem(u"disable_notification"_s, u"true"_s);
  if (reply_to_message_id) params.addQueryItem(u"reply_to_message_id"_s, QString::number(reply_to_message_id));

  HandleReplyMarkup(params, flags, keyboard);
  QNetworkReply* reply = callApi(u"sendMessage"_s, params, false);
  connect(reply, &QNetworkReply::finished, this,
          [reply, flags] () {
            if (!(flags & TelegramFlags::NotDeleteOnFinish)) {
              reply->deleteLater();
            }

            if (reply->error() != QNetworkReply::NoError) {
              qCritical() << "SendMessage failed, params: " << reply->error() << reply->url();
            }
          });
  return reply;
}

void TelegramBot::editMessageText(QVariant chatId, QVariant messageId, QString text, TelegramFlags flags, TelegramKeyboardRequest keyboard, bool *response)
{
    // if we have a null messageId, and user don't request a response, call send Message
    if (!response && messageId.isNull()) {
        SendMessage(chatId, text, 0, flags, keyboard);
        return;
    }

    // determine message id type
    bool isInlineMessageId = messageId.metaType() == QMetaType::fromType<QString>();

    QUrlQuery params;
    if(!isInlineMessageId && !chatId.isNull()) params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem(isInlineMessageId ? "inline_message_id" : "message_id", messageId.toString());
    params.addQueryItem("text", text);
    if(flags && TelegramFlags::Markdown) params.addQueryItem("parse_mode", "Markdown");
    else if(flags && TelegramFlags::Html) params.addQueryItem("parse_mode", "HTML");
    if(flags && TelegramFlags::DisableWebPagePreview) params.addQueryItem("disable_web_page_preview", "true");

    if(!(flags && TelegramFlags::ReplyKeyboardMarkup) && !(flags && TelegramFlags::ForceReply) && !(flags && TelegramFlags::ReplyKeyboardRemove)) {
      HandleReplyMarkup(params, flags, keyboard);
    }
    CallApiTemplate("editMessageText", params, response);
}

void TelegramBot::editMessageCaption(QVariant chatId, QVariant messageId, QString caption, TelegramKeyboardRequest keyboard, bool *response)
{
    // determine message id type
    bool isInlineMessageId = messageId.metaType() == QMetaType::fromType<QString>();

    QUrlQuery params;
    if(!isInlineMessageId && !chatId.isNull()) params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem(isInlineMessageId ? "inline_message_id" : "message_id", messageId.toString());
    if(!caption.isNull()) params.addQueryItem("caption", caption);

    HandleReplyMarkup(params, TelegramFlags(), keyboard);
    CallApiTemplate("editMessageCaption", params, response);
}

void TelegramBot::editMessageReplyMarkup(QVariant chatId, QVariant messageId, TelegramKeyboardRequest keyboard, bool *response)
{
    // determine message id type
    bool isInlineMessageId = messageId.metaType() == QMetaType::fromType<QString>();

    QUrlQuery params;
    if(!isInlineMessageId && !chatId.isNull()) params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem(isInlineMessageId ? "inline_message_id" : "message_id", messageId.toString());

    HandleReplyMarkup(params, TelegramFlags(), keyboard);
    CallApiTemplate("editMessageReplyMarkup", params, response);
}

void TelegramBot::forwardMessage(QVariant targetChatId, QVariant fromChatId, qint32 fromMessageId, TelegramFlags flags, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", targetChatId.toString());
    params.addQueryItem("from_chat_id", fromChatId.toString());
    params.addQueryItem("message_id", QString::number(fromMessageId));
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");

    CallApiTemplate("forwardMessage", params, response);
}

void TelegramBot::deleteMessage(QVariant chatId, qint32 messageId, bool *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("message_id", QString::number(messageId));

    CallApiTemplate("deleteMessage", params, response);
}

/*
 * Content Functions
 */
void TelegramBot::sendPhoto(QVariant chatId, QVariant photo, QString caption, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    if(!caption.isNull()) params.addQueryItem("caption", caption);
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    QHttpMultiPart* multiPart = HandleFile("photo", photo, params);
    CallApiTemplate("sendPhoto", params, response, multiPart);
}

QNetworkReply* TelegramBot::SendAudio(const QVariant& chat_id, const QVariant& audio, const QString& caption, const QString& performer,
                                      const QString& title, int duration, int reply_to_message_id, TelegramFlags flags,
                                      TelegramKeyboardRequest keyboard) {
  QUrlQuery params;
  params.addQueryItem(u"chat_id"_s, chat_id.toString());
  if (!caption.isNull()) params.addQueryItem(u"caption"_s, caption);
  if (duration >= 0) params.addQueryItem(u"duration"_s, QString::number(duration));
  if (!performer.isNull()) params.addQueryItem(u"performer"_s, performer);
  if (!title.isNull()) params.addQueryItem(u"title"_s, title);
  if (flags && TelegramFlags::DisableNotfication) params.addQueryItem(u"disable_notification"_s, u"true"_s);
  if (flags && TelegramFlags::Markdown) params.addQueryItem(u"parse_mode"_s, u"Markdown"_s);
  if (reply_to_message_id) params.addQueryItem(u"reply_to_message_id"_s, QString::number(reply_to_message_id));

  HandleReplyMarkup(params, flags, keyboard);
  QNetworkReply* reply = callApi(u"sendAudio"_s, params, false, HandleFile(u"audio"_s, audio, params, nullptr, flags));
  connect(reply, &QNetworkReply::finished, this,
          [this, reply, params, flags, audio] () {
            if (!(flags & TelegramFlags::NotDeleteOnFinish)) {
              reply->deleteLater();
            }
            if (reply->error() != QNetworkReply::NoError) {
              qCritical() << "SendAudio failed, params: " << params.toString() << reply->error() << reply->errorString();
              return;
            }
            if (flags & TelegramFlags::SaveFileIdInCache) {
              const QJsonObject jo_result{QJsonDocument::fromJson(reply->readAll()).object()[u"result"_s].toObject()};
              TelegramBotVoice voice;
              JsonHelper::PathGet(jo_result, u"voice"_qs, voice, false);
              if (voice.IsFill()) {
                cache_of_paths_to_file_id_[audio.toString()] = voice.file_id;
              }
            }
          });
  return reply;
}

void TelegramBot::sendDocument(QVariant chatId, QVariant document, QString caption, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    if(!caption.isNull()) params.addQueryItem("caption", caption);
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    QHttpMultiPart* multiPart = HandleFile("document", document, params);
    CallApiTemplate("sendDocument", params, response, multiPart);
}

void TelegramBot::sendSticker(QVariant chatId, QVariant sticker, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    QHttpMultiPart* multiPart = HandleFile("sticker", sticker, params);
    CallApiTemplate("sendSticker", params, response, multiPart);
}

void TelegramBot::sendVideo(QVariant chatId, QVariant video, QString caption, int duration, int width, int height, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    if(!caption.isNull()) params.addQueryItem("caption", caption);
    if(duration >= 0) params.addQueryItem("duration", QString::number(duration));
    if(width >= 0) params.addQueryItem("width", QString::number(width));
    if(height >= 0) params.addQueryItem("height", QString::number(height));
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    QHttpMultiPart* multiPart = HandleFile("video", video, params);
    CallApiTemplate("sendVideo", params, response, multiPart);
}

void TelegramBot::SendVoice(QVariant chat_id, QVariant voice, QString caption, int duration, int reply_to_message_id, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage* response) {
  QUrlQuery params;
  params.addQueryItem("chat_id", chat_id.toString());
  if (!caption.isNull()) params.addQueryItem("caption", caption);
  if (duration >= 0) params.addQueryItem("duration", QString::number(duration));
  if (flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
  if (flags && TelegramFlags::Markdown) params.addQueryItem(u"parse_mode"_s, u"Markdown"_s);
  if (reply_to_message_id) params.addQueryItem("reply_to_message_id", QString::number(reply_to_message_id));
  
  HandleReplyMarkup(params, flags, keyboard);
  QHttpMultiPart* multiPart = HandleFile("voice", voice, params);
  CallApiTemplate("sendVoice", params, response, multiPart);
}

void TelegramBot::sendVideoNote(QVariant chatId, QVariant videoNote, int length, int duration, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    if(length >= 0) params.addQueryItem("length", QString::number(length));
    if(duration >= 0) params.addQueryItem("duration", QString::number(duration));
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    QHttpMultiPart* multiPart = HandleFile("video_note", videoNote, params);
    CallApiTemplate("sendVideoNote", params, response, multiPart);
}

void TelegramBot::sendLocation(QVariant chatId, double latitude, double longitude, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("latitude", QString::number(latitude));
    params.addQueryItem("longitude", QString::number(longitude));
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    CallApiTemplate("sendLocation", params, response);
}

void TelegramBot::sendVenue(QVariant chatId, double latitude, double longitude, QString title, QString address, QString foursquareId, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("latitude", QString::number(latitude));
    params.addQueryItem("longitude", QString::number(longitude));
    params.addQueryItem("title", title);
    params.addQueryItem("address", address);
    if(!foursquareId.isNull()) params.addQueryItem("foursquare_id", foursquareId);
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);
    CallApiTemplate("sendVenue", params, response);
}

void TelegramBot::sendContact(QVariant chatId, QString phoneNumber, QString firstName, QString lastName, int replyToMessageId, TelegramFlags flags, TelegramKeyboardRequest keyboard, TelegramBotMessage *response)
{
    QUrlQuery params;
    params.addQueryItem("chat_id", chatId.toString());
    params.addQueryItem("phone_number", phoneNumber);
    params.addQueryItem("first_name", firstName);
    if(!lastName.isNull()) params.addQueryItem("last_name", lastName);
    if(flags && TelegramFlags::DisableNotfication) params.addQueryItem("disable_notification", "true");
    if(replyToMessageId) params.addQueryItem("reply_to_message_id", QString::number(replyToMessageId));

    HandleReplyMarkup(params, flags, keyboard);

    CallApiTemplate("sendContact", params, response);
}

void TelegramBot::StartMessagePulling(uint timeout, uint limit, TelegramPollMessageTypes message_types, long offset) {
  // remove webhook
  deleteWebhookResult();

  // build url params
  pull_params_.clear();
  if (offset) pull_params_.addQueryItem(u"offset"_s, QString::number(offset));
  else if (update_id) pull_params_.addQueryItem(u"offset"_s, QString::number(update_id));
  pull_params_.addQueryItem(u"limit"_s,   QString::number(limit));
  pull_params_.addQueryItem(u"timeout"_s, QString::number(timeout));

  // allowed updates
  if (static_cast<int>(message_types) > 0) {
    QStringList allowed_updates;
    if (message_types && TelegramPollMessageTypes::Message) allowed_updates += u"message"_s;
    if (message_types && TelegramPollMessageTypes::EditedMessage) allowed_updates += u"edited_message"_s;
    if (message_types && TelegramPollMessageTypes::ChannelPost) allowed_updates += u"channel_post"_s;
    if (message_types && TelegramPollMessageTypes::EditedChannelPost) allowed_updates += u"edited_channel_post"_s;
    if (message_types && TelegramPollMessageTypes::InlineQuery) allowed_updates += u"inline_query"_s;
    if (message_types && TelegramPollMessageTypes::ChoosenInlineQuery) allowed_updates += u"chosen_inline_result"_s;
    if (message_types && TelegramPollMessageTypes::CallbackQuery) allowed_updates += u"callback_query"_s;
    if (!allowed_updates.isEmpty()) pull_params_.addQueryItem(u"allowed_updates"_s, u"[\""_s + allowed_updates.join(u"\",\""_s) + u"\"]"_s);
  }

  Pull();
}

void TelegramBot::StopMessagePulling(bool instantly) {
  pull_params_.clear();
  if (instantly && reply_pull_) {
    reply_pull_->abort();
    reply_pull_->deleteLater();
    reply_pull_ = nullptr;
  }
}

void TelegramBot::Pull() {
  if (pull_params_.isEmpty()) {
    qInfo() << "Pulling stopped";
    return;
  }

  if (reply_pull_) {
    reply_pull_->deleteLater();
    reply_pull_ = nullptr;
  }

  reply_pull_ = callApi(u"getUpdates"_s, pull_params_, false);
  connect(reply_pull_, &QNetworkReply::finished, this, &TelegramBot::HandlePullResponse);
}

void TelegramBot::HandlePullResponse() {
  reply_pull_->deleteLater();
  if (reply_pull_->error() != QNetworkReply::NoError) {
    qWarning() << "Pull failed, error: " << reply_pull_->error();
    reply_pull_ = nullptr;
    Pull();
    return;
  }
  pull_params_.removeQueryItem(u"offset"_s);
  const QByteArray data = reply_pull_->readAll();
  reply_pull_ = nullptr;
  parseMessage(data);
  if (update_id) pull_params_.addQueryItem(u"offset"_s, QString::number(update_id + 1));
  Pull();
}

/*
 *  Webhook Functions
 */
bool TelegramBot::setHttpServerWebhook(qint16 port, QString pathCert, QString pathPrivateKey, int maxConnections, TelegramPollMessageTypes messageTypes)
{
    // try to acquire httpServer
    HttpServer* httpServer = 0;
    QSslCertificate cert;
    if(this->webHookWebServers.contains(port)) {
        // if existing webhook contains not the same privateKey, inform user and exit
        if(this->webHookWebServers.find(port).value()->isSamePrivateKey(pathPrivateKey)) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - It's not possible to set multiple private keys for one webserver, webhook installation failed...")
        }
        httpServer = this->webHookWebServers.find(port).value();

        // add new cert
        cert = httpServer->addCert(pathCert);
        if(cert.isNull()) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - Cert file %s is invalid, webhook installation failed...", qPrintable(pathCert))
        }
        if(cert.subjectInfo(QSslCertificate::CommonName).isEmpty()) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - Cert don't contain a Common Name (CN), webhook installation failed...");
        }
    }

    // if no webserver exist, create it
    else {
        // create new http server and register it for auto scope deletion if an error occours
        httpServer = new HttpServer;
        QScopedPointer<HttpServer> scopedHttpServer(httpServer);

        // handle certificates
        cert = httpServer->addCert(pathCert);
        if(cert.isNull()) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - Cert file %s is invalid, webhook installation failed...", qPrintable(pathCert))
        }
        if(cert.subjectInfo(QSslCertificate::CommonName).isEmpty()) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - Cert don't contain a Common Name (CN), webhook installation failed...")
        }
        if(!httpServer->setPrivateKey(pathPrivateKey)) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - Private Key file %s is invalid, webhook installation failed...", qPrintable(pathPrivateKey))
        }

        // permit only telegram connections
        httpServer->addWhiteListHostSubnet("149.154.164.0/22");

        // start listener
        if(!httpServer->listen(QHostAddress::Any, port)) {
            EXIT_FAILED("TelegramBot::setHttpServerWebhook - Cannot listen on port %i, webhook installation failed...", port)
        }

        // everything is okay, so register http server
        this->webHookWebServers.insert(port, scopedHttpServer.take());
    }

    // simplify data
    QString host = cert.subjectInfo(QSslCertificate::CommonName).first();

    // add rewrite rule
    httpServer->addRewriteRule(host, "/" + this->apiKey, {this, &TelegramBot::handleServerWebhookResponse});

    // build server webhook request
    QUrlQuery query;
    query.addQueryItem("url", "https://" + host + ":" + QString::number(port) + "/" + this->apiKey);
    if(maxConnections) query.addQueryItem("max_connections", QString::number(maxConnections));

    // allowed updates
    QStringList allowedUpdates;
    if(static_cast<int>(messageTypes) > 0) {
        if(messageTypes && TelegramPollMessageTypes::Message) allowedUpdates += "message";
        if(messageTypes && TelegramPollMessageTypes::EditedMessage) allowedUpdates += "edited_message";
        if(messageTypes && TelegramPollMessageTypes::ChannelPost) allowedUpdates += "channel_post";
        if(messageTypes && TelegramPollMessageTypes::EditedChannelPost) allowedUpdates += "edited_channel_post";
        if(messageTypes && TelegramPollMessageTypes::InlineQuery) allowedUpdates += "inline_query";
        if(messageTypes && TelegramPollMessageTypes::ChoosenInlineQuery) allowedUpdates += "chosen_inline_result";
        if(messageTypes && TelegramPollMessageTypes::CallbackQuery) allowedUpdates += "callback_query";
    }
    if(!allowedUpdates.isEmpty()) query.addQueryItem("allowed_updates", "[\"" + allowedUpdates.join("\",\"") + "\"]");

    // build multipart
    QByteArray certContent = cert.toPem();
    QHttpMultiPart *multiPart = CreateUploadFile("certificate", "cert.pem", certContent);

    // call api
    return this->callApiJson("setWebhook", query, multiPart).value("result").toBool();
}

void TelegramBot::deleteWebhook()
{
    this->callApi("deleteWebhook");
}

TelegramBotOperationResult TelegramBot::deleteWebhookResult()
{
    return TelegramBotOperationResult(this->callApiJson("deleteWebhook"));
}

TelegramBotWebHookInfo TelegramBot::getWebhookInfo()
{
    return TelegramBotWebHookInfo(this->callApiJson("getWebhookInfo").value("result").toObject());
}

/*
 *  Message Router functions
 */
void TelegramBot::messageRouterRegister(QString startWith, QDelegate<bool(TelegramBotUpdate)> delegate, TelegramBotMessageType type)
{
    // save message route
    this->messageRoutes.append(new MessageRoute {
                                   type,
                                   startWith,
                                   delegate
                               });
}

/*
 * Reponse Parser
 */
void TelegramBot::parseMessage(const QByteArray& data, bool singleMessage) {
  qDebug() << "PARSE: " << data;
  QJsonParseError jError;
  const QJsonObject oUpdate = QJsonDocument::fromJson(data, &jError).object();

  if (jError.error != QJsonParseError::NoError) {
    qWarning("TelegramBot::parseMessage - Parse Error: %s", qPrintable(jError.errorString()));
    return;
  }

  if (!singleMessage && !JsonHelper::PathGet(oUpdate, u"ok"_s).toBool()) {
    qWarning("TelegramBot::parseMessage - Receive Error: %i - %s", JsonHelper::PathGet(oUpdate, "error_code").toInt(),
             qPrintable(JsonHelper::PathGet(oUpdate, "description").toString()));
    return;
  }

  // loop results
  for (const QJsonValue& result : singleMessage ? QJsonArray({oUpdate}) : oUpdate.value(u"result"_s).toArray()) {
    const QJsonObject update = result.toObject();

    TelegramBotUpdate update_message(new TelegramBotUpdatePrivate);
    if (update_message->FromJson(update)) {
      update_id = update_message->update_id;

      Q_EMIT NewMessage(update_message);

      const QString route_data = update_message->inlineQuery        ? update_message->inlineQuery->query :
                                 update_message->chosenInlineResult ? update_message->chosenInlineResult->query :
                                 update_message->callbackQuery      ? update_message->callbackQuery->data :
                                 update_message->message            ? update_message->message->text : QString();
      if (route_data.isNull()) continue;
      for (auto itrRoute = messageRoutes.begin(); itrRoute != messageRoutes.end(); ++itrRoute) {
        MessageRoute* route = *itrRoute;
        if (route->type && update_message->type != update_message->type) continue;
        if (!route_data.startsWith(route->startWith)) continue;
        if (!route->delegate.invoke(update_message).first()) break;
      }
    } else {
      qCritical() << "Failed to parse update message";
    }
  }
}

void TelegramBot::handleServerWebhookResponse(HttpServerRequest request, HttpServerResponse response) {
  parseMessage(request->content, true);

  // reply to server with status OK
  response->status = HttpServerResponsePrivate::OK;
}


/*
 * Call Api Helpers
 */
template<typename T>
typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value>::type
TelegramBot::CallApiTemplate(QString method, QUrlQuery params, T* response, QHttpMultiPart* multiPart) {
  // if no response was provided, just call the api
  if (!response) {
    callApi(method, params, true, multiPart);
    return;
  }

  response->FromJson(QJsonObject(callApiJson(method, params, multiPart)).value("result").toObject());
}

template<typename T>
typename std::enable_if<!std::is_base_of<TelegramBotObject, T>::value>::type
TelegramBot::CallApiTemplate(QString method, QUrlQuery params, T* response, QHttpMultiPart* multiPart) {
  // if no response was provided, just call the api
  if (!response) {
    callApi(method, params, true, multiPart);
    return;
  }

  // get result and (if possible) convert it to T
  const QVariant result = QJsonObject(callApiJson(method, params, multiPart)).value("result").toVariant();
  if (result.canConvert<T>()) *response = result.value<T>();
  return;
}


QNetworkReply* TelegramBot::callApi(QString method, QUrlQuery params, bool deleteOnFinish, QHttpMultiPart* multiPart)
{
    // build url
    QUrl url(QString("https://api.telegram.org/bot%1/%2").arg(this->apiKey, method));
    url.setQuery(params);

    // execute
    QNetworkRequest request(url);
    QNetworkReply* reply = multiPart ? this->aManager.post(request, multiPart) : this->aManager.get(request);
    if (multiPart) multiPart->setParent(reply);
    if (deleteOnFinish) QObject::connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    return reply;
}

QJsonObject TelegramBot::callApiJson(QString method, QUrlQuery params, QHttpMultiPart *multiPart) {
  // exec request
  QNetworkReply* reply = callApi(method, params, true, multiPart);

  // wait async for answer
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  // parse answer
  return QJsonDocument::fromJson(reply->readAll()).object();
}

QHttpMultiPart* TelegramBot::CreateUploadFile(const QString& name, const QString& file_name, const QByteArray& content, bool detect_mime_type,
                                              QHttpMultiPart* multi_part) {
  if (!multi_part) multi_part = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QHttpPart content_part;
  content_part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"; filename=\"%2\"").arg(name, file_name));
  content_part.setHeader(QNetworkRequest::ContentTypeHeader, detect_mime_type ? QMimeDatabase().mimeTypeForData(content).name() : QString("application/octet-stream"));
  content_part.setBody(content);
  multi_part->append(content_part);
  
  return multi_part;
}

void TelegramBot::HandleReplyMarkup(QUrlQuery& params, TelegramFlags flags, const TelegramKeyboardRequest &keyboard) {
  QString reply_markup;
  if (flags && TelegramFlags::ForceReply) {
    reply_markup = u"{\"force_reply\":true"_s;
    if (flags && TelegramFlags::Selective) reply_markup += u",\"selective\":true"_s;
    reply_markup += u"}"_s;
  } else if (flags && TelegramFlags::ReplyKeyboardRemove) {
    reply_markup = u"{\"remove_keyboard\":true"_s;
      if(flags && TelegramFlags::Selective) reply_markup += u",\"selective\":true"_s;
      reply_markup += u"}"_s;
  } else if (!keyboard.isEmpty()) {
    QString keyboard_content = u"["_s;
    bool first_row = true;
    for (const QList<TelegramBotKeyboardButtonRequest>& row : keyboard) {
      if (!first_row) keyboard_content += u","_s;
      keyboard_content += u"["_s;
      bool first_column = true;
      for (const TelegramBotKeyboardButtonRequest& column : row) {
        keyboard_content += u"%1{\"text\":\"%2\""_s.arg(first_column ? u""_s : u","_s, column.text);
        if (flags && TelegramFlags::ReplyKeyboardMarkup) {
          if (column.requestContact) keyboard_content += u",\"request_contact\":true"_s;
          if (column.requestLocation) keyboard_content += u",\"request_location\":true"_s;
        } else {
          if (!column.url.isEmpty()) keyboard_content += u",\"url\":\"%1\""_s.arg(column.url);
          if (!column.callbackData.isEmpty()) keyboard_content += u",\"callback_data\":\"%1\""_s.arg(column.callbackData);
          if (!column.switchInlineQuery.isEmpty()) keyboard_content += u",\"switch_inline_query\":\"%1\""_s.arg(column.switchInlineQuery);
          if (!column.switchInlineQueryCurrentChat.isEmpty()) keyboard_content += u",\"switch_inline_query_current_chat\":\"%1\""_s.arg(column.switchInlineQueryCurrentChat);
        }
        keyboard_content += u"}"_s;
        first_column = false;
      }
      keyboard_content += u"]"_s;
      first_row = false;
    }
    keyboard_content += u"]"_s;

    if (flags && TelegramFlags::ReplyKeyboardMarkup) {
      reply_markup += u"{\"keyboard\":"_s + keyboard_content;
      if (flags && TelegramFlags::ResizeKeyboard) reply_markup += u",\"resize_keyboard\":true"_s;
      if (flags && TelegramFlags::OneTimeKeyboard) reply_markup += u",\"one_time_keyboard\":true"_s;
      if (flags && TelegramFlags::Selective) reply_markup += u",\"selective\":true"_s;
    } else {
      reply_markup += u"{\"inline_keyboard\":"_s + keyboard_content;
    }
    reply_markup += u"}"_s;
  }

  if (!reply_markup.isEmpty()) params.addQueryItem(u"reply_markup"_s, reply_markup);
}

QHttpMultiPart* TelegramBot::HandleFile(const QString& field_name, const QVariant& file, QUrlQuery& params, QHttpMultiPart* multi_part, TelegramFlags flags) {
  if (file.metaType() == QMetaType::fromType<QByteArray>()) {
    const QByteArray content = file.value<QByteArray>();
    multi_part = CreateUploadFile(field_name, field_name, content, true, multi_part);
  } else if (file.metaType() == QMetaType::fromType<QString>()) {
    const QString file_str{file.toString()};
    const QUrl url = QUrl::fromUserInput(file_str);
    if (url.isLocalFile() || url.isRelative()) {
      if (flags & TelegramFlags::SaveFileIdInCache) {
        auto it_file = cache_of_paths_to_file_id_.find(file_str);
        if (it_file != cache_of_paths_to_file_id_.end()) {
          params.addQueryItem(field_name, it_file.value());
          return multi_part;
        }
      }
      QFile in_file(file_str);
      if (!in_file.open(QFile::ReadOnly)) {
        qCritical() << "Failed to open file: " << file_str;
        return multi_part;
      }
      multi_part = CreateUploadFile(field_name, QFileInfo(in_file).fileName(), in_file.readAll(), true, multi_part);
    } else {
      params.addQueryItem(field_name, file.toString());
    }
  } else {
    params.addQueryItem(field_name, file.toString());
  }
  
  return multi_part;
}
