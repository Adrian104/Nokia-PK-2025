#include "UserPort.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/ITextMode.hpp"

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"), gui(gui), phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
    gui.setCloseGuard([&handler]() { return handler.handleUEClose(); });
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
    gui.showNotConnected();

}

void UserPort::showConnecting()
{
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
    gui.showConnecting();
}

void UserPort::showConnected()
{
    IUeGui::IListViewMode &menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Dial", "");
    gui.setAcceptCallback(
        [this, &menu]()
        {
            auto selected = menu.getCurrentItemIndex();
            if (!selected.first)
            {
                logger.logError("No menu item selected");
                return;
            }

            switch (static_cast<unsigned int>(selected.second))
            {
            case 0: // "Compose SMS"
                showSmsComposeMode();
                break;
            case 1: // "View SMS"
                handler->handleViewSmsList();
                break;
            case 2: // "Dial"
                showDialMode();
                break;
            default:
                logger.logError("Unknown menu selection");
                break;
            }
        });
    gui.setRejectCallback(nullptr);
}

void UserPort::showNewMessageIndicator()
{
    gui.showNewSms(true);
}

void UserPort::showSmsList(SmsDB &smsdb)
{
    IUeGui::IListViewMode &menu = gui.setListViewMode();
    menu.clearSelectionList();

    for (SmsRecord &record : smsdb)
    {
        menu.addSelectionListItem(record.getTitle(), "");
    }

    gui.setAcceptCallback(
        [this, &menu, &smsdb]()
        {
            auto selected = menu.getCurrentItemIndex();
            if (!selected.first)
            {
                logger.logError("No SMS selected");
                return;
            }
            unsigned int selectedIndex = selected.second;
            SmsRecord &selectedSms = *(smsdb.begin() + selectedIndex);
            selectedSms.m_isNew = false;
            gui.showNewSms(smsdb.hasUnreadMessages());
            handler->handleViewSms(selectedSms);
        });

    gui.setRejectCallback([this]() { showConnected(); });
}

void UserPort::showSms(SmsRecord &sms)
{
    IUeGui::IListViewMode &menu = gui.setListViewMode();
    menu.clearSelectionList();

    if (sms.m_status == SmsStatus::FAILED)
    {
        menu.addSelectionListItem("Message failed\n to be delivered", "");
    }
    menu.addSelectionListItem("From: " + std::to_string(sms.m_from.value), "");
    menu.addSelectionListItem("To: " + std::to_string(sms.m_to.value), "");
    menu.addSelectionListItem("Message: " + sms.m_message, "");

    gui.setAcceptCallback(nullptr);

    gui.setRejectCallback([this]() { handler->handleViewSmsList(); });
}

void UserPort::showSmsComposeMode()
{
    IUeGui::ISmsComposeMode &composeMode = gui.setSmsComposeMode();
    composeMode.clearSmsText();

    gui.setAcceptCallback(
        [this, &composeMode]()
        {
            auto peer = composeMode.getPhoneNumber();
            auto text = composeMode.getSmsText();

            if (peer.isValid() && !text.empty())
            {
                handler->handleSendSms(peer, text);
            }
            else
            {
                logger.logError("Invalid recipient or empty message");
            }
        });

    gui.setRejectCallback([this]() { showConnected(); });
}

void UserPort::showIncomingCall(const common::PhoneNumber &peer)
{
    const std::string INFO = "Incoming call\nfrom: " + std::to_string(peer.value);
    gui.setAlertMode().setText(INFO);

    gui.setAcceptCallback([this]() -> void { handler->handleCallAccept(); });

    gui.setRejectCallback([this]() -> void { handler->handleCallDrop(); });
}

void UserPort::showTalk()
{
    IUeGui::ICallMode &callMode = gui.setCallMode();
    callMode.clearIncomingText();

    gui.setAcceptCallback(
        [this, &callMode]()
        {
            std::string outgoingMessage = callMode.getOutgoingText();
            if (!outgoingMessage.empty())
            {
                callMode.appendIncomingText(std::to_string(phoneNumber.value) + ": " +
                                            outgoingMessage);
                handler->handleSendCallTalk(outgoingMessage);
                callMode.clearOutgoingText();
            }
        });
    gui.setRejectCallback([this]() -> void { handler->handleCallDrop(); });
}

void UserPort::addCallMessage(const common::PhoneNumber &sender, const std::string &message)
{
    IUeGui::ICallMode &callMode = gui.setCallMode();
    callMode.appendIncomingText(std::to_string(sender.value) + ": " + message);
}

void UserPort::showUnknownRecipient(const common::PhoneNumber &peer)
{
    gui.showPeerUserNotAvailable(peer);
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback([this]() -> void { handler->handleCallDrop(); });
}

void UserPort::showDialMode()
{
    IUeGui::IDialMode &dialMode = gui.setDialMode();

    gui.setAcceptCallback(
        [this, &dialMode]()
        {
            const auto TO = dialMode.getPhoneNumber();
            if (TO.isValid())
            {
                handler->handleSendCallRequest(TO);
            }
            else
            {
                logger.logError("Invalid phone number for dialing");
            }
        });

    gui.setRejectCallback([this]() { showConnected(); });
}

void UserPort::showDialling(const common::PhoneNumber &peer)
{
    const std::string INFO = "Dialling...\nto: " + std::to_string(peer.value);
    gui.setAlertMode().setText(INFO);

    gui.setRejectCallback([this]() -> void { handler->handleCallDrop(); });

    gui.setAcceptCallback([this]() {});
}
}
