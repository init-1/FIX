#ifndef FIX_FIELD_HPP_
#define FIX_FIELD_HPP_

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/optional.hpp>
#include <string> // std::string
#include <vector> // std::vector

namespace FIX {

typedef unsigned int uint;
typedef unsigned long ulong;

namespace qi = boost::spirit::qi;
namespace karma = boost::spirit::karma;

template <typename T> struct Rules;

template<> struct Rules<char>
{
  static karma::char_type Encode() { return karma::char_; }
  static qi::char_type    Decode() { return qi::char_; }
};

template<> struct Rules<int>
{
  static karma::int_type Encode() { return karma::int_; }
  static qi::int_type    Decode() { return qi::int_; }
};

template<> struct Rules<long>
{
  static karma::long_type Encode() { return karma::long_; }
  static qi::long_type    Decode() { return qi::long_; }
};

template<> struct Rules<uint>
{
  static karma::uint_type Encode() { return karma::uint_; }
  static qi::uint_type    Decode() { return qi::uint_; }
};

template<> struct Rules<ulong>
{
  static karma::ulong_type Encode() { return karma::ulong_; }
  static qi::ulong_type    Decode() { return qi::ulong_; }
};

template<> struct Rules<double>
{
  static karma::double_type Encode() { return karma::double_; }
  static qi::double_type    Decode() { return qi::double_; }
};

template<> struct Rules<std::string> // using standard encoding
{
  static karma::string_type Encode() { return karma::string; }
  static qi::string_type    Decode() { return qi::string; }
};

/*
template<> struct Rules<char *>
{
  static karma::string_type Encode() { return karma::string_; }
  static qi::string_type    Decode() { return qi::string_; }
};
*/

template<> struct Rules<bool>
{
  struct BoolPolicy : karma::bool_policies<bool> {
    template <typename CharEncoding, typename Tag, typename Iterator>
    static bool generate_false(Iterator& sink, bool b) {
      return karma::string_inserter<CharEncoding, Tag>::call(sink, "N");
    }

    template <typename CharEncoding, typename Tag, typename Iterator>
    static bool generate_true(Iterator& sink, bool b) {
      return karma::string_inserter<CharEncoding, Tag>::call(sink, "Y");
    }
  };

  typedef karma::bool_generator<bool, BoolPolicy> bool_generator;
  typedef qi::symbols<char, bool> bool_extractor;
 
  static bool_generator Encode() { return bool_generator(); }
  static bool_extractor Decode() { bool_extractor sym; sym.add("Y",true)("N",false); return sym; }
};


// Specific Rules (not templated)

struct ByteRules
{
  static karma::byte_type Encode() { return karma::byte_; }
  static qi::byte_type    Decode() { return qi::byte_; }
};

struct MultipleCharValueRules 
{
  static auto Encode() -> decltype(karma::char_ % ' ') { return karma::char_ % ' '; }
  static auto Decode() -> decltype(qi::char_ % ' ') { return qi::char_ % ' '; }
};

struct MultipleStringValueRules 
{
  static auto Encode() -> decltype(karma::string % ' ') { return karma::string % ' '; }
  static auto Decode() -> decltype(qi::string % ' ') { return qi::string % ' '; }
};

  
template <uint tag_num, typename T, typename R = Rules<T>>
struct Field
{
  enum { tag = tag_num };
  typedef Field<tag_num, T, R> type;

  template <typename Sink>
  bool encode(Sink& sink) const {  // include tag part
    static auto encode_rule = karma::uint_ << '=' << R::Encode() << '\x01';
    return boost::spirit::karma::generate(sink, encode_rule, tag, val); 
  }

  template <typename Iterator>
  bool decode(Iterator& begin, Iterator end) {  // not include tag part
    static auto decode_rule = R::Decode() >> &qi::lit('\x01');
    return boost::spirit::qi::parse(begin, end, decode_rule, val);
  }

  Field() = default;

  template <typename U>  // U can be converted to T
  explicit Field(U const& v) : val(v) {}

  explicit Field(T&& v) : val(std::move(v)) {}

  Field(Field const& f) : val(f.val) {}
  Field(Field&& f) : val(std::move(f.val)) {}

  Field& operator=(T const& v) { val = v; return *this; }
  Field& operator=(T&& v) { val = std::move(v); return *this; }

  typename std::conditional<std::is_fundamental<T>::value, T,  T const&>::type
  value() const { return val; }

private:
  T val;
};


template <typename Field>
struct Optional : boost::optional<Field>
// Has the same interface as Field<>
{
  enum { tag = Field::tag };
  typedef Optional<Field> type;

  template <typename Sink>
  void encode(Sink& sink) const {
    if (*this) // call optional<T>::operator bool()
      (**this).encode(sink); // call optional<T>::operator*()
  }

 template <typename Iterator>
 bool decode(Iterator& begin, Iterator end) {
   Field f;
   if (!f.decode(begin, end)) return false; 
   *this = f; 
   return true;
 }
};


// FIX  field types

template <uint tag_num> using Boolean  = Field<tag_num, bool>;
template <uint tag_num> using Char     = Field<tag_num, char>;
template <uint tag_num> using Int      = Field<tag_num, int>;
template <uint tag_num> using Float    = Field<tag_num, double>;
template <uint tag_num> using String   = Field<tag_num, std::string>;
template <uint tag_num> using Data     = Field<tag_num, std::string, ByteRules>; // binary data

template <uint tag_num> using Length     = Field<tag_num, uint>;
template <uint tag_num> using TagNum     = Field<tag_num, uint>;
template <uint tag_num> using SeqNum     = Field<tag_num, uint>;  // may ulong?
template <uint tag_num> using NumInGroup = Field<tag_num, uint>;
template <uint tag_num> using DayOfMonth = Field<tag_num, uint>;

template <uint tag_num> using Qty         = Field<tag_num, double>;
template <uint tag_num> using Price       = Field<tag_num, double>;
template <uint tag_num> using PriceOffset = Field<tag_num, double>;
template <uint tag_num> using Amt         = Field<tag_num, double>;
template <uint tag_num> using Percentage  = Field<tag_num, double>;


template <uint tag_num> using MultipleChar = 
  Field<tag_num, std::string, MultipleCharValueRules>; // each char in string is a value
 
template <uint tag_num> using MultipleString = 
  Field<tag_num, std::vector<std::string>, MultipleStringValueRules>;

template <uint tag_num> using Country  = Field<tag_num, std::string>;
template <uint tag_num> using Currency = Field<tag_num, std::string>;
template <uint tag_num> using Exchange = Field<tag_num, std::string>;
template <uint tag_num> using Language = Field<tag_num, std::string>;
template <uint tag_num> using XMLData  = Field<tag_num, std::string>;

template <uint tag_num> using UTCTimestamp = Field<tag_num, std::string>;

}  // namespace FIX

#endif

