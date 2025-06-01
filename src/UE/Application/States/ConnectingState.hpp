#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectingState : public BaseState
{
public:
    ConnectingState(Context& context,const common::BtsId& btsId);
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleTimeout() override;
    void handleDisconnect() override;
    IUeGui::AcceptClose handleUEClose() override;
    void handleIncomingSMS(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &text) override;
    void handleCallRequest(common::MessageId msgId,
                           const common::PhoneNumber &peer,
                           const std::string &enc) override;
private:
    const std::chrono::milliseconds ATTACH_TIMEOUT{500}; // Timeout for attach request
};

}
