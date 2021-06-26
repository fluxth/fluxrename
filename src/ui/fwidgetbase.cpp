#include "fwidgetbase.h"


FWidgetBase::FWidgetBase(QWidget* parent) :
    QWidget(parent),
    m_scanner(nullptr)
{

}

FWidgetBase::~FWidgetBase()
{

}

unique_ptr<FScanner> FWidgetBase::takeScanner()
{
    return std::move(m_scanner);
}

bool FWidgetBase::hydrateScanner(unique_ptr<FScanner> scanner)
{
    m_scanner.reset(scanner.release());
    return true;
}
