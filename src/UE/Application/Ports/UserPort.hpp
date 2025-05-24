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

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showNewMessageIndicator() override; // Highlights "M" in the top left corner of the phone
    void showSmsList(SmsDB& smsdb) override; // Shows the SMS list view screen
    void showSms(SmsRecord& sms) override; // Shows the SMS view screen
    void showSmsComposeMode() override; // Shows the SMS compose mode screen
    void showIncomingCall(common::PhoneNumber from, common::PhoneNumber to) override;
    void showTalk(common::PhoneNumber from, common::PhoneNumber to) override;
    void addCallMessage(common::PhoneNumber from, common::PhoneNumber to, const std::string &message) override;
    void showUnknownRecipient(common::PhoneNumber number) override;
    void showDialMode() override;
    void showDialling(common::PhoneNumber from, common::PhoneNumber to) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}
