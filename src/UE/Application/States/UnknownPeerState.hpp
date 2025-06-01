#pragma once

#include "BaseState.hpp"

namespace ue
{

class UnknownPeerState : public BaseState
{
public:
    UnknownPeerState(
        Context &context,
        const common::PhoneNumber &peer,
        const std::function<void(Context &unknownContext)> &exitStateCallback = nullptr);
    void handleTimeout() final;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleCallDrop() override;
    void handleDisconnect() override;

private:
    // const ITimerPort::Duration TIMEOUT_DURATION = std::chrono::seconds(2);
    const ITimerPort::Duration TIMEOUT_DURATION = std::chrono::seconds(200);
    common::PhoneNumber peer;
    std::function<void(Context &unknownContext)> exitStateCallback;
    void exitState();
    bool runExitStateCallback();
};

}
