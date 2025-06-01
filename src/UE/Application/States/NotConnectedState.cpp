#include "NotConnectedState.hpp"

#include "ConnectingState.hpp"

namespace ue
{

NotConnectedState::NotConnectedState(Context &context)
    : BaseState(context, "NotConnectedState")
{
    context.user.showNotConnected();
}

void NotConnectedState::handleSib(common::BtsId btsId)
{
    context.setState<ConnectingState>(btsId);
}
void NotConnectedState::handleIncomingSMS(common::MessageId msgId,
                                          const common::PhoneNumber &peer,
                                          const std::string &text)
{
    return;
    // This is a no-op, as we do not expect to receive SMS while connecting.
    // Since this method is purely virtual in BaseState we have to explicitly define
    // it here - otherwise we would get a compilation error.
}
void NotConnectedState::handleCallRequest(common::MessageId msgId,
                                          const common::PhoneNumber &peer,
                                          const std::string &enc)
{
    return;
    // This is a no-op, as we do not expect to receive call request while not connected.
    // Since this method is purely virtual in BaseState we have to explicitly define
    // it here - otherwise we would get a compilation error.
}
IUeGui::AcceptClose NotConnectedState::handleUEClose()
{
    return true;
}

void NotConnectedState::handleDisconnect()
{
    return;
    // This is a no-op, as we do not expect to disconnect from BTS while not connected.
    // Since this method is purely virtual in BaseState we have to explicitly define
    // it here - otherwise we would get a compilation error.
}
}
