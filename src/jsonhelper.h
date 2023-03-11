#pragma once

#include "common.h"

class BOT_EXPORT JsonHelper {
public:
  template<typename T>
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    const QVariant j_path_value = JsonHelper::PathGetImpl(data, path, show_warnings);
    target = j_path_value.value<T>();
    return j_path_value != QJsonValue(QJsonValue::Undefined) && !j_path_value.canConvert<T>();
  }
  template<typename T>
  static bool PathGet(const QJsonValue& data, const QString& path, std::optional<T>& target, bool show_warnings = true) {
    const QVariant j_path_value = JsonHelper::PathGetImpl(data, path, show_warnings);
    if (j_path_value == QJsonValue(QJsonValue::Undefined) || !j_path_value.canConvert<T>()) {
      return false;
    }
    target = j_path_value.value<T>();
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

template<typename T, class Enable = void>
class JsonHelperT {
public:
  static bool PathGet(const QJsonValue& data, const QString& path, T& target, bool show_warnings = true) {
    return JsonHelper::PathGet(data, path, target, show_warnings);
  }

  static bool PathGet(const QJsonValue& data, const QString& path, std::optional<T>& target, bool show_warnings = true) {
    return JsonHelper::PathGet(data, path, target, show_warnings);
  }

  static bool jsonPathGetArray(const QJsonValue& data, const QString& path, QList<T>& target, bool show_warnings = true) {
    QJsonValue value = show_warnings ? JsonHelper::PathGet(data, path).toJsonValue() : JsonHelper::PathGetSilent(data, path).toJsonValue();
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
    QJsonValue value = show_warnings ? JsonHelper::PathGet(data, path).toJsonValue() : JsonHelper::PathGetSilent(data, path).toJsonValue();
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

  static QVariant PathGet(const QJsonValue& data, const QString& path) {
    return JsonHelper::PathGet(data, path);
  }

  static QVariant PathGetSilent(const QJsonValue& data, const QString& path) {
    return JsonHelper::PathGetSilent(data, path);
  }
};
