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
    MOCK_METHOD(void, handleSendSms, (const common::PhoneNumber&, const common::PhoneNumber&, const std::string&), (final));

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
    MOCK_METHOD(void, showSmsComposeMode, (), (final));
};

}
