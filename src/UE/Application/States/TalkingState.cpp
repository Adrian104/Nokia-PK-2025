#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, common::PhoneNumber from, common::PhoneNumber to)
    : BaseState(context, "TalkingState"), m_from(from), m_to(to)
{
    context.bts.sendCallAccept(from, to);
    context.user.showTalk(from, to);
}

void TalkingState::handleUnknownRecipient()
{
    context.user.showUnknownRecipient(m_from);
}

void TalkingState::handleCallDrop(common::PhoneNumber from, common::PhoneNumber to)
{
    context.user.showConnected();
}

void TalkingState::handleCallAccept(common::PhoneNumber from, common::PhoneNumber to)
{
    context.user.showConnected();
}

}
