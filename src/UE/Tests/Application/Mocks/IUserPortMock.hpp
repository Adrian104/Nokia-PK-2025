#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"

namespace ue
{
class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleViewSmsList, (), (final));
    MOCK_METHOD(void, handleViewSms, (SmsRecord&), (final));
    MOCK_METHOD(void, handleSendSms, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(void, handleCallDrop, (), (final));
    MOCK_METHOD(void, handleCallAccept, (), (final));
    MOCK_METHOD(void, handleSendCallTalk, (const std::string&), (final));
    MOCK_METHOD(void, handleSendCallRequest, (const common::PhoneNumber&), (final));
    MOCK_METHOD(IUeGui::AcceptClose, handleUEClose, (), (final));
};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showNewMessageIndicator, (), (final));
    MOCK_METHOD(void, showSmsList, (SmsDB&), (final));
    MOCK_METHOD(void, showSms, (SmsRecord&), (final));
    MOCK_METHOD(void, showSmsComposeMode, (const std::string&), (final));

    MOCK_METHOD(void, showIncomingCall, (const common::PhoneNumber&), (final));
    MOCK_METHOD(void, showTalk, (), (final));
    MOCK_METHOD(void, showUnknownRecipient, (const common::PhoneNumber&), (final));
    MOCK_METHOD(void, addCallMessage, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(void, showDialMode, (), (final));
    MOCK_METHOD(void, showDialling, (const common::PhoneNumber&), (final));
};

}
