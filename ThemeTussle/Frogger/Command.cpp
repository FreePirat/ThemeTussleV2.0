#include "Command.h"

Command::Command()
{
}

Command::Command(const std::string& name, const std::string& type)
	: m_name(name)
	, m_type(type)
{
}

const std::string& Command::name() const
{
	return m_name;
}

const std::string& Command::type() const
{
	return m_type;
}

std::string Command::toString() const
{
	return m_name + ":" + m_type;
}
