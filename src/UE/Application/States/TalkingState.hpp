#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber from, common::PhoneNumber to);
    void handleUnknownRecipient() override;
    void handleCallDrop(common::PhoneNumber from, common::PhoneNumber to) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleCallTalk(common::MessageId msgId, common::PhoneNumber from, common::PhoneNumber to, const std::string &message) override;
    void handleSendCallTalk(common::PhoneNumber from, common::PhoneNumber to, const std::string & message) override;
    void handleTimeout() override;
    void handleCallRequest(common::MessageId msgId,
                           common::PhoneNumber from,
                           common::PhoneNumber to,
                           const std::string &enc) override;

private:
    common::PhoneNumber m_from;
    common::PhoneNumber m_to;
};

}
