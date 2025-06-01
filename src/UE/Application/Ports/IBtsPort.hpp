#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/IncomingMessage.hpp"

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleDisconnect() = 0;
    virtual void handleIncomingSMS(common::MessageId msgId,
                                   const common::PhoneNumber &peer,
                                   const std::string &text) = 0;
    virtual void handleCallRequest(common::MessageId msgId,
                                   const common::PhoneNumber &peer,
                                   const std::string &enc) = 0;
    virtual void handleSmsResponse(bool status) = 0;
    virtual void handleUnknownRecipient() = 0;
    virtual void handleCallTalk(const common::PhoneNumber& sender, const std::string &message) = 0;
    virtual void handleCallDropped() = 0;
    virtual void handleCallAccepted() = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual bool sendSms(const common::PhoneNumber &peer, const std::string &text) = 0;
    virtual bool sendCallDrop(const common::PhoneNumber &peer) = 0;
    virtual bool sendCallAccept(const common::PhoneNumber &peer) = 0;
    virtual bool sendCallTalk(const common::PhoneNumber &peer, const std::string &message) = 0;
    virtual bool sendCallRequest(const common::PhoneNumber &peer) = 0;

    virtual common::PhoneNumber getMyPhoneNumber() const = 0;
};

}
