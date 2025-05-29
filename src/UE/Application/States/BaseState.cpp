#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleDisconnect()
{
    logger.logError("Uexpected: handleDisconnect");
}

void BaseState::handleIncomingSMS(common::MessageId msgId,
                                  common::PhoneNumber from,
                                  common::PhoneNumber to,
                                  const std::string& text)
{
    logger.logError("Uexpected: handleIncomingSMS");
}

void BaseState::handleCallRequest(common::MessageId msgId,
                       common::PhoneNumber from,
                       common::PhoneNumber to,
                       const std::string& enc)
{
    logger.logError("Uexpected: handleCallRequest");
}

void BaseState::handleViewSmsList()
{
    logger.logError("Uexpected: handleViewSms");
}

void BaseState::handleViewSms(SmsRecord& sms)
{
    logger.logError("Uexpected: handleViewSms");
}

void BaseState::handleSendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text)
{
    logger.logError("Uexpected: handleSendSms");
}

void BaseState::handleSmsResponse(bool status)
{
    logger.logError("Uexpected: handleSmsResponse");

}

void BaseState::handleCallDrop(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logError("Uexpected: handleCallDrop");
}

void BaseState::handleCallAccept(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logError("Uexpected: handleCallAccept");
}

void BaseState::handleUnknownRecipient()
{
    logger.logError("Uexpected: handleUnknownRecipient");
}

void BaseState::handleCallTalk(common::MessageId msgId, common::PhoneNumber from, common::PhoneNumber to, const std::string& message)
{
    logger.logError("Uexpected: handleCallTalk");
}

void BaseState::handleSendCallTalk(common::PhoneNumber from, common::PhoneNumber to, const std::string& message)
{
    logger.logError("Uexpected: handleSendCallTalk");
}

void BaseState::handleSendCallRequest(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logError("Uexpected: handleSendCallRequest");
}

void BaseState::handleCallDropped(common::PhoneNumber from)
{
    logger.logError("Uexpected: handleCallDropped");
}

void BaseState::handleCallAccepted()
{
    logger.logError("Uexpected: handleCallAccepted");
}

IUeGui::AcceptClose BaseState::handleUEClose()
{
    logger.logError("Uexpected: handleUEClose");
    return false;
    // By default, prevent closing the application, so it must be handled explicitly,
    // so the cleanup will be done properly by the current state
}
}
