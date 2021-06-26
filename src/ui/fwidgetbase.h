#ifndef FWIDGETBASE_H
#define FWIDGETBASE_H

#import "../global.h"

#include <QWidget>

class FWidgetBase : public QWidget
{
    Q_OBJECT
public:
    FWidgetBase(QWidget* parent) : QWidget(parent) {}
    virtual ~FWidgetBase() {}

    virtual const char* getTitle() const = 0;
    virtual bool isValid() const { return false; }

signals:
    void validityChanged();

};

#endif // FWIDGETBASE_H
