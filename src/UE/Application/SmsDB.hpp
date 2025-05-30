#pragma once
#include "Messages/PhoneNumber.hpp"
#include <vector>
#include <string>
#include <chrono>

namespace ue
{
using clk_t = std::chrono::system_clock;

enum class SmsDir
{
	SENT,
	RECEIVED,
	UNKNOWN
};

enum class SmsStatus
{
	SUCCESS,
	FAILED
};

// Each message is represented by SmsRecord
struct SmsRecord
{
	clk_t::time_point m_creationTime; // Message creation time
	SmsDir m_direction = SmsDir::UNKNOWN; // Direction of SMS transmission
	common::PhoneNumber m_from; // Phone number of sender
	common::PhoneNumber m_to; // Phone number of receiver
	std::string m_message; // Message of the SMS
	SmsStatus m_status = SmsStatus::SUCCESS; // Status of the SMS
	bool m_isNew = false; // If true, the message is unread.

	std::string getTitle() const;
};

class SmsDB
{
public:
	// Adds SMS to the database
	void addReceivedSms(
		common::PhoneNumber from,
		common::PhoneNumber to,
		const std::string& message);

	// Adds sent SMS to the database
    void addSms(
        common::PhoneNumber from,
        common::PhoneNumber to,
        const std::string& message);

	void markLastSmsSentAsFailed();
	bool hasUnreadMessages() const;

	// SmsDB has begin() and end(), so it can be easily iterated over all the messages in the database.
	std::vector<SmsRecord>::iterator begin();
	std::vector<SmsRecord>::iterator end();

private:
	std::vector<SmsRecord> m_database;
};
}
