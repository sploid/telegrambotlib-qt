#pragma once

#include "jsonhelper.h"
#include "telegramdatastructs.h"

#include "httpserver.h"

class OutgoingMessage;

// Helper Macros
#define EXIT_FAILED(...) qWarning(__VA_ARGS__); return false;

class BOT_EXPORT TelegramBot : public QObject {
  Q_OBJECT
public:
  enum TelegramPollMessageTypes {
    All                 = 0,
    Message             = 1 << 0,
    EditedMessage       = 1 << 1,
    ChannelPost         = 1 << 2,
    EditedChannelPost   = 1 << 3,
    InlineQuery         = 1 << 4,
    ChoosenInlineQuery  = 1 << 5,
    CallbackQuery       = 1 << 6
  };

  enum TelegramFlags {
    NoFlag                       = 0,

    // Message
    Markdown                     = 1 << 0,
    Html                         = 1 << 1,
    DisableWebPagePreview        = 1 << 2,
    DisableNotfication           = 1 << 3,

    // Keyboard Global
    Selective                    = 1 << 4,

    // ReplyKeyboardMarkup
    ReplyKeyboardMarkup          = 1 << 5,
    ResizeKeyboard               = 1 << 6,
    OneTimeKeyboard              = 1 << 7,

    // ReplyKeyboardRemove
    ReplyKeyboardRemove          = 1 << 8,

    // ForceReply
    ForceReply                   = 1 << 9,

    SaveFileIdInCache            = 1 << 10
  };

  // Keyboard construction helpers
  static TelegramBotKeyboardButtonRequest ConstructTextButton(const QString& text, bool request_contact = false,
                                                              bool request_location = false) {
    return TelegramBotKeyboardButtonRequest{.text = text, .requestContact = request_contact, .requestLocation = request_location};
  }
  static TelegramBotKeyboardButtonRequest ConstructInlineButton(const QString& text, const QString& callback_data,
                                                                const QString& url = {}, const QString& inline_query_data = {},
                                                                const QString& inline_query_data_current_chat = {}) {
    return TelegramBotKeyboardButtonRequest{.text = text,
                                            .url = url,
                                            .callbackData = callback_data,
                                            .switchInlineQuery = inline_query_data,
                                            .switchInlineQueryCurrentChat = inline_query_data_current_chat};
  }
  static TelegramKeyboardRequest ConstructInlineMenu(const QList<std::tuple<QString, QString>>& menu);

  TelegramBot(QString apikey, QObject* parent);
  ~TelegramBot();

  void sendChatAction(QVariant chatId, TelegramBotChatAction action);
  void sendChatAction(QVariant chatId, QString action);

  // User Functions
  TelegramBotUserProfilePhotos getUserProfilePhotos(qint32 userId, int offset = 0, int limit = 0);

  // Chat Functions
  void kickChatMember(QVariant chatId, qint32 userId);
  void unbanChatMember(QVariant chatId, qint32 userId);
  void leaveChat(QVariant chatId);
  TelegramBotChat getChat(QVariant chatId);
  QList<TelegramBotChatMember> getChatAdministrators(const QVariant& chat_id);
  int getChatMembersCount(const QVariant& chat_id);
  TelegramBotChatMember getChatMember(QVariant chatId, qint32 userId);

  void AnswerCallbackQuery(const QString& callback_query_id, const QString& text = QString(), bool show_alert = false, int cache_time = 0, const QString& url = QString());

  OutgoingMessage* SendMessage(qint64 chat_id, const QString& text, int reply_to_message_id = 0, TelegramFlags flags = TelegramFlags::NoFlag,
                               const TelegramKeyboardRequest& keyboard = TelegramKeyboardRequest());
  void editMessageText(QVariant chatId, QVariant messageId, QString text, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void editMessageCaption(QVariant chatId, QVariant messageId, QString caption = QString(), TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void editMessageReplyMarkup(QVariant chatId, QVariant messageId, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void forwardMessage(QVariant targetChatId, QVariant fromChatId, qint32 fromMessageId, TelegramFlags flags = TelegramFlags::NoFlag);
  void deleteMessage(QVariant chatId, qint32 messageId);

  // Content Functions
  void sendPhoto(QVariant chatId, QVariant photo, QString caption = QString(), int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  OutgoingMessage* SendAudio(qint64 chat_id, const QVariant& audio, const QString& caption = QString(), const QString& performer = QString(), const QString& title = QString(), int duration = -1, int reply_to_messageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendDocument(QVariant chatId, QVariant document, QString caption = QString(), int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendSticker(QVariant chatId, QVariant sticker, int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendVideo(QVariant chatId, QVariant video, QString caption = QString(), int duration = -1, int width = -1, int height = -1, int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void SendVoice(QVariant chat_id, QVariant voice, QString caption = QString(), int duration = -1, int reply_to_message_id = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendVideoNote(QVariant chatId, QVariant videoNote, int length = -1, int duration = -1, int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendLocation(QVariant chatId, double latitude, double longitude, int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendVenue(QVariant chatId, double latitude, double longitude, QString title, QString address, QString foursquareId = QString(), int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());
  void sendContact(QVariant chatId, QString phoneNumber, QString firstName, QString lastName = QString(), int replyToMessageId = 0, TelegramFlags flags = TelegramFlags::NoFlag, TelegramKeyboardRequest keyboard = TelegramKeyboardRequest());

  // Message Puller
  void StartMessagePulling(uint timeout = 10, uint limit = 100, TelegramPollMessageTypes messageTypes = TelegramPollMessageTypes::All, long offset = 0);
  void StopMessagePulling(bool instantly = false);

  // Webhook Functions
  bool setHttpServerWebhook(qint16 port, QString pathCert, QString pathPrivateKey, int maxConnections = 10, TelegramPollMessageTypes messageTypes = TelegramPollMessageTypes::All);
  void deleteWebhook();
  TelegramBotWebHookInfo getWebhookInfo();

  // Message Router functions
  void messageRouterRegister(QString startWith, QDelegate<bool(TelegramBotUpdate)> delegate, TelegramBotMessageType type = TelegramBotMessageType::All);

Q_SIGNALS:
  void NewMessage(TelegramBotUpdate message, const QByteArray& data);

private:
  void Pull();
  void HandlePullResponse();

  void parseMessage(const QByteArray& data, bool single_message = false);

  // webhook functions
  void handleServerWebhookResponse(HttpServerRequest request, HttpServerResponse response);

  QNetworkReply* CallApi(const QString& method, const QUrlQuery& params = QUrlQuery(), bool delete_on_finish = true, QHttpMultiPart* multi_part = 0);
  OutgoingMessage* CreateOutgoingMessage(const std::optional<qint64>& chat_id, const QString& method, const QUrlQuery& params = QUrlQuery(), QHttpMultiPart* multi_part = 0);
  void SendOutgoingMessage(OutgoingMessage* om);

  // helpers
  QHttpMultiPart* CreateUploadFile(const QString& name, const QString& file_name, const QByteArray& content, bool detect_mime_type = false, QHttpMultiPart* multi_part = 0);
  void HandleReplyMarkup(QUrlQuery& params, TelegramFlags flags, const TelegramKeyboardRequest& keyboard);
  QHttpMultiPart* HandleFile(const QString& fieldName, const QVariant& file, QUrlQuery& params, QHttpMultiPart* multiPart = 0, TelegramFlags flags = TelegramFlags::NoFlag);

  // global data
  QNetworkAccessManager aManager;
  QString apiKey;
  qint64 update_id{0};

  // message puller
  QNetworkReply* reply_pull_{nullptr};
  QTimer* pull_freeze_guard_{};
  QUrlQuery pull_params_;

  // httpserver webhook
  static QMap<qint16, HttpServer*> webHookWebServers;

  // message router
  struct MessageRoute
  {
      TelegramBotMessageType type;
      QString startWith;
      QDelegate<bool(TelegramBotUpdate)> delegate;
  };
  QList<MessageRoute*> messageRoutes;
  QHash<QString, QString> cache_of_paths_to_file_id_;
  QHash<qint64, QQueue<OutgoingMessage*>> queue_of_mess_to_send_in_chat_;
};

/*
 * TelegramPollMessageTypes - operators
 */
inline TelegramBot::TelegramPollMessageTypes operator|(TelegramBot::TelegramPollMessageTypes a, TelegramBot::TelegramPollMessageTypes b)
{return static_cast<TelegramBot::TelegramPollMessageTypes>(static_cast<int>(a) | static_cast<int>(b));}

// WARNING: override standard behavior: operator && returns true if bit is set, and not if complete equal!
// Note: this improves code readibility a lot!
inline bool operator&&(TelegramBot::TelegramPollMessageTypes a, TelegramBot::TelegramPollMessageTypes b)
{return (static_cast<int>(a) & static_cast<int>(b)) == static_cast<int>(b);}

/*
 * TelegramFlags - operators
 */
inline TelegramBot::TelegramFlags operator|(TelegramBot::TelegramFlags a, TelegramBot::TelegramFlags b)
{return static_cast<TelegramBot::TelegramFlags>(static_cast<int>(a) | static_cast<int>(b));}

// WARNING: override standard behavior: operator && returns true if bit is set, and not if complete equal!
// Note: this improves code readibility a lot!
inline bool operator&&(TelegramBot::TelegramFlags a, TelegramBot::TelegramFlags b)
{return (static_cast<int>(a) & static_cast<int>(b)) == static_cast<int>(b);}
