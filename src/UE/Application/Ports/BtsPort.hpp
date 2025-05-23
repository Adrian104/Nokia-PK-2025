#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "ITransport.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BtsPort : public IBtsPort
{
public:
    BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber);
    void start(IBtsEventsHandler& handler);
    void stop();

    void sendAttachRequest(common::BtsId) override;
    bool sendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text) override;
    bool sendCallDrop(common::PhoneNumber from, common::PhoneNumber to) override;
    bool sendCallAccept(common::PhoneNumber from, common::PhoneNumber to) override;
    bool sendCallTalk(common::PhoneNumber from, common::PhoneNumber to, const std::string &message) override;
    bool sendCallRequest(common::PhoneNumber from, common::PhoneNumber to) override;

private:
    void handleMessage(BinaryMessage msg);
    void handleDisconnect();

    common::PrefixedLogger logger;
    common::ITransport& transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler* handler = nullptr;
};

}
