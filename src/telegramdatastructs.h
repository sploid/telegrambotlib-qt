#pragma once

#include "jsonhelper.h"
#include "telegramdatainterface.h"

#define TELEGRAMBOTKEYBOARD_FIELDS \
  /* global */ \
  QString text; \
   \
  /* inline keyboard */ \
  QString url; \
  QString callbackData; \
  QString switchInlineQuery; \
  QString switchInlineQueryCurrentChat; \
   \
  /* normal keyboard */ \
  bool requestContact{false}; \
  bool requestLocation{false};

struct TelegramBotKeyboardButtonRequest {
  TELEGRAMBOTKEYBOARD_FIELDS
};
typedef QList<QList<TelegramBotKeyboardButtonRequest>> TelegramKeyboardRequest;

/*
 *  Telegram System Data Structs
 */

// TelegramChatAction - This object represents all possible Teletram chat actions
enum class TelegramBotChatAction {
  Typing,
  UploadPhoto,
  RecordVideo,
  UploadVideo,
  RecordAudio,
  UploadAudio,
  UploadDocument,
  FindLocation,
  RecordVideoNote,
  UploadVideoNote
};

enum TelegramBotMessageType {
  Undefined           = 0,
  Message             = 1 << 0,
  EditedMessage       = 1 << 1,
  ChannelPost         = 1 << 2,
  EditedChannelPost   = 1 << 3,
  InlineQuery         = 1 << 4,
  ChosenInlineResult  = 1 << 5,
  CallbackQuery       = 1 << 6,
  All                 = (2^7) - 1
};

// TelegramBotOperationResult - This object represents a Telegram Operation result
struct TelegramBotOperationResult : public TelegramBotObject {
  bool result{};
  qint32 errorCode{0};
  QString description;

  TelegramBotOperationResult(const QJsonObject& object) { FromJson(object); }

  // parse logic
  void FromJson(const QJsonObject& object) override {
    JsonHelperT<bool>::PathGet(object, u"result"_qs, result);
    JsonHelperT<qint32>::PathGet(object, u"error_code"_qs, errorCode, false);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description);
  }
};

// TelegramBotWebHookInfo - This object represents Telegram Webhook Information
struct TelegramBotWebHookInfo : public TelegramBotObject {
  QString url;
  bool hasCustomCertificate;
  bool pendingUpdateCount;
  qint32 lastErrorDate;
  QString lastErrorMessage;
  qint32 maxConnections;
  QList<QString> allowedUpdates;

  TelegramBotWebHookInfo(QJsonObject object) { FromJson(object); }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"url"_qs, url);
    JsonHelperT<bool>::PathGet(object, u"has_custom_certificate"_qs, hasCustomCertificate);
    JsonHelperT<bool>::PathGet(object, u"pending_update_count"_qs, pendingUpdateCount);
    JsonHelperT<qint32>::PathGet(object, u"last_error_date"_qs, lastErrorDate, false);
    JsonHelperT<QString>::PathGet(object, u"last_error_message"_qs, lastErrorMessage, false);
    JsonHelperT<qint32>::PathGet(object, u"max_connections"_qs, maxConnections, false);
    JsonHelperT<QString>::jsonPathGetArray(object, u"allowed_updates"_qs, allowedUpdates, false);
  }
};

/*
 *  Telegram Game Data Structs
 */

// TelegramBotUser - This object represents a Telegram user or bot.
struct TelegramBotUser : public TelegramBotObject {
  qint32 id; // Unique identifier for this user or bot
  bool is_bot{false};
  QString first_name; // User‘s or bot’s first name
  std::optional<QString> last_name; // Optional. User‘s or bot’s last name
  std::optional<QString> username; // Optional. User‘s or bot’s username
  std::optional<QString> language_code; // Optional. User‘s or bot’s locale

  TelegramBotUser() { }
  TelegramBotUser(QJsonObject object) { FromJson(object); }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<qint32>::PathGet(object, u"id"_qs, id);
    JsonHelperT<bool>::PathGet(object, u"is_bot"_qs, is_bot);
    JsonHelperT<QString>::PathGet(object, u"first_name"_qs, first_name);
    JsonHelperT<QString>::PathGet(object, u"last_name"_qs, last_name, false);
    JsonHelperT<QString>::PathGet(object, u"username"_qs, username, false);
    JsonHelperT<QString>::PathGet(object, u"language_code"_qs, language_code, false);
  }
};

// TelegramBotMessageEntity - This object represents one special entity in a text message. For example, hashtags, usernames, URLs, etc.
struct TelegramBotMessageEntity : public TelegramBotObject {
  QString type; // Type of the entity. Can be mention ((at)username), hashtag, bot_command, url, email, bold (bold text), italic (italic text), code (monowidth string), pre (monowidth block), text_link (for clickable text URLs), text_mention (for users without usernames)
  qint32 offset; // Offset in UTF-16 code units to the start of the entity
  qint32 length; // Length of the entity in UTF-16 code units
  QString url; // Optional. For “text_link” only, url that will be opened after user taps on the text
  TelegramBotUser user; // Optional. For “text_mention” only, the mentioned user
  bool IsCommand() const { return type.compare(u"bot_command"_qs, Qt::CaseInsensitive) == 0; }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<qint32>::PathGet(object, u"offset"_qs, offset);
    JsonHelperT<qint32>::PathGet(object, u"length"_qs, length);
    JsonHelperT<QString>::PathGet(object, u"url"_qs, url, false);
    JsonHelperT<TelegramBotUser>::PathGet(object, u"user"_qs, user, false);
  }
};

// TelegramBotPhotoSize - This object represents one size of a photo or a file / sticker thumbnail.
struct TelegramBotPhotoSize : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 width; // Photo width
  qint32 height; // Photo height
  qint32 fileSize; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<qint32>::PathGet(object, u"width"_qs, width);
    JsonHelperT<qint32>::PathGet(object, u"height"_qs, height);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// You can provide an animation for your game so that it looks stylish in chats (check out Lumberjack for an example). This object represents an animation file to be displayed in the message containing a game.
struct TelegramBotAnimation : public TelegramBotObject {
  QString fileId; // Unique file identifier
  TelegramBotPhotoSize thumb; // Optional. Animation thumbnail as defined by sender
  QString fileName; // Optional. Original animation filename as defined by sender
  QString mimeType; // Optional. MIME type of the file as defined by sender
  qint32 fileSize; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<TelegramBotPhotoSize>::PathGet(object, u"thumb"_qs, thumb, false);
    JsonHelperT<QString>::PathGet(object, u"file_name"_qs, fileName, false);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType, false);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// This object represents a game. Use BotFather to create and edit games, their short names will act as unique identifiers.
struct TelegramBotGame : public TelegramBotObject {
  QString title; // Title of the game
  QString description; // Description of the game
  QList<TelegramBotPhotoSize> photo; // Photo that will be displayed in the game message in chats.
  QString text; // Optional. Brief description of the game or high scores included in the game message. Can be automatically edited to include current high scores for the game when the bot calls setGameScore, or manually edited using editMessageText. 0-4096 characters.
  QList<TelegramBotMessageEntity> textEntities; // Optional. Special entities that appear in text, such as usernames, URLs, bot commands, etc.
  TelegramBotAnimation animation; // Optional. Animation that will be displayed in the game message in chats. Upload via BotFather

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description);
    JsonHelperT<TelegramBotPhotoSize>::jsonPathGetArray(object, u"photo"_qs, photo);
    JsonHelperT<QString>::PathGet(object, u"text"_qs, text, false);
    JsonHelperT<TelegramBotMessageEntity>::jsonPathGetArray(object, u"text_entities"_qs, textEntities, false);
    JsonHelperT<TelegramBotAnimation>::PathGet(object, u"animation"_qs, animation, false);
  }
};

// This object represents one row of the high scores table for a game.
struct TelegramBotGameHighScore : public TelegramBotObject {
  qint32 position{}; // Position in high score table for the game
  TelegramBotUser user;
  qint32 score{};

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<qint32>::PathGet(object, u"position"_qs, position);
    JsonHelperT<TelegramBotUser>::PathGet(object, u"user"_qs, user);
    JsonHelperT<qint32>::PathGet(object, u"score"_qs, score);
  }
};

/*
 *  Telegram Base Data Structs
 */

struct TelegramBotKeyboardButton : public TelegramBotObject {
  TELEGRAMBOTKEYBOARD_FIELDS
  TelegramBotGame callbackGame;

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"text"_qs, text);
    JsonHelperT<QString>::PathGet(object, u"url"_qs, url, false);
    JsonHelperT<QString>::PathGet(object, u"callback_data"_qs, callbackData, false);
    JsonHelperT<QString>::PathGet(object, u"switch_inline_query"_qs, switchInlineQuery, false);
    JsonHelperT<QString>::PathGet(object, u"switch_inline_query_current_chat"_qs, switchInlineQueryCurrentChat, false);
    JsonHelperT<TelegramBotGame>::PathGet(object, u"callback_game"_qs, callbackGame, false);
    JsonHelperT<bool>::PathGet(object, u"request_contact"_qs, requestContact, false);
    JsonHelperT<bool>::PathGet(object, u"request_location"_qs, requestLocation, false);
  }
};
typedef QList<QList<TelegramBotKeyboardButton>> TelegramKeyboard;

// TelegramBotChat - This object represents a chat.
struct TelegramBotChat : public TelegramBotObject {
  qint64  id; // Unique identifier for this chat. This number from now can be signed 64 bit integer or double-precision float type are safe for storing this identifier.
  QString type; // Type of chat, can be either “private”, “group”, “supergroup” or “channel”
  QString title; // Optional. Title, for supergroups, channels and group chats
  QString username; // Optional. Username, for private chats, supergroups and channels if available
  QString firstName; // Optional. First name of the other party in a private chat
  QString lastName; // Optional. Last name of the other party in a private chat
  bool allMembersAreAdministrators; // Optional. True if a group has ‘All Members Are Admins’ enabled.

  TelegramBotChat() {}
  TelegramBotChat(QJsonObject object) { FromJson(object); }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<qint64>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"username"_qs, username, false);
    JsonHelperT<QString>::PathGet(object, u"first_name"_qs, firstName, false);
    JsonHelperT<QString>::PathGet(object, u"last_name"_qs, lastName, false);
    JsonHelperT<bool>::PathGet(object, u"all_members_are_administrators"_qs, allMembersAreAdministrators, false);
  }
};

// TelegramBotAudio - This object represents an audio file to be treated as music by the Telegram clients.
struct TelegramBotAudio : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 duration{}; // Duration of the audio in seconds as defined by sender
  QString performer; // Optional. Performer of the audio as defined by sender or by audio tags
  QString title; // Optional. Title of the audio as defined by sender or by audio tags
  QString mimeType; // Optional. MIME type of the file as defined by sender
  qint32 fileSize{}; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<qint32>::PathGet(object, u"duration"_qs, duration);
    JsonHelperT<QString>::PathGet(object, u"performer"_qs, performer, false);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType, false);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// TelegramBotDocument - This object represents a general file (as opposed to photos, voice messages and audio files).
struct TelegramBotDocument : public TelegramBotObject {
  QString fileId; // Unique file identifier
  TelegramBotPhotoSize thumb; // Optional. Document thumbnail as defined by sender
  QString fileName; // Optional. Original filename as defined by sender
  QString mimeType; // Optional. MIME type of the file as defined by sender
  qint32 fileSize{}; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<TelegramBotPhotoSize>::PathGet(object, u"thumb"_qs, thumb, false);
    JsonHelperT<QString>::PathGet(object, u"file_name"_qs, fileName, false);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType, false);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// TelegramBotSticker - This object represents a sticker.
struct TelegramBotSticker : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 width{}; // Sticker width
  qint32 height{}; // Sticker height
  TelegramBotPhotoSize thumb; // Optional. Sticker thumbnail in .webp or .jpg format
  QString emoji; // Optional. Emoji associated with the sticker
  qint32 fileSize{}; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<qint32>::PathGet(object, u"width"_qs, width);
    JsonHelperT<qint32>::PathGet(object, u"height"_qs, height);
    JsonHelperT<TelegramBotPhotoSize>::PathGet(object, u"thumb"_qs, thumb, false);
    JsonHelperT<QString>::PathGet(object, u"emoji"_qs, emoji, false);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// TelegramBotVideo - This object represents a video file.
struct TelegramBotVideo : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 width{}; // Video width as defined by sender
  qint32 height{}; // Video height as defined by sender
  qint32 duration{}; // Duration of the video in seconds as defined by sender
  TelegramBotPhotoSize thumb; // Optional. Video thumbnail
  QString mimeType; // Optional. Mime type of a file as defined by sender
  qint32 fileSize{}; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<qint32>::PathGet(object, u"width"_qs, width);
    JsonHelperT<qint32>::PathGet(object, u"height"_qs, height);
    JsonHelperT<qint32>::PathGet(object, u"duration"_qs, duration);
    JsonHelperT<TelegramBotPhotoSize>::PathGet(object, u"thumb"_qs, thumb, false);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType, false);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// TelegramBotVoice - This object represents a voice note.
struct TelegramBotVoice : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 duration{}; // Duration of the audio in seconds as defined by sender
  QString mimeType; // Optional. MIME type of the file as defined by sender
  qint32 fileSize{}; // Optional. File size

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<qint32>::PathGet(object, u"duration"_qs, duration);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType, false);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
  }
};

// TelegramBotContact - This object represents a phone contact.
struct TelegramBotContact : public TelegramBotObject {
  QString phoneNumber; // Contact's phone number
  QString firstName; // Contact's first name
  QString lastName; // Optional. Contact's last name
  qint32 userId{}; // Optional. Contact's user identifier in Telegram

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"phone_number"_qs, phoneNumber);
    JsonHelperT<QString>::PathGet(object, u"first_name"_qs, firstName);
    JsonHelperT<QString>::PathGet(object, u"last_name"_qs, lastName, false);
    JsonHelperT<qint32>::PathGet(object, u"user_id"_qs, userId, false);
  }
};

// TelegramBotLocation - This object represents a point on the map.
struct TelegramBotLocation : public TelegramBotObject {
  double longitude; // Longitude as defined by sender
  double latitude; // Latitude as defined by sender

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<double>::PathGet(object, u"longitude"_qs, longitude);
    JsonHelperT<double>::PathGet(object, u"latitude"_qs, latitude);
  }
};

// TelegramBotVenue - This object represents a venue.
struct TelegramBotVenue : public TelegramBotObject {
  TelegramBotLocation location; // Venue location
  QString title; // Name of the venue
  QString address; // Address of the venue
  QString foursquareId; // Optional. Foursquare identifier of the venue

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<TelegramBotLocation>::PathGet(object, u"location"_qs, location);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"address"_qs, address);
    JsonHelperT<QString>::PathGet(object, u"foursquare_id"_qs, foursquareId, false);
  }
};

// This object represent a user's profile pictures.
struct TelegramBotUserProfilePhotos : public TelegramBotObject {
  qint32 totalCount{}; // Total number of profile pictures the target user has
  QList<QList<TelegramBotPhotoSize>> photos; // Requested profile pictures (in up to 4 sizes each)

  TelegramBotUserProfilePhotos(QJsonObject object) { FromJson(object); }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<qint32>::PathGet(object, u"total_count"_qs, totalCount);
    JsonHelperT<TelegramBotPhotoSize>::jsonPathGetArrayArray(object, u"photos"_qs, photos);
  }
};

// TelegramBotFile - This object represents a file ready to be downloaded. The file can be downloaded via the link https://api.telegram.org/file/bot<token>/<file_path>. It is guaranteed that the link will be valid for at least 1 hour. When the link expires, a new one can be requested by calling getFile. Maximum file size to download is 20 MB
struct TelegramBotFile : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 fileSize{}; // Optional. File size, if known
  QString filePath; // Optional. File path. Use https://api.telegram.org/file/bot<token>/<file_path> to get the file.
  QString link; // Generated Absolute link to picture
  QDateTime validUntil; // Genetated valid time

  TelegramBotFile(QJsonObject object) { FromJson(object); }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"file_id"_qs, fileId);
    JsonHelperT<qint32>::PathGet(object, u"file_size"_qs, fileSize, false);
    JsonHelperT<QString>::PathGet(object, u"file_path"_qs, filePath, false);
  }
};

// TelegramBotChatMember - This object contains information about one member of the chat.
struct TelegramBotChatMember : public TelegramBotObject {
  TelegramBotUser user; // Information about the user
  QString status; // The member's status in the chat. Can be “creator”, “administrator”, “member”, “left” or “kicked”

  TelegramBotChatMember() {}
  TelegramBotChatMember(QJsonObject object) { FromJson(object); }

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<TelegramBotUser>::PathGet(object, u"user"_qs, user);
    JsonHelperT<QString>::PathGet(object, u"status"_qs, status);
  }
};

// TelegramBotResponseParameters - Contains information about why a request was unsuccessfull.
struct TelegramBotResponseParameters : public TelegramBotObject {
  qint32 migrateToChatId{}; // Optional. The group has been migrated to a supergroup with the specified identifier. This number may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier.
  qint32 retryAfter{}; // Optional. In case of exceeding flood control, the number of seconds left to wait before the request can be repeated

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<qint32>::PathGet(object, u"migrate_to_chat_id"_qs, migrateToChatId, false);
    JsonHelperT<qint32>::PathGet(object, u"retry_after"_qs, retryAfter, false);
  }
};

#define TELEGRAMBOTMESSAGE_FIELDS \
    qint32 messageId{}; /* Unique message identifier inside this chat */\
    std::optional<TelegramBotUser> from; /* Optional. Sender, can be empty for messages sent to channels */\
    qint32 date{}; /* Date the message was sent in Unix time */\
    TelegramBotChat chat; /* Conversation the message belongs to */\
    TelegramBotUser forwardFrom; /* Optional. For forwarded messages, sender of the original message */\
    TelegramBotChat forwardFromChat; /* Optional. For messages forwarded from a channel, information about the original channel */\
    qint32 forwardFromMessageId{}; /* Optional. For forwarded channel posts, identifier of the original message in the channel */\
    qint32 forwardDate{}; /* Optional. For forwarded messages, date the original message was sent in Unix time */\
    qint32 editDate{}; /* Optional. Date the message was last edited in Unix time */\
    QString text; /* Optional. For text messages, the actual UTF-8 text of the message, 0-4096 characters. */\
    QList<TelegramBotMessageEntity> entities; /* Optional. For text messages, special entities like usernames, URLs, bot commands, etc. that appear in the text */\
    TelegramBotAudio audio; /* Optional. Message is an audio file, information about the file */\
    TelegramBotDocument document; /* Optional. Message is a general file, information about the file */\
    TelegramBotGame game; /* Optional. Message is a game, information about the game. More about games */\
    QList<TelegramBotPhotoSize> photo; /* Optional. Message is a photo, available sizes of the photo */\
    TelegramBotSticker sticker; /* Optional. Message is a sticker, information about the sticker */\
    TelegramBotVideo video; /* Optional. Message is a video, information about the video */\
    TelegramBotVoice voice; /* Optional. Message is a voice message, information about the file */\
    QString caption; /* Optional. Caption for the document, photo or video, 0-200 characters */\
    TelegramBotContact contact; /* Optional. Message is a shared contact, information about the contact */\
    TelegramBotLocation location; /* Optional. Message is a shared location, information about the location */\
    TelegramBotVenue venue; /* Optional. Message is a venue, information about the venue */\
    TelegramBotUser newChatMember; /* Optional. A new member was added to the group, information about them (this member may be the bot itself) */\
    TelegramBotUser leftChatMember; /* Optional. A member was removed from the group, information about them (this member may be the bot itself) */\
    QString newChatTitle; /* Optional. A chat title was changed to this value */\
    QList<TelegramBotPhotoSize> newChatPhoto; /* Optional. A chat photo was change to this value */\
    bool deleteChatPhoto{false}; /* Optional. Service message: the chat photo was deleted */\
    bool groupChatCreated{false}; /* Optional. Service message: the group has been created */\
    bool supergroupChatCreated{false}; /* Optional. Service message: the supergroup has been created. This field can‘t be received in a message coming through updates, because bot can’t be a member of a supergroup when it is created. It can only be found in reply_to_message if someone replies to a very first message in a directly created supergroup. */\
    bool channelChatCreated{false}; /* Optional. Service message: the channel has been created. This field can‘t be received in a message coming through updates, because bot can’t be a member of a channel when it is created. It can only be found in reply_to_message if someone replies to a very first message in a channel. */\
    qint32 migrateToChatId{}; /* Optional. The group has been migrated to a supergroup with the specified identifier. This number may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier. */\
    qint32 migrateFromChatId{}; /* Optional. The supergroup has been migrated from a group with the specified identifier. This number may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier. */\

#define TELEGRAMBOTMESSAGE_FIELD_PARSING \
    JsonHelperT<qint32>::PathGet(object, u"message_id"_qs, messageId); \
    JsonHelperT<TelegramBotUser>::PathGet(object, u"from"_qs, from, false); \
    JsonHelperT<qint32>::PathGet(object, u"date"_qs, date); \
    JsonHelperT<TelegramBotChat>::PathGet(object, u"chat"_qs, chat, false); \
    JsonHelperT<TelegramBotUser>::PathGet(object, u"forward_from"_qs, forwardFrom, false); \
    JsonHelperT<TelegramBotChat>::PathGet(object, u"forward_from_chat"_qs, forwardFromChat, false); \
    JsonHelperT<qint32>::PathGet(object, u"forward_from_message_id"_qs, forwardFromMessageId, false); \
    JsonHelperT<qint32>::PathGet(object, u"forward_date"_qs, forwardDate, false); \
    JsonHelperT<qint32>::PathGet(object, u"edit_date"_qs, editDate, false); \
    JsonHelperT<QString>::PathGet(object, u"text"_qs, text, false); \
    JsonHelperT<TelegramBotMessageEntity>::jsonPathGetArray(object, u"entities"_qs, entities, false); \
    JsonHelperT<TelegramBotAudio>::PathGet(object, u"audio"_qs, audio, false); \
    JsonHelperT<TelegramBotDocument>::PathGet(object, u"document"_qs, document, false); \
    JsonHelperT<TelegramBotGame>::PathGet(object, u"game"_qs, game, false); \
    JsonHelperT<TelegramBotPhotoSize>::jsonPathGetArray(object, u"photo"_qs, photo, false); \
    JsonHelperT<TelegramBotSticker>::PathGet(object, u"sticker"_qs, sticker, false); \
    JsonHelperT<TelegramBotVideo>::PathGet(object, u"video"_qs, video, false); \
    JsonHelperT<TelegramBotVoice>::PathGet(object, u"voice"_qs, voice, false); \
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false); \
    JsonHelperT<TelegramBotContact>::PathGet(object, u"contact"_qs, contact, false); \
    JsonHelperT<TelegramBotLocation>::PathGet(object, u"location"_qs, location, false); \
    JsonHelperT<TelegramBotVenue>::PathGet(object, u"venue"_qs, venue, false); \
    JsonHelperT<TelegramBotUser>::PathGet(object, u"new_chat_member"_qs, newChatMember, false); \
    JsonHelperT<TelegramBotUser>::PathGet(object, u"left_chat_member"_qs, leftChatMember, false); \
    JsonHelperT<QString>::PathGet(object, u"new_chat_title"_qs, newChatTitle, false); \
    JsonHelperT<TelegramBotPhotoSize>::jsonPathGetArray(object, u"new_chat_photo"_qs, newChatPhoto, false); \
    JsonHelperT<bool>::PathGet(object, u"delete_chat_photo = false"_qs, deleteChatPhoto, false); \
    JsonHelperT<bool>::PathGet(object, u"group_chat_created = false"_qs, groupChatCreated, false); \
    JsonHelperT<bool>::PathGet(object, u"supergroup_chat_created = false"_qs, supergroupChatCreated, false); \
    JsonHelperT<bool>::PathGet(object, u"channel_chat_created = false"_qs, channelChatCreated, false); \
    JsonHelperT<qint32>::PathGet(object, u"migrate_to_chat_id"_qs, migrateToChatId, false); \
    JsonHelperT<qint32>::PathGet(object, u"migrate_from_chat_id"_qs, migrateFromChatId, false);

// TelegramBotMessageSingle - This object represents a message (without any recursive fields which references to same class, see TelegramBotMessage for complete message)
struct TelegramBotMessageSingle : public TelegramBotObject {
  TELEGRAMBOTMESSAGE_FIELDS

  void FromJson(const QJsonObject& object) override {
    TELEGRAMBOTMESSAGE_FIELD_PARSING
  }
};

// TelegramBotMessage - This object represents a message.
struct TelegramBotMessage : public TelegramBotObject {
  TELEGRAMBOTMESSAGE_FIELDS
  TelegramBotMessageSingle replyToMessage; // Optional. For replies, the original message. Note that the Message object in this field will not contain further reply_to_message fields even if it itself is a reply.
  TelegramBotMessageSingle pinnedMessage; // Optional. Specified message was pinned. Note that the Message object in this field will not contain further reply_to_message fields even if it is itself a reply.

  void FromJson(const QJsonObject& object) override {
    // message
    TELEGRAMBOTMESSAGE_FIELD_PARSING

    // own data
    JsonHelperT<TelegramBotMessageSingle>::PathGet(object, u"reply_to_message"_qs, replyToMessage, false);
    JsonHelperT<TelegramBotMessageSingle>::PathGet(object, u"pinned_message"_qs, pinnedMessage, false);
  }
};

// TelegramBotCallbackQuery - This object represents an incoming callback query from a callback button in an inline keyboard. If the button that originated the query was attached to a message sent by the bot, the field message will be present. If the button was attached to a message sent via the bot (in inline mode), the field inline_message_id will be present. Exactly one of the fields data or game_short_name will be present.
struct TelegramBotCallbackQuery : public TelegramBotObject {
  QString id; // Unique identifier for this query
  TelegramBotUser from; // Sender
  TelegramBotMessage message; // Optional. Message with the callback button that originated the query. Note that message content and message date will not be available if the message is too old
  QString inlineMessageId; // Optional. Identifier of the message sent via the bot in inline mode, that originated the query.
  QString chatInstance; // Global identifier, uniquely corresponding to the chat to which the message with the callback button was sent. Useful for high scores in games.
  QString data; // Optional. Data associated with the callback button. Be aware that a bad client can send arbitrary data in this field.
  QString gameShortName; // Optional. Short name of a Game to be returned, serves as the unique identifier for the game

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<TelegramBotUser>::PathGet(object, u"from"_qs, from);
    JsonHelperT<TelegramBotMessage>::PathGet(object, u"message"_qs, message, false);
    JsonHelperT<QString>::PathGet(object, u"inline_message_id"_qs, inlineMessageId, false);
    JsonHelperT<QString>::PathGet(object, u"chat_instance"_qs, chatInstance);
    JsonHelperT<QString>::PathGet(object, u"data"_qs, data, false);
    JsonHelperT<QString>::PathGet(object, u"game_short_name"_qs, gameShortName, false);
  }
};

/*
 * Inline Query data structs
 */

// This object represents an incoming inline query. When the user sends an empty query, your bot could return some default or trending results.
struct TelegramBotInlineQuery : public TelegramBotObject {
  QString id; // Unique identifier for this query
  TelegramBotUser from; // Sender
  TelegramBotLocation location; // Optional. Sender location, only for bots that request user location
  QString query; // Text of the query (up to 512 characters)
  QString offset; // Offset of the results to be returned, can be controlled by the bot

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<TelegramBotUser>::PathGet(object, u"from"_qs, from);
    JsonHelperT<TelegramBotLocation>::PathGet(object, u"location"_qs, location, false);
    JsonHelperT<QString>::PathGet(object, u"query"_qs, query);
    JsonHelperT<QString>::PathGet(object, u"offset"_qs, offset);
  }
};

// Represents the content of a text message to be sent as the result of an inline query.
struct TelegramBotInputMessageContent : public TelegramBotObject {
  QString messageText; // Text of the message to be sent, 1-4096 characters
  QString parseMode; // Optional. Send Markdown or HTML, if you want Telegram apps to show bold, italic, fixed-width text or inline URLs in your bot's message.
  bool disableWebPagePreview{}; // Optional. Disables link previews for links in the sent message

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"message_text"_qs, messageText);
    JsonHelperT<QString>::PathGet(object, u"parse_mode"_qs, parseMode, false);
    JsonHelperT<bool>::PathGet(object, u"disable_web_page_preview"_qs, disableWebPagePreview, false);
  }
};

// This object represents one result of an inline query. Telegram clients currently support results of the following 20 types:
struct TelegramBotInlineQueryResult : public TelegramBotObject {
  QString type; // Type of the result, must be article
  QString id; // Unique identifier for this result, 1-64 Bytes
  QString title; // Title of the result
  TelegramBotInputMessageContent inputMessageContent; // Content of the message to be sent
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  QString url; // Optional. URL of the result
  bool hideUrl{}; // Optional. Pass True, if you don't want the URL to be shown in the message
  QString description; // Optional. Short description of the result
  QString thumbUrl; // Optional. Url of the thumbnail for the result
  qint32 thumbWidth{}; // Optional. Thumbnail width
  qint32 thumbHeight{}; // Optional. Thumbnail height

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<QString>::PathGet(object, u"url"_qs, url, false);
    JsonHelperT<bool>::PathGet(object, u"hide_url"_qs, hideUrl, false);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_width"_qs, thumbWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_height"_qs, thumbHeight, false);
  }
};

// Represents a link to a photo. By default, this photo will be sent by the user with optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the photo.
struct TelegramBotInlineQueryResultPhoto : public TelegramBotObject {
  QString type; // Type of the result, must be photo
  QString id; // Unique identifier for this result, 1-64 bytes
  QString photoUrl; // A valid URL of the photo. Photo must be in jpeg format. Photo size must not exceed 5MB
  QString thumbUrl; // URL of the thumbnail for the photo
  qint32 photoWidth{}; // Optional. Width of the photo
  qint32 photoHeight{}; // Optional. Height of the photo
  QString title; // Optional. Title for the result
  QString description; // Optional. Short description of the result
  QString caption; // Optional. Caption of the photo to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the photo

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"photo_url"_qs, photoUrl);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl);
    JsonHelperT<qint32>::PathGet(object, u"photo_width"_qs, photoWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"photo_height"_qs, photoHeight, false);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to an animated GIF file. By default, this animated GIF file will be sent by the user with optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the animation.
struct TelegramBotInlineQueryResultGif : public TelegramBotObject {
  QString type; // Type of the result, must be gif
  QString id; // Unique identifier for this result, 1-64 bytes
  QString gifUrl; // A valid URL for the GIF file. File size must not exceed 1MB
  qint32 gifWidth{}; // Optional. Width of the GIF
  qint32 gifHeight{}; // Optional. Height of the GIF
  QString thumbUrl; // URL of the static thumbnail for the result (jpeg or gif)
  QString title; // Optional. Title for the result
  QString caption; // Optional. Caption of the GIF file to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the GIF animation

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"gif_url"_qs, gifUrl);
    JsonHelperT<qint32>::PathGet(object, u"gif_width"_qs, gifWidth);
    JsonHelperT<qint32>::PathGet(object, u"gif_height"_qs, gifHeight);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a video animation (H.264/MPEG-4 AVC video without sound). By default, this animated MPEG-4 file will be sent by the user with optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the animation.
struct TelegramBotInlineQueryResultMpeg4Gif : public TelegramBotObject {
  QString type; // Type of the result, must be mpeg4_gif
  QString id; // Unique identifier for this result, 1-64 bytes
  QString mpeg4Url; // A valid URL for the MP4 file. File size must not exceed 1MB
  qint32 mpeg4Width{}; // Optional. Video width
  qint32 mpeg4Height{}; // Optional. Video height
  QString thumbUrl; // URL of the static thumbnail (jpeg or gif) for the result
  QString title; // Optional. Title for the result
  QString caption; // Optional. Caption of the MPEG-4 file to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the video animation

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"mpeg4Url"_qs, mpeg4Url);
    JsonHelperT<qint32>::PathGet(object, u"mpeg4Width"_qs, mpeg4Width);
    JsonHelperT<qint32>::PathGet(object, u"mpeg4Height"_qs, mpeg4Height);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a page containing an embedded video player or a video file. By default, this video file will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the video.
struct TelegramBotInlineQueryResultVideo : public TelegramBotObject {
  QString type; // Type of the result, must be video
  QString id; // Unique identifier for this result, 1-64 bytes
  QString videoUrl; // A valid URL for the embedded video player or video file
  QString mimeType; // Mime type of the content of video url, “text/html” or “video/mp4”
  QString thumbUrl; // URL of the thumbnail (jpeg only) for the video
  QString title; // Title for the result
  QString caption; // Optional. Caption of the video to be sent, 0-200 characters
  qint32 videoWidth{}; // Optional. Video width
  qint32 videoHeight{}; // Optional. Video height
  qint32 videoDuration{}; // Optional. Video duration in seconds
  QString description; // Optional. Short description of the result
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the video

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"video_url"_qs, videoUrl);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<qint32>::PathGet(object, u"video_width"_qs, videoWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"video_height"_qs, videoHeight, false);
    JsonHelperT<qint32>::PathGet(object, u"video_duration"_qs, videoDuration, false);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to an mp3 audio file. By default, this audio file will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the audio.
struct TelegramBotInlineQueryResultAudio : public TelegramBotObject {
  QString type; // Type of the result, must be audio
  QString id; // Unique identifier for this result, 1-64 bytes
  QString audioUrl; // A valid URL for the audio file
  QString title; // Title
  QString caption; // Optional. Caption, 0-200 characters
  QString performer; // Optional. Performer
  qint32 audioDuration{}; // Optional. Audio duration in seconds
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the audio (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"audio_url"_qs, audioUrl);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<QString>::PathGet(object, u"performer"_qs, performer, false);
    JsonHelperT<qint32>::PathGet(object, u"audio_duration"_qs, audioDuration, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a voice recording in an .ogg container encoded with OPUS. By default, this voice recording will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the the voice message.
struct TelegramBotInlineQueryResultVoice : public TelegramBotObject {
  QString type; // Type of the result, must be voice
  QString id; // Unique identifier for this result, 1-64 bytes
  QString voiceUrl; // A valid URL for the voice recording
  QString title; // Recording title
  QString caption; // Optional. Caption, 0-200 characters
  qint32 voiceDuration{}; // Optional. Recording duration in seconds
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the voice recording (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"voice_url"_qs, voiceUrl);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<qint32>::PathGet(object, u"voice_duration"_qs, voiceDuration, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a file. By default, this file will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the file. Currently, only .PDF and .ZIP files can be sent using this method.
struct TelegramBotInlineQueryResultDocument : public TelegramBotObject {
  QString type; // Type of the result, must be document
  QString id; // Unique identifier for this result, 1-64 bytes
  QString title; // Title for the result
  QString caption; // Optional. Caption of the document to be sent, 0-200 characters
  QString documentUrl; // A valid URL for the file
  QString mimeType; // Mime type of the content of the file, either “application/pdf” or “application/zip”
  QString description; // Optional. Short description of the result
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the file
  QString thumbUrl; // Optional. URL of the thumbnail (jpeg only) for the file
  qint32 thumbWidth{}; // Optional. Thumbnail width
  qint32 thumbHeight{}; // Optional. Thumbnail height (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption);
    JsonHelperT<QString>::PathGet(object, u"document_url"_qs, documentUrl);
    JsonHelperT<QString>::PathGet(object, u"mime_type"_qs, mimeType);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_width"_qs, thumbWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_height"_qs, thumbHeight, false);
  }
};

// Represents a location on a map. By default, the location will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the location.
struct TelegramBotInlineQueryResultLocation : public TelegramBotObject {
  QString type; // Type of the result, must be location
  QString id; // Unique identifier for this result, 1-64 Bytes
  double latitude; // Location latitude in degrees
  double longitude; // Location longitude in degrees
  QString title; // Location title
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the location
  QString thumbUrl; // Optional. Url of the thumbnail for the result
  qint32 thumbWidth{}; // Optional. Thumbnail width
  qint32 thumbHeight{}; // Optional. Thumbnail height (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<double>::PathGet(object, u"latitude"_qs, latitude);
    JsonHelperT<double>::PathGet(object, u"longitude"_qs, longitude);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_width"_qs, thumbWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_height"_qs, thumbHeight, false);
  }
};

// Represents a venue. By default, the venue will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the venue.
struct TelegramBotInlineQueryResultVenue : public TelegramBotObject {
  QString type; // Type of the result, must be venue
  QString id; // Unique identifier for this result, 1-64 Bytes
  double latitude; // Latitude of the venue location in degrees
  double longitude; // Longitude of the venue location in degrees
  QString title; // Title of the venue
  QString address; // Address of the venue
  QString foursquareId; // Optional. Foursquare identifier of the venue if known
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the venue
  QString thumbUrl; // Optional. Url of the thumbnail for the result
  qint32 thumbWidth{}; // Optional. Thumbnail width
  qint32 thumbHeight{}; // Optional. Thumbnail height (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<double>::PathGet(object, u"latitude"_qs, latitude);
    JsonHelperT<double>::PathGet(object, u"longitude"_qs, longitude);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"address"_qs, address);
    JsonHelperT<QString>::PathGet(object, u"foursquare_id"_qs, foursquareId, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_width"_qs, thumbWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_height"_qs, thumbHeight, false);
  }
};

// Represents a contact with a phone number. By default, this contact will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the contact.
struct TelegramBotInlineQueryResultContact : public TelegramBotObject {
  QString type; // Type of the result, must be contact
  QString id; // Unique identifier for this result, 1-64 Bytes
  QString phoneNumber; // Contact's phone number
  QString firstName; // Contact's first name
  QString lastName; // Optional. Contact's last name
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the contact
  QString thumbUrl; // Optional. Url of the thumbnail for the result
  qint32 thumbWidth{}; // Optional. Thumbnail width
  qint32 thumbHeight{}; // Optional. Thumbnail height (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"phone_number"_qs, phoneNumber);
    JsonHelperT<QString>::PathGet(object, u"first_name"_qs, firstName);
    JsonHelperT<QString>::PathGet(object, u"last_name"_qs, lastName, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
    JsonHelperT<QString>::PathGet(object, u"thumb_url"_qs, thumbUrl, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_width"_qs, thumbWidth, false);
    JsonHelperT<qint32>::PathGet(object, u"thumb_height"_qs, thumbHeight, false);
  }
};

// Represents a Game.
struct TelegramBotInlineQueryResultGame : public TelegramBotObject {
  QString type; // Type of the result, must be game
  QString id; // Unique identifier for this result, 1-64 bytes
  QString gameShortName; // Short name of the game
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message (Note: This will only work in Telegram versions released after October 1, 2016. Older clients will not display any inline results if a game result is among them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"game_short_name"_qs, gameShortName);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
  }
};

// Represents a link to a photo stored on the Telegram servers. By default, this photo will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the photo.
struct TelegramBotInlineQueryResultCachedPhoto : public TelegramBotObject {
  QString type; // Type of the result, must be photo
  QString id; // Unique identifier for this result, 1-64 bytes
  QString photoFileId; // A valid file identifier of the photo
  QString title; // Optional. Title for the result
  QString description; // Optional. Short description of the result
  QString caption; // Optional. Caption of the photo to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the photo

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"photo_file_id"_qs, photoFileId);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to an animated GIF file stored on the Telegram servers. By default, this animated GIF file will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with specified content instead of the animation.
struct TelegramBotInlineQueryResultCachedGif : public TelegramBotObject {
  QString type; // Type of the result, must be gif
  QString id; // Unique identifier for this result, 1-64 bytes
  QString gifFileId; // A valid file identifier for the GIF file
  QString title; // Optional. Title for the result
  QString caption; // Optional. Caption of the GIF file to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the GIF animation

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"gif_file_id"_qs, gifFileId);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a video animation (H.264/MPEG-4 AVC video without sound) stored on the Telegram servers. By default, this animated MPEG-4 file will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the animation.
struct TelegramBotInlineQueryResultCachedMpeg4Gif : public TelegramBotObject {
  QString type; // Type of the result, must be mpeg4_gif
  QString id; // Unique identifier for this result, 1-64 bytes
  QString mpeg4FileId; // A valid file identifier for the MP4 file
  QString title; // Optional. Title for the result
  QString caption; // Optional. Caption of the MPEG-4 file to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the video animation

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"mpeg4FileId"_qs, mpeg4FileId);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a sticker stored on the Telegram servers. By default, this sticker will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the sticker.
struct TelegramBotInlineQueryResultCachedSticker : public TelegramBotObject {
  QString type; // Type of the result, must be sticker
  QString id; // Unique identifier for this result, 1-64 bytes
  QString stickerFileId; // A valid file identifier of the sticker
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the sticker (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"sticker_file_id"_qs, stickerFileId);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a file stored on the Telegram servers. By default, this file will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the file.
struct TelegramBotInlineQueryResultCachedDocument : public TelegramBotObject {
  QString type; // Type of the result, must be document
  QString id; // Unique identifier for this result, 1-64 bytes
  QString title; // Title for the result
  QString documentFileId; // A valid file identifier for the file
  QString description; // Optional. Short description of the result
  QString caption; // Optional. Caption of the document to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the file (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"document_file_id"_qs, documentFileId);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a video file stored on the Telegram servers. By default, this video file will be sent by the user with an optional caption. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the video.
struct TelegramBotInlineQueryResultCachedVideo : public TelegramBotObject {
  QString type; // Type of the result, must be video
  QString id; // Unique identifier for this result, 1-64 bytes
  QString videoFileId; // A valid file identifier for the video file
  QString title; // Title for the result
  QString description; // Optional. Short description of the result
  QString caption; // Optional. Caption of the video to be sent, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the video

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"video_file_id"_qs, videoFileId);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"description"_qs, description, false);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to a voice message stored on the Telegram servers. By default, this voice message will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the voice message.
struct TelegramBotInlineQueryResultCachedVoice : public TelegramBotObject {
  QString type; // Type of the result, must be voice
  QString id; // Unique identifier for this result, 1-64 bytes
  QString voiceFileId; // A valid file identifier for the voice message
  QString title; // Voice message title
  QString caption; // Optional. Caption, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the voice message (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"voice_file_id"_qs, voiceFileId);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents a link to an mp3 audio file stored on the Telegram servers. By default, this audio file will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the audio.
struct TelegramBotInlineQueryResultCachedAudio : public TelegramBotObject {
  QString type; // Type of the result, must be audio
  QString id; // Unique identifier for this result, 1-64 bytes
  QString audioFileId; // A valid file identifier for the audio file
  QString caption; // Optional. Caption, 0-200 characters
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the audio (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"type"_qs, type);
    JsonHelperT<QString>::PathGet(object, u"id"_qs, id);
    JsonHelperT<QString>::PathGet(object, u"audio_file_id"_qs, audioFileId);
    JsonHelperT<QString>::PathGet(object, u"caption"_qs, caption, false);
    JsonHelperT<TelegramBotKeyboardButton>::jsonPathGetArrayArray(object, u"reply_markup"_qs, replyMarkup, false);
    JsonHelperT<TelegramBotInputMessageContent>::PathGet(object, u"input_message_content"_qs, inputMessageContent, false);
  }
};

// Represents the content of a location message to be sent as the result of an inline query.
struct TelegramBotInputLocationMessageContent : public TelegramBotObject {
  double latitude; // Latitude of the location in degrees
  double longitude; // Longitude of the location in degrees (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<double>::PathGet(object, u"latitude"_qs, latitude);
    JsonHelperT<double>::PathGet(object, u"longitude"_qs, longitude);
  }
};

// Represents the content of a venue message to be sent as the result of an inline query.
struct TelegramBotInputVenueMessageContent : public TelegramBotObject {
  double latitude; // Latitude of the venue in degrees
  double longitude; // Longitude of the venue in degrees
  QString title; // Name of the venue
  QString address; // Address of the venue
  QString foursquareId; // Optional. Foursquare identifier of the venue, if known (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<double>::PathGet(object, u"latitude"_qs, latitude);
    JsonHelperT<double>::PathGet(object, u"longitude"_qs, longitude);
    JsonHelperT<QString>::PathGet(object, u"title"_qs, title);
    JsonHelperT<QString>::PathGet(object, u"address"_qs, address);
    JsonHelperT<QString>::PathGet(object, u"foursquare_id"_qs, foursquareId, false);
  }
};

// Represents the content of a contact message to be sent as the result of an inline query.
struct TelegramBotInputContactMessageContent : public TelegramBotObject {
  QString phoneNumber; // Contact's phone number
  QString firstName; // Contact's first name
  QString lastName; // Optional. Contact's last name (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"phone_number"_qs, phoneNumber);
    JsonHelperT<QString>::PathGet(object, u"first_name"_qs, firstName);
    JsonHelperT<QString>::PathGet(object, u"last_name"_qs, lastName, false);
  }
};

// Represents a result of an inline query that was chosen by the user and sent to their chat partner.
struct TelegramBotChosenInlineResult : public TelegramBotObject {
  QString resultId; // The unique identifier for the result that was chosen
  TelegramBotUser from; // The user that chose the result
  TelegramBotLocation location; // Optional. Sender location, only for bots that require user location
  QString inlineMessageId; // Optional. Identifier of the sent inline message. Available only if there is an inline keyboard attached to the message. Will be also received in callback queries and can be used to edit the message.
  QString query; // The query that was used to obtain the result

  void FromJson(const QJsonObject& object) override {
    JsonHelperT<QString>::PathGet(object, u"result_id"_qs, resultId);
    JsonHelperT<TelegramBotUser>::PathGet(object, u"from"_qs, from);
    JsonHelperT<TelegramBotLocation>::PathGet(object, u"location"_qs, location, false);
    JsonHelperT<QString>::PathGet(object, u"inline_message_id"_qs, inlineMessageId, false);
    JsonHelperT<QString>::PathGet(object, u"query"_qs, query);
  }
};

// This object represents an incoming update.
struct TelegramBotUpdatePrivate : public TelegramBotObject {
  TelegramBotMessageType type{Undefined};
  int updateId{};

  // Contains the message for the following Update types:
  // Message, EditedMessage, ChannelPost, EditedChannelPost, CallbackQuery
  // Special Case: points also to the Message of a CallbackQuery!
  TelegramBotMessage* message{};
  TelegramBotInlineQuery* inlineQuery{};
  TelegramBotChosenInlineResult* chosenInlineResult{};
  TelegramBotCallbackQuery* callbackQuery{};

  ~TelegramBotUpdatePrivate() {
    if (!callbackQuery) delete message;
    delete inlineQuery;
    delete chosenInlineResult;
    delete callbackQuery;
  }

  void FromJson(const QJsonObject& object) override {
    for (auto itr = object.begin(); itr != object.end(); itr++) {
      // parse update id
      if(itr.key() == "update_id") {
          this->updateId = itr.value().toVariant().toInt();
          continue;
      }

      static std::map<QString, TelegramBotMessageType> key_to_type{
          {u"message"_qs, Message},
          {u"edited_message"_qs, EditedMessage},
          {u"channel_post"_qs, ChannelPost},
          {u"edited_channel_post"_qs, EditedChannelPost},
          {u"inline_query"_qs, InlineQuery},
          {u"chosen_inline_result"_qs, ChosenInlineResult},
          {u"callback_query"_qs, CallbackQuery} };

      auto it_type = key_to_type.find(itr.key());
      if (it_type == key_to_type.cend()) {
        qWarning() << "Invalid type: " << itr.key();
      } else {
        type = it_type->second;
      }
      const QJsonObject oMessage = itr.value().toObject();
      switch (type) {
      case Message:
      case EditedMessage:
      case ChannelPost:
      case EditedChannelPost:
        message = new TelegramBotMessage;
        message->FromJson(oMessage);
        break;
      case InlineQuery:
        inlineQuery = new TelegramBotInlineQuery;
        inlineQuery->FromJson(oMessage);
        break;
      case ChosenInlineResult:
        chosenInlineResult = new TelegramBotChosenInlineResult;
        chosenInlineResult->FromJson(oMessage);
        break;
      case CallbackQuery:
        callbackQuery = new TelegramBotCallbackQuery;
        callbackQuery->FromJson(oMessage);
        message = &callbackQuery->message;
        break;
      }
    }
  }
};
typedef QSharedPointer<TelegramBotUpdatePrivate> TelegramBotUpdate;
