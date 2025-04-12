#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "SmsDB.hpp"
#include <memory>

namespace ue
{

struct Context
{
    common::PrefixedLogger logger;
    SmsDB smsdb;
    IBtsPort &bts;
    IUserPort &user;
    ITimerPort &timer;
    std::unique_ptr<IEventsHandler> state{};

    Context(common::ILogger &iLogger, IBtsPort &bts, IUserPort &user, ITimerPort &timer)
        : logger(iLogger, "[CTX] "), bts(bts), user(user), timer(timer)
    {}

    template <typename State, typename... Arg> void setState(Arg &&...arg)
    {
        // Names of states are mangled, which is compiler specific,
        // but are good enough for logging
        logger.logInfo("setState, from: ",
                       state ? typeid(*state).name() : "null",
                       " to: ",
                       typeid(State).name());
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};

}
