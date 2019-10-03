#include "qqq.h"
#include <sstream>
#include <qapplication.h>
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qwidget.h>
#include <qwindow.h>
#include <qlabel.h>
#include <qdebug.h>
#include <qjsondocument.h>
InitClass::InitClass()
{
    // Move object to UI thread and emit timer call, to process all from it
    // because any write actions with Qml must be from UI thread
    moveToThread(qApp->thread());
    QTimer::singleShot(0, this, SLOT(Initialize()));
}

void RecurciveJSONFill(QQmlContext* context, QObject* object, QJsonObject& json, const std::string& path)
{
    const QMetaObject* meta_obg = object->metaObject();
    json["class_name"] = meta_obg->className();
    std::string this_path = path + "/" + meta_obg->className();
    json["path"] = QString::fromStdString(path);
    json["id"] = context->nameForObject(object);

    QJsonObject class_info;
    for (int i = 0; i < meta_obg->classInfoCount(); ++i)
    {
        std::string name = meta_obg->classInfo(i).name();
        class_info[name.c_str()] = meta_obg->classInfo(i).value();
    }
    json["class_info"] = class_info;

    QJsonObject methods;
    for (int i = 0; i < meta_obg->methodCount(); ++i)
    {
        std::string name = meta_obg->method(i).name();
        methods[name.c_str()] = QString::fromStdString(meta_obg->method(i).methodSignature().toStdString());
    }
    json["methods"] = methods;

    QJsonObject properties;
    for (int i = 0; i < meta_obg->propertyCount(); ++i)
    {
        std::string name = meta_obg->property(i).name();
        auto value = object->property(name.c_str());
        properties[name.c_str()] = QJsonValue::fromVariant(value);
    }
    json["properties"] = properties;

    QJsonArray children;
    for (const auto& child : object->children())
    {
        QJsonObject child_val;
        RecurciveJSONFill(context, child, child_val, this_path);
        children.append(child_val);
    }
    json["children"] = children;
}

void InitClass::Initialize()
{
    // Get application singleton and cast to QGuiApplication to get get Windows
    QApplication* app = dynamic_cast<QApplication*>(QCoreApplication::instance());
    if (!app)
        return;

    // Find any qml window from the children list
    // TODO: support multiple windows
    QQmlContext* context = nullptr;
    QWindow* qml_window = nullptr;
    for (const auto& window : app->allWindows())
    {
        //window->dumpObjectTree();
        context = QQmlEngine::contextForObject(window);
        if (context)
        {
            qml_window = window;
            break;
        }
    }

    if (!context)
        return;

    // Get QmlEngiine instance
    QQmlEngine* engine = context->engine();
    //engine->dumpObjectTree();

    // QML:
    //
    // Label
    // {
    //     objectName: "some_label"
    //     ...
    // }

    QJsonObject window_info;
    window_info["window_name"] = qml_window->title();
    RecurciveJSONFill(context, qml_window, window_info, qml_window->metaObject()->className());

    QJsonDocument doc(window_info);
    std::string result = doc.toJson(QJsonDocument::Indented).toStdString();

    QObject* label = qml_window->findChild<QObject*>("some_label");
    if (!label)
        return;

    label->setProperty("text", "Hello from pywinauto!");
}