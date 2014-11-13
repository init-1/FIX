#ifndef FIX_MESSAGE_HPP_
#define FIX_MESSAGE_HPP_

#include "dict.hpp"
#include "combine_tuples.hpp"
#include <boost/container/static_vector.hpp>  // nice replacement for char[N]
#include <iterator> // for back_insert_iterator

namespace FIX {

template <typename Field, typename... Fields> // Field may be Optional, or Group it self
class Group 
// Group of fields, like components, header, groups in repeating groups.
// Message itself is a group.
// So a group can contain groups.
{
  typedef typename pl::tuple_cat_result<
    typename Field::type, typename Fields::type...>::type type; // type is a tuple
  type data_;

public:
  template <typename Sink, size_t N = 0>  // call with N=0
  void encode(Sink& sink) const
  {
    if (N == sizeof...(Fields) + 1) return;

    std::get<N>(data_).encode(sink);
    encode<Sink, N+1>(sink);
  }

  template <typename Iterator>
  bool decode(Iterator& begin, Iterator end)
  {
    int tag;
    while (begin != end) {
      auto old = begin;
      if (!boost::spirit::qi::parse(begin, end, qi::uint_ >> &qi::lit('='), tag))
        return false;
      auto ret = decode<Iterator, 0>(begin, end, tag);
      if (ret == 0) return false;
      if (ret == -1) {
        begin = old;  // leave it to others to process 
        break;        // but still will return true
      }
    }
    return true;
  }

  template <typename F> F& at() { return std::get<F>(data_); } 
  template <typename F> F const& get() const { return std::get<F>(data_); }

private:
  template <typename Iterator, size_t N>
  int decode(Iterator& begin, Iterator end, int tag)  
  // Returns 0: false, 1: true, -1: not found
  {
    if (N == sizeof...(Fields) + 1) return -1;  // not found this field in the group

    if (tag == std::tuple_element<N, type>::type::tag) 
      return std::get<N>(data_).decode(begin, end);

    return decode<Iterator, N+1>(begin, end, tag);
  }
};

template <typename F, typename G> inline F& at(G& g) { return g.at<F>(); }
template <typename F, typename G> inline F const& at(G const& g) { return g.at<F>(); }

 
template <typename Field, typename... Fields>
// Field is type of NoXXXXXX, indicating number of groups
class RepeatGroup 
{
  enum { tag = Field::tag }; // make it like a normal Field
  typedef RepeatGroup<Field, Fields...> type;
  typedef Group<Fields...> group_type;

  Field no_field_;
  std::vector<group_type> groups_;

public:
  template <typename Sink>  // call with N=0
  void encode(Sink& sink) 
  {
    if (!no_field_.val) return;

    no_field_.encode(sink);
    for (auto const& g : groups_) g.encode(sink);
  }

  template <typename Iterator>
  bool decode(Iterator& begin, Iterator end)
  {
    if (!no_field_.decode(begin, end)) return false;

    if (!no_field_.value()) return false;

    for (size_t i = 0; i < no_field_.val; ++i) {
      group_type group; 
      if (!group.decode(begin, end)) return false;
      groups_.push_back(std::move(group));
    }

    return true;
  }
};


struct VeryHeader : Group<BeginString, BodyLength>
{
   VeryHeader(size_t length = 0, char const* begstr = "FIXT.1.1") {
     at<BeginString>() = begstr;
     at<BodyLength>()  = length;
   } 
};

using Header = Group<
     MsgType
   , SenderCompId
   , TargetCompId
   , MsgSeqNum
   , Optional<PossDupFlag>
   , Optional<PossResend>
   , SendingTime
   , Optional<OrigSendingTime>
   , Optional<ApplVerId>
>;


template <typename InIterator, typename OutIterator> inline
void calc_checksum(InIterator begin, InIterator end, OutIterator& out) 
{
  uint8_t cs = std::accumulate(begin, end, static_cast<uint8_t>(0));
  *out++ = cs / 100 + '0';
  *out++ = cs / 10 % 10 + '0';
  *out   = cs % 10 + '0';
}

enum { CHECKSUM_SIZE = 4 };  // include last '\x01'

//-------------------------------------------------------------------------------------
// FIX Message:
// Can be accessed by:
//   getter:  var = at<FieldType>(message).value();
//   setter:  at<FieldType>(message) = var;  
// In setter, suppose type of var is convetible to T in Field<T>
// For example,
//   at<SecurityId>(message).value() returns a std::string const&
//   at<SecurityId) = var where var can be std::string or null-terminated char const*
// This way applys to header part of messages; e.g.,
//   at<SenderCompId>(msg) = "1";
// or
//   at<SenderCompId>(msg) = '1';  // yes, since std::string can be this way
//-------------------------------------------------------------------------------------
template <typename MsgTypeType, typename... Fields>
struct Message : Group<Header, Fields...>
{
  typedef MsgTypeType msg_type_type;
  typedef Group<Header, Fields...> base_type;

  Message() { at<MsgType>() = MsgTypeType().value(); }

  template <typename Iterator>  // For receiving something
  Message(Iterator& begin, Iterator end) { 
    if (!decode(begin, end)) throw std::runtime_error("Message Decoding Error");
  }

  template <typename Container>  // For receiving something
  Message(Container const& str) : Message(str.begin(), str.end()) {}

  std::string const& msgType() const { return at<MsgType>().value(); }

  template <typename Container>  // vector, static_vector, string, etc. 
  void encode(Container& str) 
  {
    str.clear();
    auto sink = back_insert_iterator(str);
    base_type::encode(back_insert_iterator(str));

    boost::container::static_vector<char,80> h;
    auto sink_h = std::back_insert_iterator<decltype(h)>(h);   
    at<BodyLength>(very_header_) = str.length();
    very_header_.encode(sink_h);   
    str.insert(0, h.begin(), h.end());

    char cs[CHECKSUM_SIZE];
    calc_checksum(str.begin(), str.end(), cs);
    cs[CHECKSUM_SIZE-1] = 0;
    checksum_ = cs;
    checksum_.encode(back_insert_iterator(str)); 
  } 

  template <typename Container>
  bool decode(Container const& str)
  {
    return decode(str.begin(), str.end());
  }

  template <typename Iterator>
  bool decode(Iterator& begin, Iterator end) 
  {
    auto b = begin;
    auto e = end - CHECKSUM_SIZE;
    if (very_header_.decode(begin, e) && // very header ok
        base_type::decode(begin, e) &&   // body ok
        begin == e &&                    // else some fields unrevolved
        checksum_.decode(begin, end))    // checksum resolved
    { 
      char cs[checksum_sz];
      calc_checksum(b, end, cs);
      if (std::equal(cs, cs+CHECKSUM_SIZE-1, checksum_.value().begin())) // checksum ok
        return true;
    } 
    return false;
  }

private:
  // Note body is in base_type 
  VeryHeader very_header_;
  CheckSum   checksum_;

friend class MsgQueue;
};


//-----------------------------------------------------------------------------
// GenericMessage for decoding a message

template <typename Iterator>
struct decode_visitor : boost::static_visitor<bool>
{
  decode_visitor(Iterator& begin, Iterator end) : begin_(begin), end_(end) {}

  template <typename Message>
  bool operator()(Message& msg) { return msg.decode(begin_, end_); }

  Iterator& begin_;  // Note it's a reference
  Iterator end_;
};

template <typename Message, typename... Messages>
struct GenericMessage : boost::variant<Message, Messages...>
// Can access GenericMessage as a variant; e.g., get<Message>(GM) returns a
// reference to object of type Message; or using a visitor.
{
  template <typename Container>
  bool decode(Container const& str)
  {
    return decode<Container::const_iterator>(str.begin(), str.end());
  }

  template <typename Iterator>
  bool decode(Iterator& begin, Iterator end)
  {
    VeryHeader very_header;
    MsgType msg_type;

    auto b = begin;
    if (very_header.decode(b, end) && msg_type.decode(b, end)) {
      if (msg_type_.value() == Message::msg_type_type().value()) {
        *this = Message();
        decode_visitor<Iterator> visitor(begin, end);
        return apply_visitor(visitor, *this);
      }
      if (sizeof...(Messages) == 0) return false;
      return decode<Iterator>(begin, end);
    }
  }

};

}  // namespace FIX

#endif

