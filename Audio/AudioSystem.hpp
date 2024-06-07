#pragma once


//-----------------------------------------------------------------------------------------------
#include "ThirdParty/fmod/fmod.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include <string>
#include <vector>
#include <map>


//-----------------------------------------------------------------------------------------------
typedef size_t SoundID;
typedef size_t SoundPlaybackID;
typedef NamedStrings EventArgs;
constexpr size_t MISSING_SOUND_ID = (size_t)(-1); // for bad SoundIDs and SoundPlaybackIDs


//-----------------------------------------------------------------------------------------------
class AudioSystem;
struct Vec3;
extern AudioSystem* g_theAudio;
struct AudioConfig
{

};

struct SoundInfo  
{
	std::string			m_soundName;
	SoundID				m_soundID = MISSING_SOUND_ID;
	SoundID				m_soundPlayBackID = MISSING_SOUND_ID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////
class AudioSystem
{
public:
	AudioSystem();
	AudioSystem(AudioConfig const& config);
	virtual ~AudioSystem();

public:
	void						Startup();
	void						Shutdown();
	virtual void				BeginFrame();
	virtual void				EndFrame();

	virtual SoundID				CreateOrGetSound(const std::string& soundFilePath, bool is2DSound = true);
	virtual SoundPlaybackID		StartSound( SoundID soundID, bool isLooped=false, float volume=1.f, float balance=0.0f, float speed=1.0f, bool isPaused=false );
	virtual void				StopSound( SoundPlaybackID soundPlaybackID );
	virtual void				SetSoundPlaybackVolume( SoundPlaybackID soundPlaybackID, float volume );	// volume is in [0,1]
	virtual void				SetSoundPlaybackBalance( SoundPlaybackID soundPlaybackID, float balance );	// balance is in [-1,1], where 0 is L/R centered
	virtual void				SetSoundPlaybackSpeed( SoundPlaybackID soundPlaybackID, float speed );		// speed is frequency multiplier (1.0 == normal)
	virtual void				SetSoundPlaybackPaused(SoundPlaybackID soundPlaybackID, bool isPaused = false);
	virtual void				ValidateResult( FMOD_RESULT result );

	void						MuteAllSounds();
	void						UnmuteAllSounds();

	static bool					MuteAll(EventArgs const& args);
	static bool					UnMuteAll(EventArgs const& args);

	void						SetNumListeners(int numListeners);
	void						UpdateListener(int listenerIndex, const Vec3& listenerPosition, const Vec3& listenerForward, const Vec3& listenerUp);
	virtual SoundPlaybackID		StartSoundAt(SoundID soundID, const Vec3& soundPosition, bool isLooped = false, float volume = 1.0f, float balance = 0.0f, float speed = 1.0f, bool isPaused = false);
	virtual void				SetSoundPosition(SoundPlaybackID soundPlaybackID, const Vec3& soundPosition);
	bool						IsPlaying(SoundPlaybackID soundPlaybackID);


	AudioConfig const&			GetConfig() const;

protected:
	FMOD::System*						m_fmodSystem;
	std::map< std::string, SoundID >	m_registeredSoundIDs;
	std::vector<SoundPlaybackID>		m_registeredSoundPlaybackIDs; 
	std::vector< FMOD::Sound* >			m_registeredSounds;
	bool								m_isMuted = false;
private:
	AudioConfig					m_config;
};

