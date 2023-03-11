#pragma once

#include "jsonhelper.h"

struct TelegramBotObject {
  virtual void FromJson(const QJsonObject& object) = 0;
  virtual ~TelegramBotObject() {}
};

template<typename T>
class JsonHelperT<T, typename std::enable_if<std::is_base_of<TelegramBotObject, T>::value>::type> {
public:
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    const QJsonObject object = show_warnings ? JsonHelper::PathGet(data, path).toJsonObject() : JsonHelper::PathGetSilent(data, path).toJsonObject();
    if (object.isEmpty()) return false;
    target.FromJson(object);
    return true;
  }

  static bool PathGet(const QJsonValue& data, const QString& path, std::optional<T>& target, bool show_warnings = true) {
    const QJsonObject object = show_warnings ? JsonHelper::PathGet(data, path).toJsonObject() : JsonHelper::PathGetSilent(data, path).toJsonObject();
    if (object.isEmpty()) return false;
    target = T();
    target->FromJson(object);
    return true;
  }

  static bool jsonPathGetArray(const QJsonValue& data, const QString& path, QList<T>& target, bool show_warnings = true) {
    QJsonArray array = show_warnings ? JsonHelper::PathGet(data, path).toJsonArray() : JsonHelper::PathGetSilent(data, path).toJsonArray();
    if (array.isEmpty()) return false;
    for (auto itr = array.begin(); itr != array.end(); itr++) {
      JsonHelperT::PathGet(*itr, u""_qs, *target.insert(target.end(), T{}));
    }
    return true;
  }

  static inline bool jsonPathGetArrayArray(const QJsonValue& data, const QString& path, QList<QList<T>>& target, bool show_warnings = true) {
    const QJsonArray array = show_warnings ? JsonHelper::PathGet(data, path).toJsonArray() : JsonHelper::PathGetSilent(data, path).toJsonArray();
    if (array.isEmpty()) return false;
    for (auto itr = array.begin(); itr != array.end(); itr++) {
      JsonHelperT::jsonPathGetArray(*itr, u""_qs, *target.insert(target.end(), QList<T>()));
    }
    return true;
  }
};
