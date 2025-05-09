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
            handler->handleSmsResponse(0);
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
        return true; // SMS sent successfully to BTS
    }
    catch (const std::exception& e)
    {
        logger.logError("Failed to send SMS: ", e.what());
        return false;
    }
}

}
