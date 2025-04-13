#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");

    gui.setAcceptCallback([this, &menu]() {
        auto selected = menu.getCurrentItemIndex();
        if (!selected.first)
        {
            logger.logError("No menu item selected");
            return;
        }
        unsigned int selectedIndex = selected.second;
        
        switch (selectedIndex)
        {
        case 0: // "Compose SMS"
            break;
        case 1: // "View SMS"
            handler->handleViewSmsList();
            break;
        default:
            logger.logError("Unknown menu selection");
            break;
        }
    });
}

void UserPort::showNewMessageIndicator()
{
    gui.showNewSms(true);
}

void UserPort::showSmsList(SmsDB& smsdb)
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    for (SmsRecord& record : smsdb)
    {
        std::string itemText = "SMS From: " + std::to_string(record.m_from.value);
        menu.addSelectionListItem(itemText, "");
    }

    gui.setAcceptCallback([this, &menu, &smsdb]() {
        auto selected = menu.getCurrentItemIndex();
        if (!selected.first)
        {
            logger.logError("No SMS selected");
            return;
        }
        unsigned int selectedIndex = selected.second;
        SmsRecord& selectedSms = *(smsdb.begin() + selectedIndex);
        handler->handleViewSms(selectedSms);
    });

    gui.setRejectCallback([this]() {
        showConnected();
    });
}

void UserPort::showSms(SmsRecord& sms)
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    menu.addSelectionListItem("From: " + std::to_string(sms.m_from.value), "");
    menu.addSelectionListItem("To: " + std::to_string(sms.m_to.value), "");
    menu.addSelectionListItem("Message: " + sms.m_message, "");

    gui.setAcceptCallback(nullptr);

    gui.setRejectCallback([this]() {
        handler->handleViewSmsList();
    });
}

}
