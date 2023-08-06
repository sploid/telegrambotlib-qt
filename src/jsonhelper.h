#pragma once

#include "common.h"

struct BOT_EXPORT TelegramBotObject {
  virtual bool FromJson(const QJsonObject& object) = 0;
  virtual ~TelegramBotObject() {}
};

class BOT_EXPORT JsonHelper {
public:
  template<typename T, typename std::enable_if<
    std::is_same<
      std::bool_constant<true>, typename std::negation<std::is_base_of<TelegramBotObject, T>>::type
    >::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    const QJsonValue j_val = JsonHelper::PathGetImplJ(data, path, show_warnings);
    if (j_val.isUndefined() || j_val.isNull()) {
      if (show_warnings) qWarning() << "Path not found: " << path;
      return false;
    }
    const QVariant var_val{QVariant::fromValue(j_val)};
    if (!var_val.canConvert<T>()) {
      if (show_warnings) qWarning() << "Cannot convert json to type: " << path;
      return false;
    }
    target = var_val.value<T>();
    return true;
  }

  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    const QJsonValue j_val = JsonHelper::PathGetImplJ(data, path, show_warnings);
    if (j_val.isUndefined() || j_val.isNull() || !j_val.isObject()) {
      if (show_warnings) qWarning() << "Invalid path for object: " << path;
      return false;
    }

    if (!target.FromJson(j_val.toObject())) {
      if (show_warnings) qWarning() << "Failed to parse object: " << path;
      return false;
    }

    return true;
  }

  template<typename T, typename std::enable_if<std::is_same<std::bool_constant<true>, typename std::negation<std::is_base_of<TelegramBotObject, T>>::type>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, QList<T>& target, bool show_warnings = true) {
    const QJsonValue j_val = JsonHelper::PathGetImplJ(data, path, show_warnings);
    if (j_val.isUndefined() || j_val.isNull() || (!j_val.isArray() && !j_val.isObject())) {
      if (show_warnings) qWarning() << "Path not found or invalid: " << path;
      return false;
    }
    if (j_val.isArray()) {
      const QJsonArray j_array = j_val.toArray();
      int idx{0};
      for (auto itr = j_array.begin(); itr != j_array.end(); itr++) {
        if (!JsonHelper::PathGet<T>(*itr, QString::number(idx++), *target.insert(target.end(), T{}), show_warnings) && show_warnings) {
          if (show_warnings) qWarning() << "Failed to parse index in path: " << path << idx;
          return false;
        }
      }
      return true;
    } else if (j_val.isObject()) {
      const QJsonObject j_object = j_val.toObject();
      for (auto itr = j_object.begin(); itr != j_object.end(); itr++) {
        if (!JsonHelper::PathGet<T>(*itr, itr.key(), *target.insert(target.end(), T{}), show_warnings) && show_warnings) {
          if (show_warnings) qWarning() << "Failed to object in path: " << path << itr.key();
          return false;
        }
      }
      return true;
    }

    qWarning() << "Invalid JSON object: " << path;
    return false;
  }

  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, QList<T>& target, bool show_warnings = true) {
    const QJsonValue j_val = JsonHelper::PathGetImplJ(data, path, show_warnings);
    if (j_val.isUndefined() || j_val.isNull() || !j_val.isArray()) {
      if (show_warnings) qWarning() << "Path not found or invalid: " << path;
      return false;
    }
    const QJsonArray j_arr{j_val.toArray()};
    for (auto itr = j_arr.begin(); itr != j_arr.end(); ++itr) {
      if (!JsonHelper::PathGet<T>(*itr, u""_s, *target.insert(target.end(), T{})) && show_warnings) {
        qWarning() << "Failed to parse " << path;
        return false;
      }
    }
    return true;
  }

  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static inline bool PathGet(const QJsonValue& data, const QString& path, QList<QList<T>>& target, bool show_warnings = true) {
    const QJsonValue j_val = JsonHelper::PathGetImplJ(data, path, show_warnings);
    if (j_val.isUndefined() || j_val.isNull() || !j_val.isArray()) {
      if (show_warnings) qWarning() << "Path not found or invalid: " << path;
      return false;
    }
    const QJsonArray j_array{j_val.toArray()};
    for (auto itr = j_array.begin(); itr != j_array.end(); itr++) {
      if (!JsonHelper::PathGet(*itr, u""_s, *target.insert(target.end(), QList<T>())) && show_warnings) {
        qWarning() << "Failed to parse " << path;
        return false;
      }
    }
    return true;
  }

  static QJsonValue PathGet(const QJsonValue& data, const QString& path) {
    return JsonHelper::PathGetImplJ(data, path, true);
  }

private:
  static QJsonValue PathGetImplJ(QJsonValue data, const QString& path, bool show_warnings);
};
