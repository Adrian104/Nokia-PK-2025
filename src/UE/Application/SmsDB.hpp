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

struct SmsRecord
{
	clk_t::time_point m_creationTime;
	SmsDir m_direction = SmsDir::UNKNOWN;
	common::PhoneNumber m_from;
	common::PhoneNumber m_to;
	std::string m_message;
};

class SmsDB
{
public:
	void addReceivedSms(
		common::PhoneNumber from,
		common::PhoneNumber to,
		const std::string& message);

	std::vector<SmsRecord>::iterator begin();
	std::vector<SmsRecord>::iterator end();

private:
	std::vector<SmsRecord> m_database;
};
}
