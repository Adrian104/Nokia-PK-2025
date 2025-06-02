#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();
    // To ensure no old (bad) callback is called every screen must
    // set both accept and reject callbacks, even if they are empty,
    // otherwise the previous screen's callback will be called which can lead to
    // unexpected behavior.
    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showNewMessageIndicator() override; // Highlights "M" in the top left corner of the phone
    void showSmsList(SmsDB& smsdb) override; // Shows the SMS list view screen
    void showSms(SmsRecord& sms) override; // Shows the SMS view screen
    void showSmsComposeMode(const std::string &text = "") override; // Shows the SMS compose mode screen
    void showIncomingCall(const common::PhoneNumber& peer) override;
    void showTalk() override;
    void addCallMessage(const common::PhoneNumber& sender, const std::string &message) override;
    void showUnknownRecipient(const common::PhoneNumber& peer) override;
    void showDialMode() override;
    void showDialling(const common::PhoneNumber& peer) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}
