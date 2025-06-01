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
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Unexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Unexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Unexpected: handleAttachReject");
}

void BaseState::handleDisconnect()
{
    logger.logError("Unexpected: handleDisconnect");
}

void BaseState::handleViewSmsList()
{
    logger.logError("Unexpected: handleViewSms");
}

void BaseState::handleViewSms(SmsRecord& sms)
{
    logger.logError("Unexpected: handleViewSms");
}

void BaseState::handleSendSms(const common::PhoneNumber& peer, const std::string& text)
{
    logger.logError("Unexpected: handleSendSms");
}

void BaseState::handleSmsResponse(bool status)
{
    logger.logError("Unexpected: handleSmsResponse");

}

void BaseState::handleCallDrop()
{
    logger.logError("Unexpected: handleCallDrop");
}

void BaseState::handleCallAccept()
{
    logger.logError("Unexpected: handleCallAccept");
}

void BaseState::handleUnknownRecipient()
{
    logger.logError("Unexpected: handleUnknownRecipient");
}

void BaseState::handleCallTalk(const common::PhoneNumber& sender, const std::string &message)
{
    logger.logError("Unexpected: handleCallTalk");
}

void BaseState::handleSendCallTalk(const std::string& message)
{
    logger.logError("Unexpected: handleSendCallTalk");
}

void BaseState::handleSendCallRequest(const common::PhoneNumber& peer)
{
    logger.logError("Unexpected: handleSendCallRequest");
}

void BaseState::handleCallDropped()
{
    logger.logError("Unexpected: handleCallDropped");
}

void BaseState::handleCallAccepted()
{
    logger.logError("Unexpected: handleCallAccepted");
}

}
