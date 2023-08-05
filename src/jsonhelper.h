#pragma once

#include "common.h"

struct BOT_EXPORT TelegramBotObject {
  virtual void FromJson(const QJsonObject& object) = 0;
  virtual ~TelegramBotObject() {}
};

class BOT_EXPORT JsonHelper {
public:
  template<typename T, typename std::enable_if<
    std::is_same<
      std::bool_constant<true>, typename std::negation<std::is_base_of<TelegramBotObject, T>>::type
    >::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    const QVariant j_path_value = JsonHelper::PathGetImpl(data, path, show_warnings);
    target = j_path_value.value<T>();
    return j_path_value != QJsonValue(QJsonValue::Undefined) && !j_path_value.canConvert<T>();
  }

  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    const QJsonObject object = show_warnings ? JsonHelper::PathGet(data, path).toJsonObject() : JsonHelper::PathGetSilent(data, path).toJsonObject();
    if (object.isEmpty()) return false;
    target.FromJson(object);
    return true;
  }

  template<typename T, typename std::enable_if<std::is_same<std::bool_constant<true>, typename std::negation<std::is_base_of<TelegramBotObject, T>>::type>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, std::optional<T>& target, bool show_warnings = true) {
    const QVariant j_path_value = JsonHelper::PathGetImpl(data, path, show_warnings);
    if (j_path_value == QJsonValue(QJsonValue::Undefined) || !j_path_value.canConvert<T>()) {
      return false;
    }
    target = j_path_value.value<T>();
    return true;
  }

  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, std::optional<T>& target, bool show_warnings = true) {
    const QJsonObject object = show_warnings ? JsonHelper::PathGet(data, path).toJsonObject() : JsonHelper::PathGetSilent(data, path).toJsonObject();
    if (object.isEmpty()) return false;
    target = T();
    target->FromJson(object);
    return true;
  }

  template<typename T, typename std::enable_if<std::is_same<std::bool_constant<true>, typename std::negation<std::is_base_of<TelegramBotObject, T>>::type>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, QList<T>& target, bool show_warnings = true) {
    QJsonValue value = show_warnings ? JsonHelper::PathGet(data, path).toJsonValue() : JsonHelper::PathGetSilent(data, path).toJsonValue();
    if (value.isArray()) {
      QJsonArray jArray = value.toArray();
      int idx{0};
      for (auto itr = jArray.begin(); itr != jArray.end(); itr++) {
        JsonHelper::PathGet<T>(*itr, QString::number(idx++), *target.insert(target.end(), T{}), show_warnings);
      }
      return true;
    }
    if (value.isObject()) {
      QJsonObject jObject = value.toObject();
      for (auto itr = jObject.begin(); itr != jObject.end(); itr++) {
        JsonHelper::PathGet<T>(*itr, itr.key(), *target.insert(target.end(), T{}), show_warnings);
      }
      return true;
    }

    return false;
  }
  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static bool PathGet(const QJsonValue& data, const QString& path, QList<T>& target, bool show_warnings = true) {
    QJsonArray array = show_warnings ? JsonHelper::PathGet(data, path).toJsonArray() : JsonHelper::PathGetSilent(data, path).toJsonArray();
    if (array.isEmpty()) return false;
    for (auto itr = array.begin(); itr != array.end(); itr++) {
      JsonHelper::PathGet<T>(*itr, u""_s, *target.insert(target.end(), T{}));
    }
    return true;
  }

  template<typename T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value, bool>::type = true>
  static inline bool PathGet(const QJsonValue& data, const QString& path, QList<QList<T>>& target, bool show_warnings = true) {
    const QJsonArray array = show_warnings ? JsonHelper::PathGet(data, path).toJsonArray() : JsonHelper::PathGetSilent(data, path).toJsonArray();
    if (array.isEmpty()) return false;
    for (auto itr = array.begin(); itr != array.end(); itr++) {
      JsonHelper::PathGet(*itr, u""_s, *target.insert(target.end(), QList<T>()));
    }
    return true;
  }

  static QVariant PathGet(const QJsonValue& data, const QString& path) {
    return JsonHelper::PathGetImpl(data, path, true);
  }
  static QVariant PathGetSilent(const QJsonValue& data, const QString& path) {
    return JsonHelper::PathGetImpl(data, path, false);
  }

private:
  static QVariant PathGetImpl(QJsonValue data, const QString& path, bool show_warnings);
};
