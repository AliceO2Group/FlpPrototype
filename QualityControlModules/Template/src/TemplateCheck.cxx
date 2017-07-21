///
/// \file   TemplateCheck.cxx
/// \author Barthelemy von Haller
///

#include "Template/TemplateCheck.h"

// std
#include <cassert>
#include <iostream>
// ROOT
#include <TH1.h>

using namespace std;

ClassImp(AliceO2::QualityControlModules::Template::TemplateCheck)

namespace AliceO2 {
namespace QualityControlModules {
namespace Template {

TemplateCheck::TemplateCheck()
{
}

TemplateCheck::~TemplateCheck()
{
}

void TemplateCheck::configure(std::string name)
{
  // First call the parent !
  CheckInterface::configure(name);
}

Quality TemplateCheck::check(const MonitorObject *mo)
{
  Quality result = Quality::Null;

  return result;
}

std::string TemplateCheck::getAcceptedType()
{
  return "TH1";
}

void TemplateCheck::beautify(MonitorObject *mo, Quality checkResult)
{
// NOOP
}

}  // namespace Template
}  // namespace QualityControl
} // namespace AliceO2

