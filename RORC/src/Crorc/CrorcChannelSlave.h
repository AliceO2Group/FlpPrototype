/// \file CrorcChannelSlave.h
/// \brief Definition of the CrorcChannelMaster class.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#pragma once

#include "ChannelSlave.h"

namespace AliceO2 {
namespace ReadoutCard {

/// TODO Currently a placeholder
class CrorcChannelSlave: public ChannelSlave
{
  public:

    CrorcChannelSlave(const Parameters& parameters);
    virtual ~CrorcChannelSlave();
    virtual CardType::type getCardType();
};

} // namespace ReadoutCard
} // namespace AliceO2
