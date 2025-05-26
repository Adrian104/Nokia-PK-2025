#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{1024};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;

    Expectation showNotConnected = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void sendAttachRequestOnSib();
};

void ApplicationNotConnectedTestSuite::sendAttachRequestOnSib()
{
    using namespace std::chrono_literals;
    EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
    EXPECT_CALL(timerPortMock, startTimer(500ms));
    EXPECT_CALL(userPortMock, showConnecting());
    objectUnderTest.handleSib(BTS_ID);
}

TEST_F(ApplicationNotConnectedTestSuite, shallSendAttatchRequestOnSib)
{
    sendAttachRequestOnSib();
}

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite()
    {
        sendAttachRequestOnSib();
    }

    void handleDisconnect()
    {
        EXPECT_CALL(userPortMock, showNotConnected());
        EXPECT_CALL(timerPortMock, stopTimer());
        objectUnderTest.handleDisconnect();
    }

    void handleAttachAccept()
    {
        EXPECT_CALL(timerPortMock, stopTimer()).Times(::testing::AnyNumber());
        EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
        objectUnderTest.handleAttachAccept();
    }
};

TEST_F(ApplicationConnectingTestSuite, shallConnectOnAttachAccept)
{
    handleAttachAccept();
}

TEST_F(ApplicationConnectingTestSuite, shallNotConnectOnAttachReject)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallNotConnectOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleDisconnectWhileConnecting)
{
    handleDisconnect();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite() : ApplicationConnectingTestSuite()
    {
        handleAttachAccept();
    }

    void handleViewSmsList()
    {
        EXPECT_CALL(userPortMock, showSmsList(_));
        objectUnderTest.handleViewSmsList();
    }

    void handleViewSms(SmsRecord& sms)
    {
        EXPECT_CALL(userPortMock, showSms(_));
        objectUnderTest.handleViewSms(sms);
    }

    void handleIncomingSMS(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string& text)
    {
        EXPECT_CALL(userPortMock, showNewMessageIndicator());
        objectUnderTest.handleIncomingSMS(msgId, from, to, text);
    }

    void handleSendSms(common::PhoneNumber from,
                       common::PhoneNumber to,
                       const std::string &text)
    {
        EXPECT_CALL(btsPortMock, sendSms(_, _, _));
        objectUnderTest.handleSendSms(from, to, text);
    }

    void handleCallRequest(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string& text)
    {
        EXPECT_CALL(userPortMock, showIncomingCall(_, _));
        EXPECT_CALL(timerPortMock, startTimer(_));
        objectUnderTest.handleCallRequest(msgId, from, to, text);
    }

    void handleCallDrop(common::PhoneNumber from, common::PhoneNumber to)
    {
        EXPECT_CALL(btsPortMock, sendCallDrop(_, _));
        objectUnderTest.handleCallDrop(from, to);
    }

    void handleCallAccept(common::PhoneNumber from, common::PhoneNumber to)
    {
        EXPECT_CALL(btsPortMock, sendCallAccept(_, _));
        EXPECT_CALL(userPortMock, showTalk(_, _));
        EXPECT_CALL(timerPortMock, startTimer(_));
        objectUnderTest.handleCallAccept(from, to);
    }
};

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnectWhileConnected)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnect();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleViewSmsList)
{
    handleViewSmsList();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleViewSms)
{
    SmsRecord sms;
    sms.m_from = common::PhoneNumber{1};
    sms.m_to = common::PhoneNumber{123};
    sms.m_message = "a";
    handleViewSms(sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceiveSms)
{
    common::MessageId id{42};
    common::PhoneNumber from{123};
    common::PhoneNumber to{222};
    std::string txt("Hello!");
    handleIncomingSMS(id, from, to, txt);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendSms)
{
    common::PhoneNumber from{31};
    common::PhoneNumber to{222};
    std::string text("test");
    handleSendSms(from, to, text);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    common::MessageId id{42};
    common::PhoneNumber from{123};
    common::PhoneNumber to{222};
    std::string txt;
    handleCallRequest(id, from, to, txt);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallDrop)
{
    common::PhoneNumber from{123};
    common::PhoneNumber to{222};
    handleCallDrop(from, to);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccept)
{
    common::PhoneNumber from{123};
    common::PhoneNumber to{222};
    handleCallAccept(from, to);
}

struct ApplicationSendingCallTestSuite : ApplicationConnectedTestSuite
{
    common::PhoneNumber peer{200};

    ApplicationSendingCallTestSuite()
    {
        EXPECT_CALL(btsPortMock, sendCallAccept(_, _))
            .Times(::testing::AnyNumber())
            .WillRepeatedly(Return(true));
        EXPECT_CALL(btsPortMock, sendCallDrop(_, _))
            .Times(::testing::AnyNumber())
            .WillRepeatedly(Return(true));
    }
    void sendCallRequest()
    {
        EXPECT_CALL(userPortMock, showDialling(PHONE_NUMBER, peer));
        EXPECT_CALL(btsPortMock, sendCallRequest(PHONE_NUMBER, peer));
        EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(60000)));
        objectUnderTest.handleSendCallRequest(PHONE_NUMBER, peer);
    }

    void handleCallAccepted()
    {
        EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(120000)))
            .Times(::testing::AnyNumber());
        EXPECT_CALL(userPortMock, showTalk(peer, PHONE_NUMBER));
        objectUnderTest.handleCallAccepted();
    }

    void handleCallDropped()
    {
        EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
        objectUnderTest.handleCallDropped();
    }

    void handleUnknownRecipient()
    {
        EXPECT_CALL(userPortMock, showUnknownRecipient(peer));
        EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
        objectUnderTest.handleUnknownRecipient();
    }
};

TEST_F(ApplicationSendingCallTestSuite, shallSendCallRequestAndShowDialling)
{
    sendCallRequest();
}

TEST_F(ApplicationSendingCallTestSuite, shallGoToTalkingStateOnCallAccepted)
{
    sendCallRequest();
    handleCallAccepted();
}

TEST_F(ApplicationSendingCallTestSuite, shallReturnToConnectedOnCallDropped)
{
    sendCallRequest();
    handleCallDropped();
}

TEST_F(ApplicationSendingCallTestSuite, shallShowUnknownRecipientOnUnknownRecipient)
{
    sendCallRequest();
    handleUnknownRecipient();
}

TEST_F(ApplicationSendingCallTestSuite, shallReturnToConnectedOnTimeout)
{
    sendCallRequest();
    EXPECT_CALL(btsPortMock, sendCallDrop(peer, PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationSendingCallTestSuite, shallShowIncomingCallInterruptingSendingCall)
{
    sendCallRequest();
    common::MessageId msgId{99};
    common::PhoneNumber from{12};
    common::PhoneNumber to{PHONE_NUMBER};
    std::string enc;

    EXPECT_CALL(timerPortMock, stopTimer()).Times(::testing::AnyNumber());
    EXPECT_CALL(btsPortMock, sendCallDrop(peer, PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(30000)));
    EXPECT_CALL(userPortMock, showIncomingCall(from, to));

    objectUnderTest.handleCallRequest(msgId, from, to, enc);
}

struct ApplicationTalkingTestSuite : ApplicationSendingCallTestSuite
{
    ApplicationTalkingTestSuite()
    {
        sendCallRequest();
        handleCallAccepted();
    }
    void handleCallDropFromBts()
    {
        EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
        objectUnderTest.handleCallDropped();
    }

    void handleCallDropFromUe()
    {
        EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER, peer));
        EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
        objectUnderTest.handleCallDrop(PHONE_NUMBER, peer);
    }

    void handleCallTalk(common::MessageId msgId,
        common::PhoneNumber from,
        common::PhoneNumber to,
        const std::string& text)
    {
        EXPECT_CALL(userPortMock, addCallMessage(from, to, text));
        EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(120000)));
        objectUnderTest.handleCallTalk(msgId, from, to, text);
    }
    
    void handleSendCallTalk(common::PhoneNumber from,
        common::PhoneNumber to,
        const std::string& text)
    {
        EXPECT_CALL(btsPortMock, sendCallTalk(from, to, text));
        objectUnderTest.handleSendCallTalk(from, to, text);
    }
    
    void handleUnknownRecipientCallTalk()
    {
        EXPECT_CALL(userPortMock, showUnknownRecipient(peer));
        objectUnderTest.handleUnknownRecipient();
    }
};

TEST_F(ApplicationTalkingTestSuite, shallReturnToConnectedOnCallDropFromBts)
{
    handleCallDropFromBts();
}

TEST_F(ApplicationTalkingTestSuite, shallReturnToConnectedOnCallDropFromUe)
{
    handleCallDropFromUe();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleCallTalk)
{
    common::MessageId msgId{1};
    common::PhoneNumber from{123};
    common::PhoneNumber to{9};
    std::string text{"test incoming calltalk"};

    handleCallTalk(msgId, from, to, text);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleSendCallTalk)
{
    common::PhoneNumber from{9};
    common::PhoneNumber to{123};
    std::string text{"test outgoing calltalk"};

    handleSendCallTalk(from, to, text);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleUnknownRecipientOnCallTalk)
{
    handleUnknownRecipientCallTalk();
}

}