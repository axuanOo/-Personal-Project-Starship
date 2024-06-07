#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include <map>



struct EventSystemConfig
{

};

class EventSystem
{
public:

	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void StartUp();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void SubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr);
	void FireEvent(std::string const& eventName, EventArgs& args);													// For have parameters functions
	void FireEvent(std::string const& eventName);																	// For no parameter functions


	std::vector<std::string> GetAllDevConsoleRegisteredCommands();

protected:
	EventSystemConfig									m_config;
	std::map<std::string, SubscriptionList>				m_subscriptionListByEventName;

private:
};

///---------------------------------------------------------------------------------------------------------------------------------
/// Standalone global namespace helper functions, find the event system and call the function inside it
/// 
void SubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventSystemCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);													
void FireEvent(std::string const& eventName);