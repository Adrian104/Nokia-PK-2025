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
                           const common::PhoneNumber &peer,
                           const std::string &text)
    {
        EXPECT_CALL(userPortMock, showNewMessageIndicator());
        objectUnderTest.handleIncomingSMS(msgId, peer, text);
    }

    void handleSendSms(const common::PhoneNumber &peer, const std::string &text)
    {
        EXPECT_CALL(btsPortMock, sendSms(_, _));
        objectUnderTest.handleSendSms(peer, text);
    }

    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string& text)
    {
        EXPECT_CALL(userPortMock, showIncomingCall(_));
        EXPECT_CALL(timerPortMock, startTimer(_));
        objectUnderTest.handleCallRequest(msgId, peer, text);
    }

    void handleCallDrop()
    {
        EXPECT_CALL(btsPortMock, sendCallDrop(_));
        objectUnderTest.handleCallDrop();
    }

    void handleCallAccept()
    {
        EXPECT_CALL(btsPortMock, sendCallAccept(_));
        EXPECT_CALL(userPortMock, showTalk());
        EXPECT_CALL(timerPortMock, startTimer(_));
        objectUnderTest.handleCallAccept();
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
    common::PhoneNumber peer{123};
    std::string txt("Hello!");
    handleIncomingSMS(id, peer, txt);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendSms)
{
    common::PhoneNumber peer{123};
    std::string text("test");
    handleSendSms(peer, text);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    common::MessageId id{42};
    common::PhoneNumber peer{123};
    std::string txt;
    handleCallRequest(id, peer, txt);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallDrop)
{
    handleCallDrop();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccept)
{
    handleCallAccept();
}

struct ApplicationSendingCallTestSuite : ApplicationConnectedTestSuite
{
    common::PhoneNumber peer{200};

    ApplicationSendingCallTestSuite()
    {
        EXPECT_CALL(btsPortMock, sendCallAccept(_))
            .Times(::testing::AnyNumber())
            .WillRepeatedly(Return(true));
        EXPECT_CALL(btsPortMock, sendCallDrop(_))
            .Times(::testing::AnyNumber())
            .WillRepeatedly(Return(true));
    }
    void sendCallRequest()
    {
        EXPECT_CALL(userPortMock, showDialling(peer));
        EXPECT_CALL(btsPortMock, sendCallRequest(peer));
        EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(60000)));
        objectUnderTest.handleSendCallRequest(peer);
    }

    void handleCallAccepted()
    {
        EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(120000)))
            .Times(::testing::AnyNumber());
        EXPECT_CALL(userPortMock, showTalk());
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
    EXPECT_CALL(btsPortMock, sendCallDrop(peer));
    EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationSendingCallTestSuite, shallShowIncomingCallInterruptingSendingCall)
{
    sendCallRequest();
    common::MessageId msgId{99};
    std::string enc;

    EXPECT_CALL(timerPortMock, stopTimer()).Times(::testing::AnyNumber());
    EXPECT_CALL(btsPortMock, sendCallDrop(peer));
    EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(30000)));
    EXPECT_CALL(userPortMock, showIncomingCall(peer));

    objectUnderTest.handleCallRequest(msgId, peer, enc);
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
        EXPECT_CALL(btsPortMock, sendCallDrop(peer));
        EXPECT_CALL(userPortMock, showConnected()).Times(::testing::AnyNumber());
        objectUnderTest.handleCallDrop();
    }

    void handleCallTalk(common::MessageId msgId,
        const common::PhoneNumber &peer,
        const std::string& text)
    {
        EXPECT_CALL(userPortMock, addCallMessage(peer, text));
        EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(120000)));
        objectUnderTest.handleCallTalk(peer, text);
    }
    
    void handleSendCallTalk(const common::PhoneNumber &peer,
        const std::string& text)
    {
        EXPECT_CALL(btsPortMock, sendCallTalk(peer, text));
        objectUnderTest.handleSendCallTalk(text);
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
    common::PhoneNumber peer{123};
    std::string text{"test incoming calltalk"};

    handleCallTalk(msgId, peer, text);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleSendCallTalk)
{
    common::PhoneNumber peer{9};
    std::string text{"test outgoing calltalk"};

    handleSendCallTalk(peer, text);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleUnknownRecipientOnCallTalk)
{
    handleUnknownRecipientCallTalk();
}

}
