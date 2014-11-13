#ifndef OCG_MSG_DEFS_HPP_
#define OCG_MSG_DEFS_HPP_

#include "message.hpp"

namespace FIX { 

#define DEF_MSGTYPE(classname, val) \
struct classname : MsgType { classname() : MsgType(val){} };

// Some MsgTypes used in HKEx OCG
DEF_MSGTYPE(mtLogon,          "A")
DEF_MSGTYPE(mtLogout,         "5")
DEF_MSGTYPE(mtHeartbeat,      "0")
DEF_MSGTYPE(mtTestRequest,    "1")
DEF_MSGTYPE(mtResendRequest,  "2")
DEF_MSGTYPE(mtReject,         "3")
DEF_MSGTYPE(mtSeqReset,       "4")
DEF_MSGTYPE(mtNewOrder,       "D")
DEF_MSGTYPE(mtOrderCacelRequest, "F")
DEF_MSGTYPE(mtMassCancelRequest, "q")
DEF_MSGTYPE(mtOboCancelRequest,  "F")
DEF_MSGTYPE(mtOboMassCancel,     "q")
DEF_MSGTYPE(mtUserRequest,       "BE")


// Components  
using compParties = RepeatGroup<
    NoPartyIds
  , PartyId
  , PartyIdSource
  , PartyRole
>;

using compInstrument = Group<
    SecurityId
  , SecurityIdSource
  , Optional<SecurityExchange>
>;

using compDisclosureInstructionGrp = RepeatGroup<
    NoDisclosureInstructions
  , DisclosureType
  , DisclosureInstruction
>;

using Logon = 
  Message<mtLogon
    , EncryptMethod
    , HeartbeatInterval
    , NextExpectedMsgSeqNum
    , Optional<EncryptPasswordMethod>
    , Optional<EncryptPassword>
    , Optional<EncryptNewPassword>
    , Optional<SessionStatus>
    , DefaultApplVerId
    , Optional<TestMessageIndicator>
    , Optional<Text>
  >;

using Logout = 
  Message<mtLogout
    , Optional<SessionStatus>
    , Optional<Text>
  >;

using Heartbeat = 
  Message<mtHeartbeat
    , Optional<TestRequestId>
  >;

using TestRequest = 
  Message<mtTestRequest
    , TestRequestId
  >;

using ResendRequest = 
  Message<mtResendRequest
    , BeginSeqNum
    , EndSeqNum
  >;             
                       
using Reject =
  Message<mtReject
    , RefSeqNum
    , Optional<RefTagId>
    , Optional<RefMsgType>
    , Optional<SessionRejectReason>
    , Optional<Text>
  >;

using SeqReset = 
  Message<mtSeqReset
    , NewSeqNum
    , Optional<GapFillFlag>
  >;

using NewOrder = 
  Message<mtNewOrder
    , ClOrdId
    , compParties
    , compInstrument
    , Optional<ExecInst>
    , OrdType
    , Optional<Text>
    , Optional<TimeInForce>
    , Side
    , OrderQty
    , Optional<Price0>  // Price is already used as Field Type as Amt, etc. Price0 as Price Field Type
    , TransactTime
    , Optional<PositionEffect>
    , Optional<OrderCapacity>
    , Optional<OrderRestrictions>
    , Optional<MaxPriceLevels>
    , compDisclosureInstructionGrp
  >;

}  // namespace FIX::msg

#endif



