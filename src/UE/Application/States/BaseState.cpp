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

}
