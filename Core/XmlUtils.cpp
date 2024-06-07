#include "Engine/Core/XmlUtils.hpp"

int ParseXmlAttribute(XmlElement const& element, char const* attributeName, int defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		int value = defaultValue;
		Strings strings = SplitStringOnDelimiter(text, ',');
		value = atoi(strings[0].c_str());
		return value;
	}
	else
	{
		return defaultValue;
	}
}

char ParseXmlAttribute(XmlElement const& element, char const* attributeName, char defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		char value = defaultValue;
		value = *text;
		return value;
	}
	else
	{
		return defaultValue;
	}
}

bool ParseXmlAttribute(XmlElement const& element, char const* attributeName, bool defaultValue)
{

	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		std::string text_string = std::string(text);
		if (text_string == "True" || text_string == "true")
		{
			return true;
		}
		else if (text_string == "False" || text_string == "false")
		{
			return false;
		}
		return defaultValue;
	}
	else
	{
		return defaultValue;
	}

}

float ParseXmlAttribute(XmlElement const& element, char const* attributeName, float defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		float value = defaultValue;
		Strings strings = SplitStringOnDelimiter(text, ',');
		value = static_cast<float>(atof(strings[0].c_str()));
		return value;
	}
	else
	{
		return defaultValue;
	}
}

Rgba8 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		Rgba8 value = defaultValue;
		value.SetFromText(text);
		return value;
	}
	else
	{
		return defaultValue;
	}	

}

Vec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		Vec2 value = defaultValue;
		value.SetFromText(text);
		return value;
	}
	else
	{
		return defaultValue;
	}
}

IntVec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		IntVec2 value = defaultValue;
		value.SetFromText(text);
		return value;
	}
	else
	{
		return defaultValue;
	}
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, std::string const& defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		std::string string = defaultValue;
		string = std::string(text);
		return string;
	}
	else
	{
		return defaultValue;
	}
}

Strings ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		Strings strings = defaultValues;
		strings = SplitStringOnDelimiter(text, ',');
		return strings;
	}
	else
	{
		return defaultValues;
	}
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue)
{
	if (element.FindAttribute(attributeName))
	{
		const char* text = element.FindAttribute(attributeName)->Value();
		std::string string = std::string(defaultValue);
		string = std::string(text);
		return string;
	}
	else
	{
		return defaultValue;
	}
}
