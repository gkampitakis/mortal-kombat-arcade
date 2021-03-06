#include "Game.h"
#include "AnimatorHolder.h"
#include "MusicPlayer.h"

bool Game::start = false;
bool Game::EndOfGame = false;

Game::Game() {
	timeAnimator = new TickTimerAnimator(NULL);
	HitAnimator = new TickTimerAnimator(NULL);
	Animator = new TickTimerAnimator(NULL);
	timeAnimation = new TickTimerAnimation(111);
	ProjectileAnimation = new TickTimerAnimation(120);
	round = 1;
	camera = { STAGE_WIDTH / 2 - SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
};

Game::~Game() {
	CleanUp();
};

bool Game::initialize(SDL_Surface* surface) {
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm("stage");
	background = tmp->GetBitmap();
	AnimationFilm* temp = AnimationFilmHolder::Get()->GetFilm("bckg");
	movingBckg = temp->GetBitmap();
	//Init fonts
	Timerfont = TTF_OpenFont("media/font.ttf", 70);
	if (Timerfont == NULL)
	{
		cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
		return false;
	}

	Namefont = TTF_OpenFont("media/font.ttf", 45);
	if (Namefont == NULL)
	{
		cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
		return false;
	}
	//call the initialization of players

	subzero = new Fighter("subzero", { 580,500 });
	scorpion = new Fighter("scorpion", { 1280,500 });
	if (!subzero->initialize("config/subzero_controller.json")) return false;
	if (!scorpion->initialize("config/scorpion_controller.json")) return false;

	return true;
};


void Game::DrawGame(SDL_Surface& surface) {

	if (start) {
		collisionNhits(*subzero, *scorpion);
		collisionNhits(*scorpion, *subzero);
	}

	MatchEnd(surface);
	cameraAdjustment();

	SDL_Rect fullscreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_BlitScaled(movingBckg, 0, &surface, &fullscreen);
	SDL_BlitSurface(background, &camera, &surface, &fullscreen);
	//For debugging purposes the timer is big 
	printTimer(gameTimer.ReverseTimer(GAME_TIMER), { SCREEN_WIDTH / 2 - 35, 5 }, &surface, { 198, 0, 10, 255 });

	if (!start&&timeAnimator->GetState() == ANIMATOR_RUNNING) {
		ResetMatch();
		printMessage("Round " + to_string(round), { SCREEN_WIDTH / 2 - 180,SCREEN_HEIGHT / 2 - 200 }, &surface, { 255, 255, 0, 255 }, 150);
	}

	//The camera might need moving or interaction with the playerres 
	if (rand() % 2 + 1 == 2) {
		scorpion->Draw(surface, subzero->GetPosition(), camera);
		subzero->Draw(surface, scorpion->GetPosition(), camera);
	}
	else {//illusion of being at the same z-order they never must collapse
		subzero->Draw(surface, scorpion->GetPosition(), camera);
		scorpion->Draw(surface, subzero->GetPosition(), camera);
	}

	//x,y, height/width changed the orientation in function
	RenderHpBarLeft(subzero->getHealth(), surface);
	RenderHpBarRight(scorpion->getHealth(), surface);
};

void Game::CleanUp() {
	AnimatorHolder::CleanUp();
};

void Game::HandleInput(SDL_Event& event) {
	if (!Game::start) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_SPACE && !EndOfGame) {
				DelayAction([&]() {
					AnimatorHolder::Remove(timeAnimator);
					Game::start = true;
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("fight"), 0);
					gameTimer.start();
				}, 1000);
			}
		}
	}
	else {
		subzero->Handler();
		scorpion->Handler();
	}
};


void Game::printTimer(const std::string& msg, Point position, SDL_Surface *surface, SDL_Color color) {
	SDL_Rect dest = { position.x,position.y,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(Timerfont, msg.c_str(), color);

	if (stext) {
		SDL_BlitSurface(stext, NULL, surface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}
};

void Game::RenderHpBarRight(float healt, SDL_Surface& surface) {
	Rect bar = { 679, 65, 45, 580 };
	healt = healt > 1.f ? 1.f : healt < 0.f ? 0.f : healt;

	SDL_Surface* tmp = SDL_CreateRGBSurface(0, bar.h, bar.w, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 198, 0, 10));
	SDL_BlitSurface(tmp, NULL, &surface, &bar);
	SDL_FreeSurface(tmp);

	int pw = (int)((float)bar.w * healt);

	tmp = SDL_CreateRGBSurface(0, pw, bar.h, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 0, 128, 0));
	Rect tempRect = { bar.x + (bar.w - pw),bar.y,bar.w,bar.h };
	SDL_BlitSurface(tmp, NULL, &surface, &tempRect);
	SDL_FreeSurface(tmp);

	//NAME 
	SDL_Rect dest = { 1055,65,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(Namefont, "SCORPION", { 255, 255, 0, 255 });
	if (stext) {
		SDL_BlitSurface(stext, NULL, &surface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}
	tmp = AnimationFilmHolder::Get()->GetFilm("win")->GetBitmap();

	Rect displayWin = { 1210,110,50,50 };
	if (scorpion->GetWin() == 1) {
		SDL_BlitScaled(tmp, NULL, &surface, &displayWin);
	}
	else if (scorpion->GetWin() == 2) {
		SDL_BlitScaled(tmp, NULL, &surface, &displayWin);
		displayWin.x = displayWin.x - 50;
		SDL_BlitScaled(tmp, NULL, &surface, &displayWin);
	}
};

void Game::RenderHpBarLeft(float healt, SDL_Surface& surface) {
	Rect bar = { 24, 65, 45, 580 };
	healt = healt > 1.f ? 1.f : healt < 0.f ? 0.f : healt;

	SDL_Surface* tmp = SDL_CreateRGBSurface(0, bar.h, bar.w, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 198, 0, 10));
	SDL_BlitSurface(tmp, NULL, &surface, &bar);
	SDL_FreeSurface(tmp);

	int pw = (int)((float)bar.w * healt);

	tmp = SDL_CreateRGBSurface(0, pw, bar.h, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 0, 128, 0));
	SDL_BlitSurface(tmp, NULL, &surface, &bar);
	SDL_FreeSurface(tmp);

	//Name
	SDL_Rect dest = { 50,65,0,0 };
	SDL_Surface *stext = TTF_RenderText_Blended(Namefont, "SUBZERO", { 255, 255, 0, 255 });
	if (stext) {
		SDL_BlitSurface(stext, NULL, &surface, &dest);
		SDL_FreeSurface(stext);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
	}
	tmp = AnimationFilmHolder::Get()->GetFilm("win")->GetBitmap();

	Rect displayWin = { 555,110,50,50 };
	if (subzero->GetWin() == 1) {
		SDL_BlitScaled(tmp, NULL, &surface, &displayWin);
	}
	else if (subzero->GetWin() == 2) {
		SDL_BlitScaled(tmp, NULL, &surface, &displayWin);
		displayWin.x = displayWin.x - 50;
		SDL_BlitScaled(tmp, NULL, &surface, &displayWin);
	}
};

void Game::DelayAction(const std::function<void()>& f, delay_t d) {
	if (timeAnimator&&timeAnimator->GetState() != ANIMATOR_RUNNING) {
		timeAnimation->setOnTick([] {
			//Nothing to do here
		}).SetDelay(d).SetReps(1);
		timeAnimator = new TickTimerAnimator(timeAnimation);
		timeAnimator->SetOnFinish(f);
		timeAnimator->Start(SDL_GetTicks());
		AnimatorHolder::MarkAsRunning(timeAnimator);
	}
};

void Game::DelayHitAction(const std::function<void()>& f, delay_t d) {
	if (HitAnimator&&HitAnimator->GetState() != ANIMATOR_RUNNING) {
		timeAnimation->setOnTick([] {
			//Nothing to do here
		}).SetDelay(d).SetReps(1);
		HitAnimator = new TickTimerAnimator(timeAnimation);
		HitAnimator->SetOnFinish(f);
		HitAnimator->Start(SDL_GetTicks());
		AnimatorHolder::MarkAsRunning(HitAnimator);
	}
};


void Game::printMessage(const string& msg, Point position, SDL_Surface *surface, SDL_Color color, int fontsize) {

	SDL_Rect dest = { position.x,position.y,0,0 };
	tmpFont = TTF_OpenFont("media/font.ttf", fontsize);
	if (tmpFont == NULL)
	{
		throw
			std::string("Failed to load lazy font! SDL_ttf Error");
	}
	SDL_Surface *stext = TTF_RenderText_Blended(tmpFont, msg.c_str(), color);

	if (stext) {
		SDL_BlitSurface(stext, NULL, surface, &dest);
		SDL_FreeSurface(stext);
		TTF_CloseFont(tmpFont);
	}
	else {
		throw
			std::string("Couldn't allocate text surface in printMessageAt");
		TTF_CloseFont(tmpFont);
	}
};


void Game::cameraAdjustment() {
	if (subzero->GetPosition().x < camera.x) camera.x = camera.x - 10;
	if (scorpion->GetPosition().x + 194 > camera.x + SCREEN_WIDTH) camera.x = camera.x + 10;
	if (camera.x < 0) camera.x = 0;
	if (camera.x > STAGE_WIDTH - SCREEN_WIDTH) camera.x = STAGE_WIDTH - SCREEN_WIDTH;
};

void Game::matchWin(Fighter& winner, Fighter& loser, SDL_Surface& surface) {
	MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect(winner.GetName() + ".wins"), 0);
	loser.SetState("LOSE");
	winner.SetState("WIN");
	if (winner.GetWin() >= 2)
		EndOfGame = true;
	else round++;
};

int Game::GetRound(void) const {
	return round;
};

void Game::MatchEnd(SDL_Surface& surface) {
	if (!gameTimer.isStarted() && start) {//timer stopped
		Game::start = false;
		if (subzero->getHealth() > scorpion->getHealth()) {
			subzero->SetWin();
			matchWin(*subzero, *scorpion, surface);
		}
		else if (subzero->getHealth() < scorpion->getHealth()) {
			scorpion->SetWin();
			matchWin(*scorpion, *subzero, surface);
		}
		else {
			if (rand() % 2) {
				scorpion->SetWin();
				matchWin(*scorpion, *subzero, surface);
			}
			else {
				subzero->SetWin();
				matchWin(*subzero, *scorpion, surface);
			}
		}
	}
	else if (subzero->getHealth() == 0 && start) {
		Game::start = false;
		gameTimer.stop();
		scorpion->SetWin();
		matchWin(*scorpion, *subzero, surface);
	}
	else if (scorpion->getHealth() == 0 && start) {
		Game::start = false;
		gameTimer.stop();
		subzero->SetWin();
		matchWin(*subzero, *scorpion, surface);
	}
};

void Game::collisionNhits(Fighter& hitter, Fighter& hitted) {

	if (hitter.proximityDetector(hitted.GetSprite())) {
		/*
		 *NORMAL ATTACKS
		 */
		if (hitter.GetAction()._Equal("punch") || hitter.GetAction()._Equal("kick")) {
			if (hitted.GetState()._Equal("BLOCK")) {
				DelayHitAction([&]() {
					AnimatorHolder::Remove(HitAnimator);//Hit blocked
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("block"), 0);
					hitter.fightstasts.blocked++;
				}, hitter.GetAction()._Equal("punch") ? HIT_PUNCH_DELAY : HIT_KICK_DELAY);

			}
			else if (hitted.GetState()._Equal("BLOCKDWN") || hitted.GetState()._Equal("UP") || hitted.GetState()._Equal("DOWN")) {
				//Nothing happens
			}
			else {
				//sound && inflictions animations
				DelayHitAction([&]() {
					float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					if (hitter.GetAction()._Equal("punch")) {
						hitted.removeHealth(PUNCH_DMG*r);
					}
					else {
						hitted.removeHealth(KICK_DMG*r);
					}
					AnimatorHolder::Remove(HitAnimator);
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("singlehit"), 0);
					hitted.InflictionAnimation("singlehit", SINGLE_HIT_DELAY, hitter.GetAction()._Equal("punch") ? "punch" : "kick", 0);
				}, hitter.GetAction()._Equal("punch") ? HIT_PUNCH_DELAY : HIT_KICK_DELAY);
				//blood and tears			
			}
		}/*
		 *UP ATTACKS
		 */
		else if (hitter.GetAction()._Equal("uppunch") || hitter.GetAction()._Equal("upkick")) {
			if (hitted.GetState()._Equal("BLOCK") || hitted.GetState()._Equal("BLOCKDWN")) {
				DelayHitAction([&]() {
					AnimatorHolder::Remove(HitAnimator);//Hit blocked
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("block"), 0);
					hitter.fightstasts.blocked++;
				}, 750);
			}
			else {
				//sound && inflictions animations
				DelayHitAction([&]() {
					float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					if (hitter.GetAction()._Equal("uppunch")) {
						hitted.removeHealth(UPPERCUT_DMG*r);
					}
					else {
						hitted.removeHealth(KICK_DMG*r);
					}
					AnimatorHolder::Remove(HitAnimator);
					hitted.InflictionAnimation("singlehit", SINGLE_HIT_DELAY, hitter.GetAction()._Equal("punch") ? "punch" : "kick", 0);
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("singlehit"), 0);
				}, 750);
				//blood and tears	
			}
		}
		/*
		 *DOWN ATTACKS
		 */
		else if (hitter.GetAction()._Equal("downpunch") || hitter.GetAction()._Equal("downkick")) {
			if (hitted.GetState()._Equal("BLOCKDWN")) {
				//Block hit 
				DelayHitAction([&]() {
					AnimatorHolder::Remove(HitAnimator);//Hit blocked
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("block"), 0);
					hitter.fightstasts.blocked++;
				}, hitter.GetAction()._Equal("downpunch") ? HIT_LOW_PUNCH_DELAY : HIT_LOW_KICK_DELAY);

			}
			else if (hitted.GetState()._Equal("UP")) {
				//Nothing happens
			}
			else {
				//Here reduce health maybe depending on hit 


				//sound && inflictions animations
				DelayHitAction([&]() {
					float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					if (hitter.GetAction()._Equal("downpunch")) {
						hitted.removeHealth(PUNCH_DMG*r);
					}
					else {
						hitted.removeHealth(KICK_DMG);
					}
					AnimatorHolder::Remove(HitAnimator);
					MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("singlehit"), 0);
					hitted.InflictionAnimation("uppercuthit", DOWN_HIT_DELAY, "downhit", 0);

				}, hitter.GetAction()._Equal("downpunch") ? HIT_LOW_PUNCH_DELAY : HIT_LOW_KICK_DELAY);
				//blood and tears
			}
		}
	}
	/*
	 * COMBOS
	 */
	else if (hitter.GetAction()._Equal("combo1")) {
		if (hitted.GetState()._Equal("BLOCK")) {
			DelayHitAction([&]() {
				hitter.HideProjectile();
				AnimatorHolder::Remove(HitAnimator);//Hit blocked
				MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("block"), 0);
				hitter.fightstasts.blocked++;

			}, 600);
		}
		else if (hitted.GetState()._Equal("BLOCKDWN") || hitted.GetState()._Equal("UP") || hitted.GetState()._Equal("DOWN") || hitted.GetState()._Equal("DISABLED")) {
			if (Animator&&Animator->GetState() != ANIMATOR_RUNNING) {
				ProjectileAnimation->setOnTick([] {
					//Nothing to do here
				}).SetDelay(1000).SetReps(1);
				Animator = new TickTimerAnimator(ProjectileAnimation);
				Animator->SetOnFinish([&] {
					AnimatorHolder::Remove(Animator);
					hitter.HideProjectile();
				});
				Animator->Start(SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(Animator);
			}
		}
		else {
			//sound && inflictions animations
			DelayAction([&]() {
				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				hitter.HideProjectile();
				hitted.removeHealth(COMBO_DMG*r);

				AnimatorHolder::Remove(timeAnimator);
				MusicPlayer::Get()->PlayEffect(hitter.GetName()._Equal("subzero") ? MusicPlayer::Get()->RetrieveEffect("subzerocombohit") : MusicPlayer::Get()->RetrieveEffect("scorpioncombohit"), 0);

				hitted.InflictionAnimation("disabled", 250, "combo", hitter.GetName()._Equal("subzero") ? 0 : 70);
				hitted.SetState("DISABLED");
				TickTimerAnimator* temp = new TickTimerAnimator(NULL);
				TickTimerAnimation* tempAnimation = new TickTimerAnimation(20);
				tempAnimation->setOnTick([] {
					//Nothing to do here
				}).SetDelay(DISABLED_DELAY).SetReps(1);
				temp = new TickTimerAnimator(tempAnimation);
				temp->SetOnFinish([&] {
					AnimatorHolder::Remove(temp);
					hitted.SetState("READY");
				});
				temp->Start(SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(temp);
		}, 450);			
	}
}
};

Fighter* Game::GetWinner(void) {
	return subzero->GetWin() >= 2 ? subzero : scorpion;
};

Fighter* Game::GetLoser(void) {
	return subzero->GetWin() >= 2 ? scorpion : subzero;
};

void Game::ResetMatch(void) {
	scorpion->SetState("READY");
	subzero->SetState("READY");
	scorpion->ResetHealth();
	subzero->ResetHealth();
	subzero->ResetPosition({ 580,500 });
	scorpion->ResetPosition({ 1280,500 });

	//here call a center camera function
	camera.x = 364;
};