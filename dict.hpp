#ifndef FIX_DICT_HPP_
#define FIX_DICT_HPP_

#include "field.hpp"

namespace FIX {

// All Fields used in HKEx OCG

using BeginSeqNum       = SeqNum<7>;
using BeginString       = String<8>;
using BodyLength        = Length<9>;
using CheckSum          = String<10>;
using EndSeqNum         = SeqNum<16>;
using MsgSeqNum         = SeqNum<34>;
using MsgType           = String<35>;
using NewSeqNum         = SeqNum<36>;
using PossDupFlag       = Boolean<43>;
using RefSeqNum         = SeqNum<45>;
using SenderCompId      = String<49>;
using SendingTime       = UTCTimestamp<52>;
using TargetCompId      = String<56>;
using PossResend        = Boolean<97>;
using EncryptMethod     = Int<98>;
using HeartbeatInterval = Int<108>;
using TestRequestId     = String<112>;
using OrigSendingTime   = UTCTimestamp<122>;
using GapFillFlag       = Boolean<123>;
using RefTagId          = Int<371>;
using RefMsgType        = String<372>;
using SessionRejectReason   = Int<373>;
using BusinessRejectReason  = Int<380>;
using TestMessageIndicator  = Boolean<464>;
using Password              = String<554>;
using NextExpectedMsgSeqNum = SeqNum<789>;
using NewPassword           = String<925>;
using ApplVerId             = String<1128>;
using DefaultApplVerId      = String<1137>;
using EncryptPasswordMethod = Data<1400>;
using EncryptPassword       = Data<1402>; 
using EncryptNewPassword    = Data<1404>; 
using SessionStatus         = Int<1409>;

// Busiess Level

using ClOrdId            = String<11>;
using CumQty             = Qty<14>;
using ExecId             = String<17>;
using ExecInst           = MultipleChar<18>;
using ExecRefId          = String<19>;
using SecurityIdSource   = String<22>;
using LastPx             = Price<31>;
using LastQty            = Qty<32>;
using OrderId            = String<37>;
using OrderQty           = Qty<38>;
using OrdStatus          = Char<39>;
using OrdType            = Char<40>;
using OrigClOrdId        = String<41>;
using Price0             = Price<44>;
using SecurityId         = String<48>;
using Side               = Char<54>;
using Text               = String<58>;
using TimeInForce        = Char<59>;
using TransactTime       = UTCTimestamp<60>;
using PositionEffect     = Char<77>;
using CxlRejReason       = Int<102>;
using OrdRejReason       = Int<103>;
using BixPx              = Price<132>;
using OfferPx            = Price<133>;
using BidSize            = Qty<134>;
using OfferSize          = Qty<135>;
using ExecType           = Char<150>;
using LeavesQty          = Qty<151>;
using SecurityExchange   = Exchange<207>;
using NoQuoteEntries     = NumInGroup<295>;
using QuoteStatus        = Int<297>;
using QuoteCancelType    = Int<298>;
using QuoteRejectReason  = Int<300>;
using ExecRestatementReason  = Int<378>;
using BusinessRejectRefId    = String<379>;
using BidId                  = String<390>;
using CxlRejResponseTo       = Char<434>;
using PartyIdSource          = Char<447>;
using PartyId                = String<448>;
using PartyRole              = Int<452>;
using NoPartyIds             = NumInGroup<453>;
using TradeReportTransType   = Int<487>;
using OrderCapacity          = Char<528>;
using OrderRestrictions      = MultipleChar<529>;
using MassCancelRequestType  = Char<530>;
using MassCancelResponseType = Char<531>;
using MassCancelRejectReason = Int<532>;
using QuoteType              = Int<537>;
using NoSides                = NumInGroup<552>;
using UserName               = String<553>;
using TradeReportId          = String<571>;
using MatchType              = String<574>;
using NoClearingInstructions = NumInGroup<576>;
using ClearingInstruction    = Int<577>;
using TradeReportRejectReason = Int<751>;
using TrdType                = Int<828>;
using TradeReportTyp         = Int<856>;
using TrdMatchId             = String<880>;
using LastFragment           = Boolean<893>;
using UserRequestId          = String<923>;
using UserRequestType        = Int<924>;
using TrdRptStatus           = Int<939>;
using TradeId                = String<1003>;
using MaxPriceLevels         = Int<1090>;
using LotType                = Char<1093>;
using OrderCategory          = Char<1115>;
using TradeHandlingInsr      = Char<1123>;
using QuoteMsgId             = String<1166>;
using MarketSegmentId        = String<1300>;
using RejectText             = String<1328>;
using MassActionReportId     = String<1369>;
using RequestResult          = Int<1511>;
using TotNoPartyList         = Int<1512>;
using InstrumentScopeOperator   = Int<1535>;
using InstrumentScopeSecurityId = Int<1538>;
using InstrumentScopeSecurityIdSource = Int<1539>;

using NoThrottles          = NumInGroup<1610>;
using ThrottleAction       = Int<1611>;
using ThrottleType         = Int<1612>;
using ThrottleNoMsgs       = Int<1613>;
using ThrottleTimeInterval = Int<1614>;
using ThrottleTimeUnit     = Int<1615>;

using InstrumentScopeSecurityExchange = Exchange<1616>;
using NoInstrumentScopes   = NumInGroup<1656>;
using NoPartyDetails       = NumInGroup<1671>;
using PartyDetailId        = String<1691>;
using PartyDetailIdSource  = Char<1692>;
using PartyDetailRole      = Int<1693>;

using EntitlementsRequestId = String<1770>;
using EntitlementReportId   = String<1771>;
using NoPartyEntitlements   = NumInGroup<1772>;
using NoEntitlements        = NumInGroup<1773>;
using EntitlementIndicator  = Boolean<1774>;
using EntitlementType       = Int<1775>;
using EntitlementId         = String<1776>;
using NoEntitlementAttrib   = Int<1777>;
using EntitlementAttribType = Int<1778>;
using EntitlementAttribDataType = Int<1779>;
using EntitlementAttribValue = String<1780>;

using NoDisclosureInstructions = NumInGroup<1812>;
using DisclosureType        = Int<1813>;
using DisclosureInstruction = Int<1814>;
using OfferId               = String<1867>;
using NoValueChecks         = NumInGroup<1868>;
using ValueCheckType        = Int<1869>;
using ValueCheckAction      = Int<1870>;
using ExchangeTradeType     = Char<5681>;

// Below not used in HKEx OCG
using ResetSeqnumFlag = Boolean<141>;

using OnBehalfOfCompId      = String<115>;
using OnBehalfOfSubId       = String<116>;
using OnBehalfOfLocationId  = String<144>;
using OnBehalfOfSendingTime = UTCTimestamp<370>;

}  // namespace FIX

#endif

