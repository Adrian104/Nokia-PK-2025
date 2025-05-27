#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    transport.registerDisconnectedCallback([this]() {handleDisconnect();});
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            const std::string& text = reader.readRemainingText();
            handler->handleIncomingSMS(msgId, from, to, text);
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            handler->handleUnknownRecipient();
            break;
        }
        case common::MessageId::CallRequest:
        {
            const std::string& encryptionData = reader.readRemainingText();
            handler->handleCallRequest(msgId, from, to, encryptionData);
            break;
        }
        case common::MessageId::CallTalk:
        {
            const std::string& text = reader.readRemainingText();
            handler->handleCallTalk(msgId, from, to, text);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            handler->handleCallAccepted();
            break;
        }
        case common::MessageId::CallDropped:
        {
            handler->handleCallDropped(from);
            break;
        }
        default:
            logger.logError("unknow message: ", msgId, ", from: ", from);

        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}


void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());


}

void BtsPort::handleDisconnect()
{
    logger.logInfo("handleDisconnect");
    if (handler)
        handler->handleDisconnect();
    else
        logger.logError("handleDisconnect: handler is nullptr");
}

bool BtsPort::sendSms(const common::PhoneNumber& from, const common::PhoneNumber& to, const std::string& text)
{
    logger.logDebug("sendSms: from ", from.value, " to ", to.value, " text: ", text);

    try
    {
        common::OutgoingMessage msg{common::MessageId::Sms, from, to};
        msg.writeText(text);
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception& e)
    {
        logger.logError("Failed to send SMS: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallDrop(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logDebug("sendCallDrop: from ", from, " to ", to);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallDropped, to, from};
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception& e)
    {
        logger.logError("Failed to send call drop: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallAccept(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logDebug("sendCallAccept: from ", from, " to ", to);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallAccepted, to, from};
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception& e)
    {
        logger.logError("Failed to send call accept: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallTalk(common::PhoneNumber from, common::PhoneNumber to, const std::string &message)
{
    logger.logDebug("sendCallTalk: from ", from, " to ", to);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallTalk, from, to};
        msg.writeText(message);
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception& e)
    {
        logger.logError("Failed to send call talk: ", e.what());
        return false;
    }

}

bool BtsPort::sendCallRequest(common::PhoneNumber from, common::PhoneNumber to)
{
    logger.logDebug("sendCallRequest: from ", from, " to ", to);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallRequest, from, to};
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception &e)
    {
        logger.logError("Failed to send call request: ", e.what());
        return false;
    }
}

}
