#include <Timer.h>

Timer::Timer()
{
	StartTicks = 0;
	PausedTicks = 0;

	Paused = false;
	Started = false;
};

void Timer::start() {
	Started = true;
	Paused = false;
	StartTicks = SDL_GetTicks();
	PausedTicks = 0;
};

void Timer::stop() {
	Started = false;
	Paused = false;
	StartTicks = 0;
	PausedTicks = 0;
};

void Timer::pause() {
	if (Started && !Paused)
	{
		Paused = true;
		PausedTicks = SDL_GetTicks() - StartTicks;
		StartTicks = 0;
	}
};

void Timer::unpause() {
	if (Started && Paused)
	{
		Paused = false;
		StartTicks = SDL_GetTicks() - PausedTicks;
		PausedTicks = 0;
	}
};

Uint32 Timer::getTicks() {
	Uint32 time = 0;

	if (Started)
	{
		if (Paused)
		{
			time = PausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - StartTicks;
		}
	}
	return time;
};

bool Timer::isStarted()
{
	return Started;
}

bool Timer::isPaused()
{
	return Paused && Started;
}