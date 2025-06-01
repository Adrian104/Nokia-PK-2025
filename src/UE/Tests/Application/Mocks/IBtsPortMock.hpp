#pragma once

#include <gmock/gmock.h>
#include "Ports/IBtsPort.hpp"

namespace ue
{

class IBtsEventsHandlerMock : public IBtsEventsHandler
{
public:
    IBtsEventsHandlerMock();
    ~IBtsEventsHandlerMock() override;

    MOCK_METHOD(void, handleSib, (common::BtsId), (final));
    MOCK_METHOD(void, handleAttachAccept, (), (final));
    MOCK_METHOD(void, handleAttachReject, (), (final));
    MOCK_METHOD(void, handleDisconnect, (), (final));
    MOCK_METHOD(void, handleIncomingSMS, (common::MessageId, const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(void, handleCallRequest, (common::MessageId, const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(void, handleSmsResponse, (bool), (final));
    MOCK_METHOD(void, handleUnknownRecipient, (), (final));
    MOCK_METHOD(void, handleCallTalk, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(void, handleCallDropped, (), (final));
    MOCK_METHOD(void, handleCallAccepted, (), (final));
};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(bool, sendSms, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(bool, sendCallDrop, (const common::PhoneNumber&), (final));
    MOCK_METHOD(bool, sendCallAccept, (const common::PhoneNumber&), (final));
    MOCK_METHOD(bool, sendCallTalk, (const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(bool, sendCallRequest, (const common::PhoneNumber&), (final));
    MOCK_METHOD(common::PhoneNumber, getMyPhoneNumber, (), (const, final));
};

}
