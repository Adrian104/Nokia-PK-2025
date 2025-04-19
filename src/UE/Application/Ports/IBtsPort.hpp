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
                                   common::PhoneNumber from,
                                   common::PhoneNumber to,
                                   const std::string& text) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual bool sendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text) = 0;
};

}
