#include "ViewingSmsListState.hpp"

#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"
#include "ViewingSmsState.hpp"

namespace ue
{

ViewingSmsListState::ViewingSmsListState(Context &context)
    : BaseState(context, "ViewingSmsListState")
{
    context.user.showSmsList(context.smsdb);
}

void ViewingSmsListState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ViewingSmsListState::handleIncomingSMS(common::MessageId msgId,
                                            const common::PhoneNumber &peer,
                                            const std::string &text)
{
    logger.logInfo("Received message from ", std::to_string(peer.value), ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();

    context.smsdb.addReceivedSms(peer, number_of_this_UE, text);
    context.user.showNewMessageIndicator();
    context.user.showSmsList(context.smsdb);
}

void ViewingSmsListState::handleCallRequest(common::MessageId msgId,
                                            const common::PhoneNumber &peer,
                                            const std::string &enc)
{
    context.setState<ReceivingCallState>(peer,
                                         [this](Context &unknownContext)
                                         { unknownContext.setState<ViewingSmsListState>(); });
}

IUeGui::AcceptClose ViewingSmsListState::handleUEClose()
{
    return true;
}

void ViewingSmsListState::handleCallDrop()
{
    context.setState<ConnectedState>();
}

void ViewingSmsListState::handleViewSms(SmsRecord &sms)
{
    context.setState<ViewingSmsState>(sms);
}

}