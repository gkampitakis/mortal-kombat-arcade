#include "Fighter.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"
#include "SpriteHolder.h"
#include "MusicPlayer.h"

Fighter::Fighter(string Name, Point position) {
	name = Name;
	health = 1.0;
	win = 0;
	sprite = new Sprite(position, AnimationFilmHolder::Get()->GetFilm(Name + ".stance"), SpriteTypes::FIGHTER, Fighter::name._Equal("subzero") ? true : false);
	animator = new FrameRangeAnimator();
	projectileAnimator = new FrameRangeAnimator();
	projectile = new Sprite({ 0,0 }, AnimationFilmHolder::Get()->GetFilm(name + ".projectile"), SpriteTypes::PROJECTILE);
	projectile->SetVisibility(false);
	tickAnimator = new TickTimerAnimator(NULL);
};

bool Fighter::initialize(const string& path) {
	try {
		json config;
		std::ifstream config_file(path, std::ifstream::binary);
		input::key_combination keycodes;
		config_file >> config;
		for (json::iterator it = config["combinations"].begin(); it != config["combinations"].end(); it++) {
			keycodes.clear();
			int i = 0;
			json tmp = *it;
			string action = tmp["action"];
			for (json::iterator i = tmp["key_strokes"].begin(); i != tmp["key_strokes"].end(); i++) {
				keycodes.push_back(*i);
			}
			Fighter::inputController.AddAction(keycodes, action);
		}
		stateTransitions.SetState("READY");
		setStateMachine();

		return true;//true or false catch here
	}
	catch (const std::exception& e) {
		cerr << e.what();
		return false;
	}
};

void Fighter::Draw(SDL_Surface& surface, Point enemy, Rect& camera) {
	sprite->SetEnemy(enemy);
	int width = (SCREEN_WIDTH - sprite->getFilm()->GetFrameBox(sprite->GetFrameNo()).w * 2) / 6;
	int height = (SCREEN_WIDTH - sprite->getFilm()->GetFrameBox(sprite->GetFrameNo()).h * 2) / 3;
	using Input = logic::StateTransitions::Input;

	sprite->DisplayCamera(surface, width, height, camera);
	projectile->DisplayCamera(surface, projectile->getFilm()->GetFrameBox(projectile->GetFrame()).w * 3, projectile->getFilm()->GetFrameBox(projectile->GetFrame()).h * 3, camera);

	Input tmpInput;
	tmpInput.insert(Make_key(inputController.GetLogical()));
	Fighter::stateTransitions.PerformTransitions(tmpInput, false);
};

void Fighter::setStateMachine() {
	using Input = logic::StateTransitions::Input;
	stateTransitions.
		/*
		*  PUNCHES-> LOW/HIGH AND PUNCH
		*/
		SetTransition("READY", Input{ ".PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {//This might is not needed ill check it at combos
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {

				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(rand() % 2 + 1 == 2 ? AnimationFilmHolder::Get()->GetFilm(name + ".punch1") : AnimationFilmHolder::Get()->GetFilm(name + ".punch2"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? PUNCH_DIST : -PUNCH_DIST, 0, PUNCH_DELAY, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
				nextAction = "punch";
				MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("punchWave"), 0);
				stateTransitions.SetState("READY");
			}
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN.PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".lowpunch"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, LOW_PUNCH_DELAY, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
				nextAction = "downpunch";
				MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("punchWave"), 0);
			}
		});
	})
		.SetTransition("DOWN", Input{ ".BCK.DOWN.SPECIAL" }, [&](void) {
		SetActionWithAnimator([&]() {//HINT: Hit them all at once
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".duck")) {
				Fighter::name._Equal("subzero") ? MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("subzerocombowave"), 0) : MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("scorpioncombowave"), 0);
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".combo1"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 180, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);

				if (Fighter::name._Equal("subzero"))
					projectile->SetX(sprite->GetPosition().x + 40);
				else
					projectile->SetX(sprite->GetPosition().x - 190);

				projectile->SetY(sprite->GetPosition().y + 40);
				projectile->SetEnemy(sprite->GetEnemy());
				projectile->SetVisibility(true);

				projectileAnimator->Start(projectile, new FrameRangeAnimation(0, projectile->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? 270 : -330, 0, 400, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(projectileAnimator);
				nextAction = "combo1";
				stateTransitions.SetState("DOWN");
			}
		});
	})
		.SetTransition("READY", Input{ ".DOWN.KICK.SPECIAL" }, [&](void) {
		SetActionWithAnimator([&]() {//HINT: Hit them all at once
			stateTransitions.SetState("DOWN");
		});
	})

		.SetTransition("UP", Input{ ".PUNCH" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".highpunch"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? HIGH_PUNCH_DIST : -HIGH_PUNCH_DIST, 0, HIGH_PUNCH_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
			nextAction = "uppunch";
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("punchWave"), 0);
		}
	})
		/*
		* KICKS-> LOW/HIGH AND KICK
		*/
		.SetTransition("READY", Input{ ".KICK" }, [&](void) {
		SetActionWithAnimator([&]() {//This might is not needed ill check it at combos
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(rand() % 2 + 1 == 2 ? AnimationFilmHolder::Get()->GetFilm(name + ".kick1") : AnimationFilmHolder::Get()->GetFilm(name + ".kick2"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? KICK_DIST : -KICK_DIST, 0, KICK_DELAY, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
				nextAction = "kick";
				MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("kickWave"), 0);
				stateTransitions.SetState("READY");
			}
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN.KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".lowkick"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, LOW_KICK_DELAY, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
				nextAction = "downkick";
				MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("kickWave"), 0);
			}
		});
	})
		.SetTransition("UP", Input{ ".KICK" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".highkick1"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? HIGH_KICK_DIST : -HIGH_KICK_DIST, 0, HIGH_KICK_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
			nextAction = "upkick";
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("kickWave"), 0);
		}
	})
		/*
		* MOVES-> UP/BACK/FORWARD/DOWN
		*/
		.SetTransition("READY", Input{ ".UP" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			nextAction = "waiting";
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".up"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, -JUMP_DIST, JUMP_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
		}
	})
		.SetTransition("READY", Input{ ".BCK" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".move"));

			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -MOVE_DIST : MOVE_DIST, 0, MOVE_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("READY");
		}
	})
		.SetTransition("READY", Input{ ".FWD" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".move"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? MOVE_DIST : -MOVE_DIST, 0, MOVE_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("READY");
		}
	})
		.SetTransition("READY", Input{ ".DOWN" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".down"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, DUCK_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("DOWN");
		}
	})
		.SetTransition("DOWN", Input{ ".DOWN" }, [&](void) {
		if (animator->HasFinished()) {
			nextAction = "waiting";
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".duck"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, DUCK_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		/*
		* BLOCKS->BLOCK/BLOCK DOWN
		*/
		.SetTransition("READY", Input{ ".BLOCK" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(animator);
			nextAction = "block";
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".block"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, BLOCK_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("BLOCK");
		}
	})
		.SetTransition("DOWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			stateTransitions.SetState("BLOCKDWN");
		});
	})
		.SetTransition("BLOCK", Input{ ".BLOCK" }, [&](void) {
		if (animator->HasFinished()) {//This might is not needed ill check it at combos
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".stillblock"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, STILL_BLOCK_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("BLOCK");
		}
	})
		.SetTransition("BLOCKDWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {//This might is not needed ill check it at combos
			nextAction = "blockdwn";
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".downblock"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, LOW_BLOCK_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		/*
		* DEFAULT TRANSITIONS
		*/
		.SetTransition("DOWN", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			nextAction = "waiting";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("UP", Input{}, [&](void) {
		if (tickAnimator&&tickAnimator->GetState() != ANIMATOR_RUNNING) {
			TickTimerAnimation* tmp2 = new TickTimerAnimation(10);
			tmp2->setOnTick([&] {
				if (sprite->GetPosition().y + JUMP_DIST < 500) sprite->Move({ 0,JUMP_DIST });
			}).SetDelay(400).SetReps(3);
			tickAnimator = new TickTimerAnimator(tmp2);
			tickAnimator->SetOnFinish([&]() {
				sprite->SetY(500);//Error Correction
				stateTransitions.SetState("READY");
			});
			tickAnimator->Start(SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(tickAnimator);
		}
	})
		.SetTransition("BLOCK", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			nextAction = "waiting";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("BLOCKDWN", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			nextAction = "waiting";
			stateTransitions.SetState("DOWN");
		});
	})
		.SetTransition("READY", Input{}, [&](void) {
		if (animator->HasFinished()) {
			nextAction = "waiting";
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".stance"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, Fighter::name._Equal("subzero") ? SUBZERO_STANCE_DELAY : SCORPION_STANCE_DELAY, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		/*
		* COMBOS
		*/
		.SetTransition("DOWN", Input{ ".DOWN.FWD" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {
			AnimatorHolder::Remove(animator);
			nextAction = "dwnflip";
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipdown"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? DOWN_FLIP_DIST : -DOWN_FLIP_DIST, 0, DOWNFLIP_DELAY, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("READY");
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("jump"), 0);
		}
	})
		.SetTransition("DOWN", Input{ ".BCK.DOWN" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipdown"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -DOWN_FLIP_DIST : DOWN_FLIP_DIST, 0, DOWNFLIP_DELAY, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("READY");
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("jump"), 0);
		}
	})
		.SetTransition("UP", Input{ ".FWD" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipup"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? UP_FLIP_DIST : -UP_FLIP_DIST, 0, UPFLIP_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("jump"), 0);
		}
	})
		.SetTransition("UP", Input{ ".BCK" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipup"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -UP_FLIP_DIST : UP_FLIP_DIST, 0, UPFLIP_DELAY, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
			MusicPlayer::Get()->PlayEffect(MusicPlayer::Get()->RetrieveEffect("jump"), 0);
		}
	})
		.SetTransition("FlipFWD", Input{ ".FWD.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("FlipFWD", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("FlipBCK", Input{ ".FWD.BCK" }, [&](void) {
		SetActionWithAnimator([&]() {
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("FlipBCK", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("WIN", Input{ "." }, [&](void) {
	})
		.SetTransition("WIN", Input{}, [&](void) {
		if (animator->HasFinished() || !sprite->getFilm()->GetId()._Equal(name + ".win")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".win"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, WIN_DELAY, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		.SetTransition("LOSE", Input{ "." }, [&](void) {
	})
		.SetTransition("LOSE", Input{}, [&](void) {
		if (animator->HasFinished() || !sprite->getFilm()->GetId()._Equal(name + ".dizzy")) {//the second migth need delete
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".dizzy"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, LOSE_DELAY, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	});
};

void Fighter::Handler() {
	inputController.Handle();
};

void Fighter::SetActionWithAnimator(const std::function<void()>& f) {
	if (tickAnimator&&tickAnimator->GetState() != ANIMATOR_RUNNING) {
		TickTimerAnimation* tmp2 = new TickTimerAnimation(10);
		tmp2->setOnTick([] {
			//Nothing to do here
		}).SetDelay(FIGHTER_ACTION_DELAY_MSECS).SetReps(1);
		tickAnimator = new TickTimerAnimator(tmp2);
		tickAnimator->SetOnFinish(f);
		tickAnimator->Start(SDL_GetTicks());
		AnimatorHolder::MarkAsRunning(tickAnimator);
	}
};

const string Fighter::Make_key(const Input& input) const {
	std::string result;
	for (auto& s : input)
		result += "." + s;
	return result;
}

Point Fighter::GetPosition(void) const {
	return sprite->GetPosition();
};

void Fighter::SetState(string state) {
	stateTransitions.SetState(state);
};

string Fighter::GetState(void) const {
	return stateTransitions.GetState();
};

string Fighter::GetName(void) {
	return name;
}

void  Fighter::removeHealth(float h) {
	if (health - h > 0) {
		health = health - h;
	}
	else health = 0;
};

float Fighter::getHealth(void) const {
	return health;
};

int Fighter::GetWin(void) const {
	return win;
};

void Fighter::SetWin(void) {
	win++;
};

bool Fighter::proximityDetector(Sprite* enemy) {
	return sprite->CollisionCheck(enemy);//Here also add the "missile" detection for combos
};

Sprite* Fighter::GetSprite(void)const {
	return sprite;
};

string Fighter::GetAction(void)const {
	return nextAction;
};

void Fighter::ResetHealth(void) {
	health = 1.0;
};

void Fighter::ResetPosition(Point x) {
	sprite->SetY(x.y);
	sprite->SetX(x.x);
};

void Fighter::InflictionAnimation(string Animation, int speed, string hit) {
	if (hit._Equal("punch")) fightstasts.received_punches++;
	else if (hit._Equal("kick")) fightstasts.received_kicks++;
	else if (hit._Equal("downhit") || hit._Equal("combo"))  fightstasts.received_signif_strikes++;

	sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + "." + Animation));
	animator->Start(sprite,
		new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, speed, false, 60), SDL_GetTicks());
	//We ll add and movement later
};

void Fighter::HideProjectile(void) {
	nextAction = "waiting";
	projectile->SetVisibility(false);
};
