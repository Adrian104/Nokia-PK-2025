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
    MOCK_METHOD(void, handleIncomingSMS, (common::MessageId, common::PhoneNumber, common::PhoneNumber, const std::string&), (final));
    MOCK_METHOD(void , handleSmsResponse, (bool), (final));
    MOCK_METHOD(void, handleCallRequest, (common::MessageId, common::PhoneNumber, common::PhoneNumber, const std::string&), (final));
    MOCK_METHOD(void, handleUnknownRecipient, (), (final));
    MOCK_METHOD(void, handleCallTalk, (common::MessageId, common::PhoneNumber, common::PhoneNumber, const std::string&), (final));
    MOCK_METHOD(void, handleCallDropped, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallAccepted, (), (final));
};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(bool, sendSms, (const common::PhoneNumber&, const common::PhoneNumber&, const std::string&), (final));
    MOCK_METHOD(bool, sendCallDrop, (common::PhoneNumber, common::PhoneNumber), (final));
    MOCK_METHOD(bool, sendCallAccept, (common::PhoneNumber, common::PhoneNumber), (final));
    MOCK_METHOD(bool, sendCallTalk, (common::PhoneNumber, common::PhoneNumber, const std::string&), (final));
    MOCK_METHOD(bool, sendCallRequest, (common::PhoneNumber, common::PhoneNumber), (final));
};

}
