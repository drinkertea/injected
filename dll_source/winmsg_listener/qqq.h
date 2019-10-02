#pragma once
#include <QtCore>
#include <qobject.h>

class InitClass : public QObject
{
      Q_OBJECT
public slots:
    void Initialize();
public:
    InitClass();

};