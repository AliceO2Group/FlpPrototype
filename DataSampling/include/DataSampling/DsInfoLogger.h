///
/// \file    DsInfoLogger.h
/// \author  Barthelemy von Haller
///


#ifndef QUALITY_CONTROL_DSINFOLOGGER_H
#define QUALITY_CONTROL_DSINFOLOGGER_H

#include <iostream>
#include <InfoLogger/InfoLogger.hxx>

typedef AliceO2::InfoLogger::InfoLogger infologger; // not to have to type the full stuff each time -> log::endm

namespace AliceO2 {
namespace DataSampling {

/// \brief  Singleton class that any class in the DataSampling can use to log.
///
/// The aim of this class is to avoid every class in the package to define
/// and configure its own instance of InfoLogger.
/// Independent InfoLogger instances can still be created when and if needed.
///
/// Usage :   DsInfoLogger::GetInstance() << "blabla" << infologger::endm;
///
/// \author Barthelemy von Haller
class DsInfoLogger : public AliceO2::InfoLogger::InfoLogger
{

  public:

    static DsInfoLogger &getInstance()
    {
      // Guaranteed to be destroyed. Instantiated on first use
      static DsInfoLogger foo;
      return foo;
    }

  private:

    DsInfoLogger()
    {
      // TODO configure the DataSampling infologger, e.g. proper facility
      *this << "DataSampling infologger initialized" << infologger::endm;
    }

    virtual ~DsInfoLogger()
    {
    }

    // Disallow copying
    DsInfoLogger &operator=(const DsInfoLogger &) = delete;

    DsInfoLogger(const DsInfoLogger &) = delete;
};

}
}

#endif //QUALITY_CONTROL_DSINFOLOGGER_H
