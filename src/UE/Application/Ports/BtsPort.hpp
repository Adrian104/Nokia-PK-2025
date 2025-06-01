#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "CommonEnvironment/ITransport.hpp"
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
    bool sendSms(const common::PhoneNumber& peer, const std::string& text) override;
    bool sendCallDrop(const common::PhoneNumber& peer) override;
    bool sendCallAccept(const common::PhoneNumber& peer) override;
    bool sendCallTalk(const common::PhoneNumber& peer, const std::string &message) override;
    bool sendCallRequest(const common::PhoneNumber& peer) override;

    common::PhoneNumber getMyPhoneNumber() const override;


private:
    void handleMessage(common::BinaryMessage msg);
    void handleDisconnect();

    common::PrefixedLogger logger;
    common::ITransport& transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler* handler = nullptr;
};

}
