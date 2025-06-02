#include "ViewingSmsState.hpp"

#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"
#include "ViewingSmsListState.hpp"

namespace ue
{

ViewingSmsState::ViewingSmsState(Context &context, const SmsRecord &sms)
    : BaseState(context, "ViewingSmsState"), sms(sms)
{
    context.user.showSms(sms);
}

void ViewingSmsState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ViewingSmsState::handleIncomingSMS(common::MessageId msgId,
                                        const common::PhoneNumber &peer,
                                        const std::string &text)
{
    logger.logInfo("Received message from ", std::to_string(peer.value), ", content: ", text);
    const auto number_of_this_UE = context.bts.getMyPhoneNumber();

    context.smsdb.addReceivedSms(peer, number_of_this_UE, text);
    context.user.showNewMessageIndicator();
}

void ViewingSmsState::handleCallRequest(common::MessageId msgId,
                                        const common::PhoneNumber &peer,
                                        const std::string &enc)
{
    const auto sms_copy = sms;
    context.setState<ReceivingCallState>(peer,
                                         [sms_copy](Context &unknownContext)
                                         { unknownContext.setState<ViewingSmsState>(sms_copy); });
}

IUeGui::AcceptClose ViewingSmsState::handleUEClose()
{
    return true;
}

void ViewingSmsState::handleCallDrop()
{
    context.setState<ViewingSmsListState>();
}

}