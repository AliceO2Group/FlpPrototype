///
/// @file   DataSampling.h
/// @author Barthelemy von Haller
///

#ifndef DATASAMPLING_LIBS_DATASAMPLING_DATASAMPLING_H_
#define DATASAMPLING_LIBS_DATASAMPLING_DATASAMPLING_H_

#include <string>
#include <iostream>

#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/seq.hpp>

namespace AliceO2 {
namespace DataSampling {

#define DEFINE_ENUM(name, values)                               \
  enum name {                                                   \
    BOOST_PP_SEQ_FOR_EACH(DEFINE_ENUM_VALUE, , values)          \
  };                                                            \
  const char* format_##name(name val) {                         \
    switch (val) {                                              \
      BOOST_PP_SEQ_FOR_EACH(DEFINE_ENUM_FORMAT, , values)       \
    default:                                                    \
        return 0;                                               \
    }                                                           \
  }

#define DEFINE_ENUM_VALUE(r, data, elem)                        \
  BOOST_PP_SEQ_HEAD(elem)                                       \
  BOOST_PP_IIF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(elem), 2),      \
               = BOOST_PP_SEQ_TAIL(elem), )                     \
  BOOST_PP_COMMA()

#define DEFINE_ENUM_FORMAT(r, data, elem)             \
  case BOOST_PP_SEQ_HEAD(elem):                       \
  return BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(elem));

DEFINE_ENUM(Policy,
            ((Must)(0))
            ((Percent))
            ((Most)))

    DEFINE_ENUM(DataFormat,
                ((Raw)(0))
                ((STF))
                ((TF))
                ((CTF))
                ((AOD)))

//enum class Policy
//{
//  Must, Percent, Most
//};
//enum class DataFormat
//{
//  Raw, STF, TF, CTF, AOD
//};

class DataSampling
{
  public:
    DataSampling();
    DataSampling(std::string Uri);
    virtual ~DataSampling();

    void* getData(int timeout); // TODO add specific types ? what do we get back ?
    void releaseData();

    void setLocation(std::string Uri);
    std::string getLocation() const;
    void setPolicy(Policy policy);
    Policy getPolicy() const;
    void setDataFormat(DataFormat format);
    DataFormat getDataFormat() const;
//    void setFilter();

  private:
    std::string m_location;
    Policy m_policy;
    DataFormat m_format;
};

} /* namespace DataSampling */
} /* namespace AliceO2 */

#endif /* DATASAMPLING_LIBS_DATASAMPLING_DATASAMPLING_H_ */
