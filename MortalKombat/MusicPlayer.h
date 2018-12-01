#include "MyLibraries.h"

class MusicPlayer {
private:
	MusicPlayer(void) {};
	~MusicPlayer() { CleanUp(); }

	typedef map<string, Mix_Music*> Playlist;
	typedef map<string, Mix_Chunk*> SoundEffect;

	SoundEffect soundeffects;
	Playlist playlist;

	static MusicPlayer *musicplayer;//singleton
public:
	static MusicPlayer *Get(void);
	void LoadMusic(const string &path, string id);
	void LoadEffect(const string &path, string id);

	Mix_Music* RetrieveMusic(const string) const;
	Mix_Chunk* RetrieveEffect(const string) const;

	void PlayMusic(Mix_Music *music);
	void PlayEffect(Mix_Chunk* effect, int loops);
	void PauseMusic(void);//If called twice resumes the music
	void StopMusic(void);

	void CleanUp();
};
	