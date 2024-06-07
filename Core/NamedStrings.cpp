#include "Engine/Core/NamedStrings.hpp"

void NamedStrings::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	const XmlAttribute* attribute = element.FirstAttribute();

	m_keyValuePairs.clear();

	while (attribute != nullptr) 
	{
		SetValue(attribute->Name(), attribute->Value());
		attribute = attribute->Next();
	}

}

void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);

	if (iter == m_keyValuePairs.end()) 
	{
		return defaultValue;
	}
	else
	{
		return iter->second;
	}
}

bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);

	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		if (iter->second =="True"||iter->second =="true")
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
}

int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		return atoi((iter->second).c_str());
	}
}

float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		return (float)atof((iter->second).c_str());
	}
}

std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return std::string(defaultValue);
	}
	else
	{
		return iter->second;
	}
}

Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		Rgba8 colorValue = Rgba8();
		colorValue.SetFromText((iter->second).c_str());
		return colorValue;
	}
}

Vec2 NamedStrings::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		Vec2 value = Vec2();
		value.SetFromText((iter->second).c_str());
		return value;
	}
}

IntVec2 NamedStrings::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		IntVec2 value = IntVec2();
		value.SetFromText((iter->second).c_str());
		return value;
	}
}

Vec3 NamedStrings::GetValue(std::string const& keyName, Vec3 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		Vec3 value = Vec3();
		value.SetFromText((iter->second).c_str());
		return value;
	}

}

EulerAngles NamedStrings::GetValue(std::string const& keyName, EulerAngles const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		EulerAngles value = EulerAngles();
		Strings strings = SplitStringOnDelimiter((iter->second).c_str(), ',');
		value.m_yawDegrees = (float)atof(strings[0].c_str());
		value.m_pitchDegrees = (float)atof(strings[1].c_str());
		value.m_rollDegrees = (float)atof(strings[2].c_str());
		return value;
	}

}

FloatRange NamedStrings::GetValue(std::string const& keyName, FloatRange const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator iter = m_keyValuePairs.find(keyName);
	if (iter == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	else
	{
		FloatRange value = FloatRange();
		Strings strings = SplitStringOnDelimiter((iter->second).c_str(), '~');
		value.m_min = (float)atof(strings[0].c_str());
		value.m_max = (float)atof(strings[1].c_str());
		return value;
	}

}
