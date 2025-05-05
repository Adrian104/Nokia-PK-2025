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
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showConnected());
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
}
