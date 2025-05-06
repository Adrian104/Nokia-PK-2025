#include "SmsDB.hpp"

namespace ue
{
void SmsDB::addReceivedSms(
	common::PhoneNumber from,
	common::PhoneNumber to,
	const std::string& message)
{
	SmsRecord sms{};
	sms.m_creationTime = clk_t::now();
	sms.m_direction = SmsDir::RECEIVED;
	sms.m_from = from;
	sms.m_to = to;
	sms.m_message = message;

	m_database.emplace_back(std::move(sms));
}

void SmsDB::addSms(
    common::PhoneNumber from,
    common::PhoneNumber to,
    const std::string& message)
{
    SmsRecord sms{};
    sms.m_creationTime = clk_t::now();
    sms.m_direction = SmsDir::SENT;
    sms.m_from = from;
    sms.m_to = to;
    sms.m_message = message;

    m_database.emplace_back(std::move(sms));
}

void SmsDB::markLastSmsSentAsFailed()
{
    for (auto it = m_database.rbegin(); it != m_database.rend(); ++it)
    {
        if (it->m_direction == SmsDir::SENT)
        {
            it->m_status = SmsStatus::FAILED;
            return;
        }
    }
}

std::vector<SmsRecord>::iterator SmsDB::begin()
{
	return m_database.begin();
}

std::vector<SmsRecord>::iterator SmsDB::end()
{
	return m_database.end();
}
}
