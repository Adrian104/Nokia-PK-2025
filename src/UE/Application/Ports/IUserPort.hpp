#pragma once
#include "SmsDB.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
    virtual void handleViewSmsList() = 0;
    virtual void handleViewSms(SmsRecord& sms) = 0;
    virtual void handleSendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text) = 0;
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
};

}
