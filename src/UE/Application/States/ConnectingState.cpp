#include "ConnectingState.hpp"

#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue
{

ConnectingState::ConnectingState(Context &context,const common::BtsId& btsId ) : BaseState(context, "ConnectingState")
{
    context.user.showConnecting();
    context.bts.sendAttachRequest(btsId);
    context.timer.startTimer(ATTACH_TIMEOUT);
}

void ConnectingState::handleAttachAccept()
{
    context.setState<ConnectedState>();
}

void ConnectingState::handleAttachReject()
{
    context.setState<NotConnectedState>();
}

void ConnectingState::handleTimeout()
{
    context.setState<NotConnectedState>();
}

void ConnectingState::handleDisconnect()
{
    context.timer.stopTimer();
    context.logger.logInfo("Disconnected");
    context.setState<NotConnectedState>();
}

IUeGui::AcceptClose ConnectingState::handleUEClose()
{
    context.timer.stopTimer();
    return true;
}
void ConnectingState::handleIncomingSMS(common::MessageId msgId,
                                        const common::PhoneNumber &peer,
                                        const std::string &text)
{
    return;
    // This is a no-op, as we do not expect to receive SMS while connecting.
    // Since this method is purely virtual in BaseState we have to explicitly define
    // it here - otherwise we would get a compilation error.
}
void ConnectingState::handleCallRequest(common::MessageId msgId,
                                        const common::PhoneNumber &peer,
                                        const std::string &enc)
{
    return;
    // This is a no-op, as we do not expect to receive call request while connecting.
    // Since this method is purely virtual in BaseState we have to explicitly define
    // it here - otherwise we would get a compilation error.
}

}
