#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger,
                 common::ITransport &transport,
                 common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"), transport(transport), phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](common::BinaryMessage msg)
                                      { handleMessage(msg); });
    transport.registerDisconnectedCallback([this]() { handleDisconnect(); });
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(common::BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();
        // Read 'to' phone number, but we don't use it here
        // It is important so that subsequent call to readRemainingText() works correctly,
        // and doesn't try to read the 'to' phone number as a text.
        // Reader is stateful, so it keeps track of the current position in the message.


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
            const std::string &text = reader.readRemainingText();
            handler->handleIncomingSMS(msgId, from, text);
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            handler->handleUnknownRecipient();
            break;
        }
        case common::MessageId::CallRequest:
        {
            const std::string &encryptionData = reader.readRemainingText();
            handler->handleCallRequest(msgId, from, encryptionData);
            break;
        }
        case common::MessageId::CallTalk:
        {

            const std::string &text = reader.readRemainingText();
            handler->handleCallTalk(from, text);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            handler->handleCallAccepted();
            break;
        }
        case common::MessageId::CallDropped:
        {
            handler->handleCallDropped();
            break;
        }
        default:
            logger.logError("unknow message: ", msgId, ", from: ", from);
        }
    }
    catch (std::exception const &ex)
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

bool BtsPort::sendSms(const common::PhoneNumber &peer, const std::string &text)
{
    logger.logDebug("sendSms: from ", phoneNumber.value, " to ", peer.value, " text: ", text);

    try
    {
        common::OutgoingMessage msg{common::MessageId::Sms, phoneNumber, peer};
        msg.writeText(text);
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception &e)
    {
        logger.logError("Failed to send SMS: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallDrop(const common::PhoneNumber &peer)
{
    logger.logDebug("sendCallDrop: from ", phoneNumber, " to ", peer);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallDropped, phoneNumber, peer};
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception &e)
    {
        logger.logError("Failed to send call drop: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallAccept(const common::PhoneNumber &peer)
{
    logger.logDebug("sendCallAccept: from ", phoneNumber, " to ", peer);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallAccepted, phoneNumber, peer};
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception &e)
    {
        logger.logError("Failed to send call accept: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallTalk(const common::PhoneNumber &peer, const std::string &message)
{
    logger.logDebug("sendCallTalk: from ", phoneNumber, " to ", peer);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallTalk, phoneNumber, peer};
        msg.writeText(message);
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception &e)
    {
        logger.logError("Failed to send call talk: ", e.what());
        return false;
    }
}

bool BtsPort::sendCallRequest(const common::PhoneNumber &peer)
{
    logger.logDebug("sendCallRequest: from ", phoneNumber, " to ", peer);

    try
    {
        common::OutgoingMessage msg{common::MessageId::CallRequest, phoneNumber, peer};
        transport.sendMessage(msg.getMessage());
        return true;
    }
    catch (const std::exception &e)
    {
        logger.logError("Failed to send call request: ", e.what());
        return false;
    }
}
common::PhoneNumber BtsPort::getMyPhoneNumber() const
{
    return phoneNumber;
}

}
