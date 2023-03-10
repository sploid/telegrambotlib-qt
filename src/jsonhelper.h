#pragma once

#include "common.h"

class BOT_EXPORT JsonHelper {
public:
  // main json parser
  template<typename T>
  static bool PathGet(QJsonValue data, QString path, T& target, bool show_warnings = true) {
    // get value and exit if value is invalid, or is not convertable to T
    QVariant jPathValue = JsonHelper::PathGetImpl(data, path, show_warnings);
    target = jPathValue.value<T>();

    // return true if convert was successfull otherwise false
    return jPathValue != QJsonValue(QJsonValue::Undefined) && !jPathValue.canConvert<T>();
  }
  static QVariant PathGet(QJsonValue data, QString path) { return JsonHelper::PathGetImpl(data, path, true); }
  static QVariant jsonPathGetSilent(QJsonValue data, QString path) { return JsonHelper::PathGetImpl(data, path, false); }

private:
    static QVariant PathGetImpl(QJsonValue data, const QString& path, bool show_warnings);
};

template<typename T, class Enable = void>
class JsonHelperT {
public:
  static bool PathGet(QJsonValue data, QString path, T& target, bool show_warnings = true) {
      return JsonHelper::PathGet(data, path, target, show_warnings);
  }

  static bool jsonPathGetArray(QJsonValue data, QString path, QList<T>& target, bool show_warnings = true) {
    QJsonValue value = show_warnings ? JsonHelper::PathGet(data, path).toJsonValue() : JsonHelper::jsonPathGetSilent(data, path).toJsonValue();
    if (value.isArray()) {
      QJsonArray jArray = value.toArray();
      int idx{0};
      for (auto itr = jArray.begin(); itr != jArray.end(); itr++) {
        JsonHelper::PathGet(*itr, QString::number(idx++), *target.insert(target.end(), T{}), show_warnings);
      }
      return true;
    }
    if (value.isObject()) {
      QJsonObject jObject = value.toObject();
      for (auto itr = jObject.begin(); itr != jObject.end(); itr++) {
        JsonHelper::PathGet(*itr, itr.key(), *target.insert(target.end(), T{}), show_warnings);
      }
      return true;
    }

    return false;
  }

  static bool jsonPathGetArrayArray(QJsonValue data, QString path, QList<QList<T>>& target, bool show_warnings = true) {
    QJsonValue value = show_warnings ? JsonHelper::PathGet(data, path).toJsonValue() : JsonHelper::jsonPathGetSilent(data, path).toJsonValue();
    if (value.isArray()) {
      QJsonArray jArray = value.toArray();
      int idx{0};
      for (auto itr = jArray.begin(); itr != jArray.end(); itr++) {
        JsonHelperT::jsonPathGetArray(*itr, QString::number(idx++), *target.insert(target.end(), QList<T>()), show_warnings);
      }
    } else if (value.isObject()) {
      QJsonObject jObject = value.toObject();
      for (auto itr = jObject.begin(); itr != jObject.end(); itr++) {
        JsonHelperT::jsonPathGetArray(*itr, itr.key(), *target.insert(target.end(), QList<T>()), show_warnings);
      }
    } else {
      return false;
    }
    return true;
  }

  static QVariant PathGet(QJsonValue data, QString path) {
    return JsonHelper::PathGet(data, path);
  }

  static QVariant jsonPathGetSilent(QJsonValue data, QString path) {
    return JsonHelper::jsonPathGetSilent(data, path);
  }
};
