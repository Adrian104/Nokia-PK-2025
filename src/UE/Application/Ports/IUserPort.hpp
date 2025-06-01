#pragma once
#include "IUeGui.hpp"
#include "SmsDB.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
    virtual void handleViewSmsList() = 0;
    virtual void handleViewSms(SmsRecord& sms) = 0;
    virtual void handleSendSms(const common::PhoneNumber& peer, const std::string& text) = 0;
    virtual void handleCallDrop() = 0;
    virtual void handleCallAccept() = 0;
    virtual void handleSendCallTalk(const std::string& message) = 0;
    virtual void handleSendCallRequest(const common::PhoneNumber& peer) = 0;
    virtual IUeGui::AcceptClose handleUEClose() = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showNewMessageIndicator() = 0; // Highlights "M" in the top left corner of the phone
    virtual void showSmsList(SmsDB& smsdb) = 0; // Shows the SMS list view screen
    virtual void showSms(SmsRecord& sms) = 0; // Shows the SMS view screen
    virtual void showSmsComposeMode() = 0;
    virtual void showIncomingCall(const common::PhoneNumber& peer) = 0;
    virtual void showTalk() = 0;
    virtual void showUnknownRecipient(const common::PhoneNumber& peer) = 0;
    virtual void addCallMessage(const common::PhoneNumber& sender, const std::string &message) = 0;
    virtual void showDialMode() = 0;
    virtual void showDialling(const common::PhoneNumber& peer) = 0;
};

}
