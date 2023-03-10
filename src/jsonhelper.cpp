#include "jsonhelper.h"

QVariant JsonHelper::PathGetImpl(QJsonValue data, const QString& path, bool show_warnings) {
  QStringList splitted_path = path.split('.', Qt::SkipEmptyParts);
  QStringList processed;
  while (!splitted_path.isEmpty()) {
    const QString path_element = splitted_path.takeFirst();
    processed += path_element;

    if (data.isUndefined()) return data;

    if (data.type() == QJsonValue::Array) {
      bool is_int{false};
      int int_path = path_element.toInt(&is_int);
      QJsonArray j_array = data.toArray();
      if (!is_int) {
        if (show_warnings) qWarning("JsonHelper::jsonPath: pos: %s -> QJsonArray path element is non numeric",  qPrintable(processed.join(".")));
        int_path = -1;
      } else if (show_warnings && j_array.count() <= int_path) {
        if (j_array.count() >= 1) {
          qWarning("JsonHelper::jsonPath: pos: %s -> QJsonArray with length %i, is smaller than requested",  qPrintable(processed.join(".")), j_array.count());
        } else {
          qWarning("JsonHelper::jsonPath: pos: %s -> QJsonArray is empty so cannot access requested index",  qPrintable(processed.join(".")));
        }
      }
      data = j_array.at(int_path);
    } else if (data.type() == QJsonValue::Object) {
      QJsonObject j_object = data.toObject();
      if (show_warnings && !j_object.contains(path_element)) {
        qWarning("JsonHelper::jsonPath: pos: %s -> QJsonObject path element not found",  qPrintable(processed.join(".")));
      }
      data = j_object.value(path_element);
    } else {
      if (show_warnings && !splitted_path.isEmpty()) {
        qWarning("JsonHelper::jsonPath: pos: %s -> Reached data end, but still path data available (%s)", qPrintable(processed.join(".")), qPrintable(splitted_path.join(".")));
      }
      return data.toVariant();
    }
  }

  // if we reach this point we have not reached the tail of the json object, so we just return the current position (correct converted to the right type!), so that the user can continue process
  return data.isObject() ? QVariant::fromValue(data.toObject()) :
         data.isArray() ? QVariant::fromValue(data.toArray()) :
                           QVariant::fromValue(data);
}
