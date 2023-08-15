#pragma once

#include "jsonhelper.h"

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
  qint32 error_code{0};
  QString description;

  TelegramBotOperationResult(const QJsonObject& object) { FromJson(object); }

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"error_code"_s, error_code, false);
    return JsonHelper::PathGet(object, u"result"_s, result) &&
           JsonHelper::PathGet(object, u"description"_s, description);
  }
};

// TelegramBotWebHookInfo - This object represents Telegram Webhook Information
struct TelegramBotWebHookInfo : public TelegramBotObject {
  QString url;
  bool hasCustomCertificate{};
  bool pendingUpdateCount{};
  qint32 lastErrorDate{};
  QString lastErrorMessage;
  qint32 maxConnections{};
  QList<QString> allowedUpdates;

  TelegramBotWebHookInfo(QJsonObject object) { FromJson(object); }

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"last_error_date"_s, lastErrorDate, false);
    JsonHelper::PathGet(object, u"last_error_message"_s, lastErrorMessage, false);
    JsonHelper::PathGet(object, u"max_connections"_s, maxConnections, false);
    JsonHelper::PathGet(object, u"allowed_updates"_s, allowedUpdates, false);
    return JsonHelper::PathGet(object, u"url"_s, url) &&
           JsonHelper::PathGet(object, u"has_custom_certificate"_s, hasCustomCertificate) &&
           JsonHelper::PathGet(object, u"pending_update_count"_s, pendingUpdateCount);
  }
};

/*
 *  Telegram Game Data Structs
 */

// TelegramBotUser - This object represents a Telegram user or bot.
struct TelegramBotUser : public TelegramBotObject {
  qint64 id; // Unique identifier for this user or bot
  bool is_bot{false};
  QString first_name; // User‘s or bot’s first name
  std::optional<QString> last_name; // Optional. User‘s or bot’s last name
  std::optional<QString> username; // Optional. User‘s or bot’s username
  std::optional<QString> language_code; // Optional. User‘s or bot’s locale

  TelegramBotUser() { }
  TelegramBotUser(QJsonObject object) { FromJson(object); }

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"last_name"_s, last_name, false);
    JsonHelper::PathGet(object, u"username"_s, username, false);
    JsonHelper::PathGet(object, u"language_code"_s, language_code, false);
    return JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"is_bot"_s, is_bot) &&
           JsonHelper::PathGet(object, u"first_name"_s, first_name);
  }
};

// TelegramBotMessageEntity - This object represents one special entity in a text message. For example, hashtags, usernames, URLs, etc.
struct TelegramBotMessageEntity : public TelegramBotObject {
  QString type{}; // Type of the entity. Can be mention ((at)username), hashtag, bot_command, url, email, bold (bold text), italic (italic text), code (monowidth string), pre (monowidth block), text_link (for clickable text URLs), text_mention (for users without usernames)
  qint32 offset{}; // Offset in UTF-16 code units to the start of the entity
  qint32 length{}; // Length of the entity in UTF-16 code units
  QString url; // Optional. For “text_link” only, url that will be opened after user taps on the text
  TelegramBotUser user; // Optional. For “text_mention” only, the mentioned user
  bool IsCommand() const { return type.compare(u"bot_command"_s, Qt::CaseInsensitive) == 0; }

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"url"_s, url, false);
    JsonHelper::PathGet(object, u"user"_s, user, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"offset"_s, offset) &&
           JsonHelper::PathGet(object, u"length"_s, length);
  }
};

// TelegramBotPhotoSize - This object represents one size of a photo or a file / sticker thumbnail.
struct TelegramBotPhotoSize : public TelegramBotObject {
  QString fileId; // Unique identifier for this file
  qint32 width{}; // Photo width
  qint32 height{}; // Photo height
  qint32 fileSize{}; // Optional. File size

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId) &&
           JsonHelper::PathGet(object, u"width"_s, width) &&
           JsonHelper::PathGet(object, u"height"_s, height);
  }
};

// You can provide an animation for your game so that it looks stylish in chats (check out Lumberjack for an example). This object represents an animation file to be displayed in the message containing a game.
struct TelegramBotAnimation : public TelegramBotObject {
  QString fileId; // Unique file identifier
  TelegramBotPhotoSize thumb; // Optional. Animation thumbnail as defined by sender
  QString fileName; // Optional. Original animation filename as defined by sender
  QString mimeType; // Optional. MIME type of the file as defined by sender
  qint32 fileSize; // Optional. File size

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"thumb"_s, thumb, false);
    JsonHelper::PathGet(object, u"file_name"_s, fileName, false);
    JsonHelper::PathGet(object, u"mime_type"_s, mimeType, false);
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"text"_s, text, false);
    JsonHelper::PathGet(object, u"text_entities"_s, textEntities, false);
    JsonHelper::PathGet(object, u"animation"_s, animation, false);
    return JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"description"_s, description) &&
           JsonHelper::PathGet(object, u"photo"_s, photo);
  }
};

// This object represents one row of the high scores table for a game.
struct TelegramBotGameHighScore : public TelegramBotObject {
  qint32 position{}; // Position in high score table for the game
  TelegramBotUser user;
  qint32 score{};

  bool FromJson(const QJsonObject& object) override {
    return JsonHelper::PathGet(object, u"position"_s, position) &&
           JsonHelper::PathGet(object, u"user"_s, user) &&
           JsonHelper::PathGet(object, u"score"_s, score);
  }
};

/*
 *  Telegram Base Data Structs
 */

struct TelegramBotKeyboardButton : public TelegramBotObject {
  TELEGRAMBOTKEYBOARD_FIELDS
  TelegramBotGame callbackGame;

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"url"_s, url, false);
    JsonHelper::PathGet(object, u"callback_data"_s, callbackData, false);
    JsonHelper::PathGet(object, u"switch_inline_query"_s, switchInlineQuery, false);
    JsonHelper::PathGet(object, u"switch_inline_query_current_chat"_s, switchInlineQueryCurrentChat, false);
    JsonHelper::PathGet(object, u"callback_game"_s, callbackGame, false);
    JsonHelper::PathGet(object, u"request_contact"_s, requestContact, false);
    JsonHelper::PathGet(object, u"request_location"_s, requestLocation, false);
    return JsonHelper::PathGet(object, u"text"_s, text);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"username"_s, username, false);
    JsonHelper::PathGet(object, u"first_name"_s, firstName, false);
    JsonHelper::PathGet(object, u"last_name"_s, lastName, false);
    JsonHelper::PathGet(object, u"all_members_are_administrators"_s, allMembersAreAdministrators, false);
    return JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"type"_s, type);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"performer"_s, performer, false);
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"mime_type"_s, mimeType, false);
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId) &&
           JsonHelper::PathGet(object, u"duration"_s, duration);
  }
};

// TelegramBotDocument - This object represents a general file (as opposed to photos, voice messages and audio files).
struct TelegramBotDocument : public TelegramBotObject {
  QString fileId; // Unique file identifier
  TelegramBotPhotoSize thumb; // Optional. Document thumbnail as defined by sender
  QString fileName; // Optional. Original filename as defined by sender
  QString mimeType; // Optional. MIME type of the file as defined by sender
  qint32 fileSize{}; // Optional. File size

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"thumb"_s, thumb, false);
    JsonHelper::PathGet(object, u"file_name"_s, fileName, false);
    JsonHelper::PathGet(object, u"mime_type"_s, mimeType, false);
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"thumb"_s, thumb, false);
    JsonHelper::PathGet(object, u"emoji"_s, emoji, false);
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId) &&
           JsonHelper::PathGet(object, u"width"_s, width) &&
           JsonHelper::PathGet(object, u"height"_s, height);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"thumb"_s, thumb, false);
    JsonHelper::PathGet(object, u"mime_type"_s, mimeType, false);
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId) &&
           JsonHelper::PathGet(object, u"width"_s, width) &&
           JsonHelper::PathGet(object, u"height"_s, height) &&
           JsonHelper::PathGet(object, u"duration"_s, duration);
  }
};

// TelegramBotVoice - This object represents a voice note.
struct TelegramBotVoice : public TelegramBotObject {
  QString file_id; // Identifier for this file, which can be used to download or reuse the file
  QString file_unique_id; // Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
  qint32 duration{}; // Duration of the audio in seconds as defined by sender
  QString mime_type; // Optional. MIME type of the file as defined by sender
  qint32 file_size{}; // Optional. File size

  bool IsFill() const {
    return !file_id.isEmpty() && !file_unique_id.isEmpty();
  }

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"mime_type"_s, mime_type, false);
    JsonHelper::PathGet(object, u"file_size"_s, file_size, false);
    return JsonHelper::PathGet(object, u"file_id"_s, file_id) &&
           JsonHelper::PathGet(object, u"file_unique_id"_s, file_unique_id) &&
           JsonHelper::PathGet(object, u"duration"_s, duration);
  }
};

// TelegramBotContact - This object represents a phone contact.
struct TelegramBotContact : public TelegramBotObject {
  QString phoneNumber; // Contact's phone number
  QString firstName; // Contact's first name
  QString lastName; // Optional. Contact's last name
  qint64 userId{}; // Optional. Contact's user identifier in Telegram

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"last_name"_s, lastName, false);
    JsonHelper::PathGet(object, u"user_id"_s, userId, false);
    return JsonHelper::PathGet(object, u"phone_number"_s, phoneNumber) &&
           JsonHelper::PathGet(object, u"first_name"_s, firstName);
  }
};

// TelegramBotLocation - This object represents a point on the map.
struct TelegramBotLocation : public TelegramBotObject {
  double longitude{}; // Longitude as defined by sender
  double latitude{}; // Latitude as defined by sender

  bool FromJson(const QJsonObject& object) override {
    return JsonHelper::PathGet(object, u"longitude"_s, longitude) &&
           JsonHelper::PathGet(object, u"latitude"_s, latitude);
  }
};

// TelegramBotVenue - This object represents a venue.
struct TelegramBotVenue : public TelegramBotObject {
  TelegramBotLocation location; // Venue location
  QString title; // Name of the venue
  QString address; // Address of the venue
  QString foursquareId; // Optional. Foursquare identifier of the venue

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"foursquare_id"_s, foursquareId, false);
    return JsonHelper::PathGet(object, u"location"_s, location) &&
           JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"address"_s, address);
  }
};

// This object represent a user's profile pictures.
struct TelegramBotUserProfilePhotos : public TelegramBotObject {
  qint32 totalCount{}; // Total number of profile pictures the target user has
  QList<QList<TelegramBotPhotoSize>> photos; // Requested profile pictures (in up to 4 sizes each)

  TelegramBotUserProfilePhotos(QJsonObject object) { FromJson(object); }

  bool FromJson(const QJsonObject& object) override {
    return JsonHelper::PathGet(object, u"total_count"_s, totalCount) &&
           JsonHelper::PathGet(object, u"photos"_s, photos);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"file_size"_s, fileSize, false);
    JsonHelper::PathGet(object, u"file_path"_s, filePath, false);
    return JsonHelper::PathGet(object, u"file_id"_s, fileId);
  }
};

// TelegramBotChatMember - This object contains information about one member of the chat.
struct TelegramBotChatMember : public TelegramBotObject {
  TelegramBotUser user; // Information about the user
  QString status; // The member's status in the chat. Can be “creator”, “administrator”, “member”, “left” or “kicked”

  TelegramBotChatMember() {}
  TelegramBotChatMember(QJsonObject object) { FromJson(object); }

  bool FromJson(const QJsonObject& object) override {
    return JsonHelper::PathGet(object, u"user"_s, user) &&
           JsonHelper::PathGet(object, u"status"_s, status);
  }
};

// TelegramBotResponseParameters - Contains information about why a request was unsuccessfull.
struct TelegramBotResponseParameters : public TelegramBotObject {
  qint64 migrateToChatId{}; // Optional. The group has been migrated to a supergroup with the specified identifier. This number may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier.
  qint32 retryAfter{}; // Optional. In case of exceeding flood control, the number of seconds left to wait before the request can be repeated

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"migrate_to_chat_id"_s, migrateToChatId, false);
    JsonHelper::PathGet(object, u"retry_after"_s, retryAfter, false);
    return true;
  }
};

#define TELEGRAMBOTMESSAGE_FIELDS \
    qint64 messageId{}; /* Unique message identifier inside this chat */\
    std::optional<TelegramBotUser> from; /* Optional. Sender, can be empty for messages sent to channels */\
    qint64 date{}; /* Date the message was sent in Unix time */\
    TelegramBotChat chat; /* Conversation the message belongs to */\
    TelegramBotUser forwardFrom; /* Optional. For forwarded messages, sender of the original message */\
    TelegramBotChat forwardFromChat; /* Optional. For messages forwarded from a channel, information about the original channel */\
    qint64 forwardFromMessageId{}; /* Optional. For forwarded channel posts, identifier of the original message in the channel */\
    qint64 forwardDate{}; /* Optional. For forwarded messages, date the original message was sent in Unix time */\
    qint64 editDate{}; /* Optional. Date the message was last edited in Unix time */\
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
    qint64 migrateToChatId{}; /* Optional. The group has been migrated to a supergroup with the specified identifier. This number may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier. */\
    qint64 migrateFromChatId{}; /* Optional. The supergroup has been migrated from a group with the specified identifier. This number may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier. */\

#define TELEGRAMBOTMESSAGE_FIELD_PARSING \
    JsonHelper::PathGet(object, u"from"_s, from, false); \
    JsonHelper::PathGet(object, u"chat"_s, chat, false); \
    JsonHelper::PathGet(object, u"forward_from"_s, forwardFrom, false); \
    JsonHelper::PathGet(object, u"forward_from_chat"_s, forwardFromChat, false); \
    JsonHelper::PathGet(object, u"forward_from_message_id"_s, forwardFromMessageId, false); \
    JsonHelper::PathGet(object, u"forward_date"_s, forwardDate, false); \
    JsonHelper::PathGet(object, u"edit_date"_s, editDate, false); \
    JsonHelper::PathGet(object, u"text"_s, text, false); \
    JsonHelper::PathGet(object, u"entities"_s, entities, false); \
    JsonHelper::PathGet(object, u"audio"_s, audio, false); \
    JsonHelper::PathGet(object, u"document"_s, document, false); \
    JsonHelper::PathGet(object, u"game"_s, game, false); \
    JsonHelper::PathGet(object, u"photo"_s, photo, false); \
    JsonHelper::PathGet(object, u"sticker"_s, sticker, false); \
    JsonHelper::PathGet(object, u"video"_s, video, false); \
    JsonHelper::PathGet(object, u"voice"_s, voice, false); \
    JsonHelper::PathGet(object, u"caption"_s, caption, false); \
    JsonHelper::PathGet(object, u"contact"_s, contact, false); \
    JsonHelper::PathGet(object, u"location"_s, location, false); \
    JsonHelper::PathGet(object, u"venue"_s, venue, false); \
    JsonHelper::PathGet(object, u"new_chat_member"_s, newChatMember, false); \
    JsonHelper::PathGet(object, u"left_chat_member"_s, leftChatMember, false); \
    JsonHelper::PathGet(object, u"new_chat_title"_s, newChatTitle, false); \
    JsonHelper::PathGet(object, u"new_chat_photo"_s, newChatPhoto, false); \
    JsonHelper::PathGet(object, u"delete_chat_photo = false"_s, deleteChatPhoto, false); \
    JsonHelper::PathGet(object, u"group_chat_created = false"_s, groupChatCreated, false); \
    JsonHelper::PathGet(object, u"supergroup_chat_created = false"_s, supergroupChatCreated, false); \
    JsonHelper::PathGet(object, u"channel_chat_created = false"_s, channelChatCreated, false); \
    JsonHelper::PathGet(object, u"migrate_to_chat_id"_s, migrateToChatId, false); \
    JsonHelper::PathGet(object, u"migrate_from_chat_id"_s, migrateFromChatId, false); \
    return JsonHelper::PathGet(object, u"message_id"_s, messageId) && \
           JsonHelper::PathGet(object, u"date"_s, date);

// TelegramBotMessageSingle - This object represents a message (without any recursive fields which references to same class, see TelegramBotMessage for complete message)
struct TelegramBotMessageSingle : public TelegramBotObject {
  TELEGRAMBOTMESSAGE_FIELDS

  bool FromJson(const QJsonObject& object) override {
    TELEGRAMBOTMESSAGE_FIELD_PARSING
  }
};

// TelegramBotMessage - This object represents a message.
struct TelegramBotMessage : public TelegramBotObject {
  TELEGRAMBOTMESSAGE_FIELDS
  TelegramBotMessageSingle replyToMessage; // Optional. For replies, the original message. Note that the Message object in this field will not contain further reply_to_message fields even if it itself is a reply.
  TelegramBotMessageSingle pinnedMessage; // Optional. Specified message was pinned. Note that the Message object in this field will not contain further reply_to_message fields even if it is itself a reply.

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"reply_to_message"_s, replyToMessage, false);
    JsonHelper::PathGet(object, u"pinned_message"_s, pinnedMessage, false);
    TELEGRAMBOTMESSAGE_FIELD_PARSING
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"message"_s, message, false);
    JsonHelper::PathGet(object, u"inline_message_id"_s, inlineMessageId, false);
    JsonHelper::PathGet(object, u"data"_s, data, false);
    JsonHelper::PathGet(object, u"game_short_name"_s, gameShortName, false);
    return JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"from"_s, from) &&
           JsonHelper::PathGet(object, u"chat_instance"_s, chatInstance);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"location"_s, location, false);
    return JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"from"_s, from) &&
           JsonHelper::PathGet(object, u"query"_s, query) &&
           JsonHelper::PathGet(object, u"offset"_s, offset);
  }
};

// Represents the content of a text message to be sent as the result of an inline query.
struct TelegramBotInputMessageContent : public TelegramBotObject {
  QString messageText; // Text of the message to be sent, 1-4096 characters
  QString parseMode; // Optional. Send Markdown or HTML, if you want Telegram apps to show bold, italic, fixed-width text or inline URLs in your bot's message.
  bool disableWebPagePreview{}; // Optional. Disables link previews for links in the sent message

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"parse_mode"_s, parseMode, false);
    JsonHelper::PathGet(object, u"disable_web_page_preview"_s, disableWebPagePreview, false);
    return JsonHelper::PathGet(object, u"message_text"_s, messageText);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"url"_s, url, false);
    JsonHelper::PathGet(object, u"hide_url"_s, hideUrl, false);
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl, false);
    JsonHelper::PathGet(object, u"thumb_width"_s, thumbWidth, false);
    JsonHelper::PathGet(object, u"thumb_height"_s, thumbHeight, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"photo_width"_s, photoWidth, false);
    JsonHelper::PathGet(object, u"photo_height"_s, photoHeight, false);
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"photo_url"_s, photoUrl) &&
           JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"gif_url"_s, gifUrl) &&
           JsonHelper::PathGet(object, u"gif_width"_s, gifWidth) &&
           JsonHelper::PathGet(object, u"gif_height"_s, gifHeight) &&
           JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"mpeg4Url"_s, mpeg4Url) &&
           JsonHelper::PathGet(object, u"mpeg4Width"_s, mpeg4Width) &&
           JsonHelper::PathGet(object, u"mpeg4Height"_s, mpeg4Height) &&
           JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"video_width"_s, videoWidth, false);
    JsonHelper::PathGet(object, u"video_height"_s, videoHeight, false);
    JsonHelper::PathGet(object, u"video_duration"_s, videoDuration, false);
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"video_url"_s, videoUrl) &&
           JsonHelper::PathGet(object, u"mime_type"_s, mimeType) &&
           JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl) &&
           JsonHelper::PathGet(object, u"title"_s, title);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"performer"_s, performer, false);
    JsonHelper::PathGet(object, u"audio_duration"_s, audioDuration, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"audio_url"_s, audioUrl) &&
           JsonHelper::PathGet(object, u"title"_s, title);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"voice_duration"_s, voiceDuration, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"voice_url"_s, voiceUrl) &&
           JsonHelper::PathGet(object, u"title"_s, title);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl, false);
    JsonHelper::PathGet(object, u"thumb_width"_s, thumbWidth, false);
    JsonHelper::PathGet(object, u"thumb_height"_s, thumbHeight, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"caption"_s, caption) &&
           JsonHelper::PathGet(object, u"document_url"_s, documentUrl) &&
           JsonHelper::PathGet(object, u"mime_type"_s, mimeType);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl, false);
    JsonHelper::PathGet(object, u"thumb_width"_s, thumbWidth, false);
    JsonHelper::PathGet(object, u"thumb_height"_s, thumbHeight, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"latitude"_s, latitude) &&
           JsonHelper::PathGet(object, u"longitude"_s, longitude) &&
           JsonHelper::PathGet(object, u"title"_s, title);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"foursquare_id"_s, foursquareId, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl, false);
    JsonHelper::PathGet(object, u"thumb_width"_s, thumbWidth, false);
    JsonHelper::PathGet(object, u"thumb_height"_s, thumbHeight, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"latitude"_s, latitude) &&
           JsonHelper::PathGet(object, u"longitude"_s, longitude) &&
           JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"address"_s, address);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"last_name"_s, lastName, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    JsonHelper::PathGet(object, u"thumb_url"_s, thumbUrl, false);
    JsonHelper::PathGet(object, u"thumb_width"_s, thumbWidth, false);
    JsonHelper::PathGet(object, u"thumb_height"_s, thumbHeight, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"phone_number"_s, phoneNumber) &&
           JsonHelper::PathGet(object, u"first_name"_s, firstName);
  }
};

// Represents a Game.
struct TelegramBotInlineQueryResultGame : public TelegramBotObject {
  QString type; // Type of the result, must be game
  QString id; // Unique identifier for this result, 1-64 bytes
  QString gameShortName; // Short name of the game
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message (Note: This will only work in Telegram versions released after October 1, 2016. Older clients will not display any inline results if a game result is among them.)

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"game_short_name"_s, gameShortName);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"photo_file_id"_s, photoFileId);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"gif_file_id"_s, gifFileId);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"title"_s, title, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"mpeg4FileId"_s, mpeg4FileId);
  }
};

// Represents a link to a sticker stored on the Telegram servers. By default, this sticker will be sent by the user. Alternatively, you can use inputMessageContent to send a message with the specified content instead of the sticker.
struct TelegramBotInlineQueryResultCachedSticker : public TelegramBotObject {
  QString type; // Type of the result, must be sticker
  QString id; // Unique identifier for this result, 1-64 bytes
  QString stickerFileId; // A valid file identifier of the sticker
  TelegramKeyboard replyMarkup; // Optional. Inline keyboard attached to the message
  TelegramBotInputMessageContent inputMessageContent; // Optional. Content of the message to be sent instead of the sticker (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"sticker_file_id"_s, stickerFileId);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"document_file_id"_s, documentFileId);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"description"_s, description, false);
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"video_file_id"_s, videoFileId) &&
           JsonHelper::PathGet(object, u"title"_s, title);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"voice_file_id"_s, voiceFileId) &&
           JsonHelper::PathGet(object, u"title"_s, title);
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

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"caption"_s, caption, false);
    JsonHelper::PathGet(object, u"reply_markup"_s, replyMarkup, false);
    JsonHelper::PathGet(object, u"input_message_content"_s, inputMessageContent, false);
    return JsonHelper::PathGet(object, u"type"_s, type) &&
           JsonHelper::PathGet(object, u"id"_s, id) &&
           JsonHelper::PathGet(object, u"audio_file_id"_s, audioFileId);
  }
};

// Represents the content of a location message to be sent as the result of an inline query.
struct TelegramBotInputLocationMessageContent : public TelegramBotObject {
  double latitude; // Latitude of the location in degrees
  double longitude; // Longitude of the location in degrees (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  bool FromJson(const QJsonObject& object) override {
    return JsonHelper::PathGet(object, u"latitude"_s, latitude) &&
           JsonHelper::PathGet(object, u"longitude"_s, longitude);
  }
};

// Represents the content of a venue message to be sent as the result of an inline query.
struct TelegramBotInputVenueMessageContent : public TelegramBotObject {
  double latitude; // Latitude of the venue in degrees
  double longitude; // Longitude of the venue in degrees
  QString title; // Name of the venue
  QString address; // Address of the venue
  QString foursquareId; // Optional. Foursquare identifier of the venue, if known (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"foursquare_id"_s, foursquareId, false);
    return JsonHelper::PathGet(object, u"latitude"_s, latitude) &&
           JsonHelper::PathGet(object, u"longitude"_s, longitude) &&
           JsonHelper::PathGet(object, u"title"_s, title) &&
           JsonHelper::PathGet(object, u"address"_s, address);
  }
};

// Represents the content of a contact message to be sent as the result of an inline query.
struct TelegramBotInputContactMessageContent : public TelegramBotObject {
  QString phoneNumber; // Contact's phone number
  QString firstName; // Contact's first name
  QString lastName; // Optional. Contact's last name (Note: This will only work in Telegram versions released after 9 April, 2016. Older clients will ignore them.)

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"last_name"_s, lastName, false);
    return JsonHelper::PathGet(object, u"phone_number"_s, phoneNumber) &&
           JsonHelper::PathGet(object, u"first_name"_s, firstName);
  }
};

// Represents a result of an inline query that was chosen by the user and sent to their chat partner.
struct TelegramBotChosenInlineResult : public TelegramBotObject {
  QString resultId; // The unique identifier for the result that was chosen
  TelegramBotUser from; // The user that chose the result
  TelegramBotLocation location; // Optional. Sender location, only for bots that require user location
  QString inlineMessageId; // Optional. Identifier of the sent inline message. Available only if there is an inline keyboard attached to the message. Will be also received in callback queries and can be used to edit the message.
  QString query; // The query that was used to obtain the result

  bool FromJson(const QJsonObject& object) override {
    JsonHelper::PathGet(object, u"location"_s, location, false);
    JsonHelper::PathGet(object, u"inline_message_id"_s, inlineMessageId, false);
    return JsonHelper::PathGet(object, u"result_id"_s, resultId) &&
           JsonHelper::PathGet(object, u"from"_s, from) &&
           JsonHelper::PathGet(object, u"query"_s, query);
  }
};

// This object represents an incoming update.
struct TelegramBotUpdatePrivate : public TelegramBotObject {
  TelegramBotMessageType type{Undefined};
  qint64 update_id{};

  // Contains the message for the following Update types:
  // Message, EditedMessage, ChannelPost, EditedChannelPost, CallbackQuery
  // Special Case: points also to the Message of a CallbackQuery!
  TelegramBotMessage* message{};
  TelegramBotInlineQuery* inlineQuery{};
  TelegramBotChosenInlineResult* chosenInlineResult{};
  TelegramBotCallbackQuery* callbackQuery{};

  ~TelegramBotUpdatePrivate() {
    if (!callbackQuery && message) delete message;
    if (inlineQuery) delete inlineQuery;
    if (chosenInlineResult) delete chosenInlineResult;
    if (callbackQuery) delete callbackQuery;
  }

  bool FromJson(const QJsonObject& object) override {
    static std::map<QString, TelegramBotMessageType> key_to_type{
        {u"message"_s, Message},
        {u"edited_message"_s, EditedMessage},
        {u"channel_post"_s, ChannelPost},
        {u"edited_channel_post"_s, EditedChannelPost},
        {u"inline_query"_s, InlineQuery},
        {u"chosen_inline_result"_s, ChosenInlineResult},
        {u"callback_query"_s, CallbackQuery} };

    bool found_and_parsed_type{false};
    for (auto itr = object.begin(); itr != object.end(); ++itr) {
      if (itr.key() == u"update_id"_s) {
        update_id = itr.value().toInteger();
        continue;
      }

      auto it_type = key_to_type.find(itr.key());
      if (it_type == key_to_type.cend()) {
        qWarning() << "Invalid type: " << itr.key();
      } else {
        type = it_type->second;
        const QJsonObject jo_message = itr.value().toObject();
        switch (type) {
        case Message:
        case EditedMessage:
        case ChannelPost:
        case EditedChannelPost:
          message = new TelegramBotMessage;
          if (!(found_and_parsed_type = message->FromJson(jo_message))) {
            qCritical() << "Failed to parse TelegramBotMessage";
            return false;
          }
          break;
        case InlineQuery:
          inlineQuery = new TelegramBotInlineQuery;
          if (!(found_and_parsed_type = inlineQuery->FromJson(jo_message))) {
            qCritical() << "Failed to parse TelegramBotInlineQuery";
            return false;
          }
          break;
        case ChosenInlineResult:
          chosenInlineResult = new TelegramBotChosenInlineResult;
          if (!(found_and_parsed_type = chosenInlineResult->FromJson(jo_message))) {
            qCritical() << "Failed to parse TelegramBotChosenInlineResult";
            return false;
          }
          break;
        case CallbackQuery:
          callbackQuery = new TelegramBotCallbackQuery;
          if (!(found_and_parsed_type = callbackQuery->FromJson(jo_message))) {
            qCritical() << "Failed to parse TelegramBotChosenInlineResult";
            return false;
          }
          message = &callbackQuery->message;
          break;
        }
      }
    }
    const bool result = found_and_parsed_type && update_id;
    if (!result) {
      qCritical() << "Update type not found";
    }
    return result;
  }
};
typedef QSharedPointer<TelegramBotUpdatePrivate> TelegramBotUpdate;
