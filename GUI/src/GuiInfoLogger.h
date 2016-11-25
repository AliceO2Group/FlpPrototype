///
/// \file GuiInfoLogger.h
/// \author Barthelemy von Haller
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#ifndef MONITORING_MONINFOLOGGER_H
#define MONITORING_MONINFOLOGGER_H

#include <iostream>
#include <InfoLogger/InfoLogger.hxx>

namespace AliceO2
{
/// ALICE O2 GUI
namespace GUI
{

/// Singleton class that is used in all Monitoring classes
///
/// The aim of this class is to avoid every class in the package to define and configure its own instance of InfoLogger.
/// Independent InfoLogger instance can still be created when and if needed.
class GuiInfoLogger : public AliceO2::InfoLogger::InfoLogger
{
  public:
    static GuiInfoLogger &GetInstance()
    {
      static GuiInfoLogger foo;
      return foo;
    }

  private:
    GuiInfoLogger()
    {
      *this << "InfoLogger instance for Monitoring initialized" << AliceO2::InfoLogger::InfoLogger::endm;
    }

    virtual ~GuiInfoLogger()
    {
    }

    /// Delete copy constructors
    GuiInfoLogger &operator=(const GuiInfoLogger &) = delete;
    GuiInfoLogger(const GuiInfoLogger &) = delete;

};

} // GUI
} // Alice02

#endif //MONITORING_MONINFOLOGGER_H
