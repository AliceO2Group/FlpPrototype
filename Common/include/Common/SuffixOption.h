/// \file SuffixOption.h
/// \brief Definition of the SuffixOption class.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#ifndef ALICEO2_FLPPROTOTYPE_COMMON_SUFFIXOPTION_H_
#define ALICEO2_FLPPROTOTYPE_COMMON_SUFFIXOPTION_H_

#include <string>
#include <boost/program_options/value_semantic.hpp>
#include "Common/SuffixNumber.h"

namespace AliceO2 
{
namespace Common
{

/// Wrapper around SuffixNumber to use it with boost::program_options
/// Example:
/// myOptions.add_options()("my-option", Common::SuffixOption<uint64_t>::make(&myOption)->default_value("10k"),
///     "An option for a number");
template <typename Number>
class SuffixOption final : public boost::program_options::value_semantic
{
  public:
    using ThisType = SuffixOption<Number>;
    using SuffixNumberType = SuffixNumber<Number>;

    SuffixOption(SuffixNumberType* storeTo = nullptr) : mStoreToSuffixNumber(storeTo)
    {
    }

    SuffixOption(Number* storeTo = nullptr) : mStoreToNumber(storeTo)
    {
    }

    ThisType* required()
    {
      mRequired = true;
      return this;
    }

    ThisType* default_value(std::string defaultValue)
    {
      mHasDefault = true;
      mName = std::string("(=" + defaultValue + ")");
      mDefaultValue.setNumber(defaultValue);
      return this;
    }

    ThisType* default_value(Number defaultValue)
    {
      mHasDefault = true;
      mName = std::string("(=" + std::to_string(defaultValue) + ")");
      mDefaultValue.setNumber(defaultValue);
      return this;
    }


    virtual ~SuffixOption()
    {
    }

    virtual std::string name() const override
    {
      return mName;
    }

    virtual unsigned min_tokens() const override
    {
      return 1;
    }

    virtual unsigned max_tokens() const override
    {
      return 16;
    }

    virtual bool adjacent_tokens_only() const override
    {
      return true;
    }

    virtual bool is_composing() const override
    {
      return false;
    }

    virtual bool is_required() const override
    {
      return mRequired;
    }

    virtual void parse(boost::any& valueStore, const std::vector<std::string>& newTokens, bool) const override
    {
      valueStore = SuffixNumberType(newTokens.at(0));
    }

    virtual bool apply_default(boost::any& value) const override
    {
      if (!mHasDefault) {
        return false;
      }
      value = mDefaultValue;
      return true;
    }

    virtual void notify(const boost::any& value) const override
    {
      if (mStoreToSuffixNumber) {
        *mStoreToSuffixNumber = boost::any_cast<SuffixNumberType>(value);
      }

      if (mStoreToNumber) {
        *mStoreToNumber = boost::any_cast<SuffixNumberType>(value).getNumber();
      }
    }

    static ThisType* make(SuffixNumberType* number)
    {
      return new ThisType(number);
    }

    static ThisType* make(Number* number)
    {
      return new ThisType(number);
    }

  private:
    Number* mStoreToNumber = nullptr;
    SuffixNumberType* mStoreToSuffixNumber = nullptr;
    bool mRequired = false;
    bool mHasDefault = false;
    SuffixNumberType mDefaultValue;
    std::string mName;
};

} // namespace Common
} // namespace AliceO2

#endif // ALICEO2_FLPPROTOTYPE_COMMON_SUFFIXOPTION_H_
