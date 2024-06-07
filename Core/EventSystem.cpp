#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/NamedStrings.hpp"

EventSystem* g_theEventSystem = nullptr;

EventSystem::EventSystem(EventSystemConfig const& config)
	:m_config(config)
{

}

EventSystem::~EventSystem()
{
}

void EventSystem::StartUp()
{

}

void EventSystem::Shutdown()
{
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr)
{
	auto iter = m_subscriptionListByEventName.find(eventName);

	if (iter != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscription = iter->second;

		for (int i = 0; i < subscription.size(); ++i)
		{
			if (subscription[i] == nullptr)
			{
				subscription[i] = functionPtr;
				return;
			}
		}
		subscription.push_back(functionPtr);
	}
	else 
	{
		m_subscriptionListByEventName[eventName].push_back(functionPtr);
	}
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr)
{
	auto iter = m_subscriptionListByEventName.find(eventName);

	if (iter != m_subscriptionListByEventName.end()) 
	{
		SubscriptionList& subscription = iter->second;

		for (int i = 0; i < subscription.size(); ++i)
		{
			if (functionPtr == subscription[i])
			{
				subscription[i] = nullptr;
			}
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	auto iter = m_subscriptionListByEventName.find(eventName);

	if (iter != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscription = iter->second;

		if ((subscription.size() == 0))
		{
			g_theConsole->AddLine(g_theConsole->ERROR, "NO REGISTERED SUBSCRIBERS!");
			return;
		}

		for (int i = 0; i < subscription.size(); ++i)
		{
			if (subscription[i] != nullptr)
			{
				subscription[i](args);
			}
		}
	}
	else
	{
		if (g_theConsole!=nullptr)
		{
			g_theConsole->AddLine(g_theConsole->ERROR, "NO SUCH COMMAND IN THE COMMAND LIST!");
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName)
{
	NamedStrings nullStrings;
	FireEvent(eventName, nullStrings);
}


std::vector<std::string> EventSystem::GetAllDevConsoleRegisteredCommands()
{
	std::vector<std::string> commandList;
	commandList.reserve(m_subscriptionListByEventName.size());

	for (const auto& keyPair : m_subscriptionListByEventName)
	{
		commandList.push_back(keyPair.first);
	}

	return commandList;
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr)
{
	g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr)
{
	g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
}

void FireEvent(std::string const& eventName, EventArgs& args)
{
	g_theEventSystem->FireEvent(eventName, args);
}

void FireEvent(std::string const& eventName)
{
	g_theEventSystem->FireEvent(eventName);
}
