#ifndef FWIDGETBASE_H
#define FWIDGETBASE_H

#import "../global.h"
#import "../fscanner.h"

#include <QWidget>

class FWidgetBase : public QWidget
{
    Q_OBJECT
public:
    FWidgetBase(QWidget* parent);
    virtual ~FWidgetBase();

    virtual const char* getTitle() const = 0;
    virtual bool isValid() const { return false; }

    virtual void widgetWillAppear() {};
    virtual void widgetWillDisappear() {};

    virtual unique_ptr<FScanner> takeScanner();
    virtual bool hydrateScanner(unique_ptr<FScanner> scanner);

protected:
    unique_ptr<FScanner> m_scanner;

signals:
    void validityChanged();

};

#endif // FWIDGETBASE_H
