#include "qqq.h"
#include <sstream>
#include <qapplication.h>
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qwidget.h>
#include <qwindow.h>
#include <qlabel.h>
InitClass::InitClass()
{
    // Move object to UI thread and emit timer call, to process all from it
    // because any write actions with Qml must be from UI thread
    moveToThread(qApp->thread());
    QTimer::singleShot(0, this, SLOT(Initialize()));
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
        window->dumpObjectTree();
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
    engine->dumpObjectTree();

    // QML:
    //
    // Label
    // {
    //     objectName: "some_label"
    //     ...
    // }
    QObject* label = qml_window->findChild<QObject*>("some_label");
    if (!label)
        return;

    label->setProperty("text", "Hello freom pywinauto!");
}