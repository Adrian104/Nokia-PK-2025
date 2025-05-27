#include "SmsDB.hpp"

namespace ue
{
std::string SmsRecord::getTitle() const
{
    std::string buffer;

    if (m_isNew)
        buffer += "(NEW) ";

    if (m_direction == SmsDir::RECEIVED)
        buffer += "from: " + std::to_string(m_from.value);
    else if (m_direction == SmsDir::SENT)
        buffer += "to: " + std::to_string(m_from.value);

    return buffer;
}

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
	sms.m_isNew = true;

	m_database.push_back(std::move(sms));
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
    sms.m_isNew = false;

    m_database.push_back(std::move(sms));
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

bool SmsDB::hasUnreadMessages() const
{
    for (const SmsRecord& rec : m_database)
        if (rec.m_isNew)
            return true;

    return false;
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
