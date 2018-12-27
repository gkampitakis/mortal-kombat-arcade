#include "Fighter.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"
#include "SpriteHolder.h"

Fighter::Fighter(string Name, Point position) {
	name = Name;
	tickAnimator = new TickTimerAnimator(NULL);
	health = 1.0;
	sprite = new Sprite(position, AnimationFilmHolder::Get()->GetFilm(Name + ".stance"), SpriteTypes::FIGHTER, Fighter::name._Equal("subzero")?true:false);
	animator = new FrameRangeAnimator();
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

void Fighter::Draw(SDL_Surface& gScreenSurface, Point enemy,Rect& camera) {
	sprite->SetEnemy(enemy);
	int width = (SCREEN_WIDTH - sprite->getFilm()->GetFrameBox(sprite->GetFrameNo()).w * 2) / 6;
	int height = (SCREEN_WIDTH - sprite->getFilm()->GetFrameBox(sprite->GetFrameNo()).h * 2) / 3;
	using Input = logic::StateTransitions::Input;
	sprite->DisplayCamera(gScreenSurface, width, height,camera);

	Input tmpInput;
	tmpInput.insert(Make_key(inputController.GetLogical()));
	Fighter::stateTransitions.PerformTransitions(tmpInput, false);//Investigate this flag how works
};

/*
* Watchout in which time you want to make the transition before or after the act like 1st print and then change state ?
or the opposite
*/
void Fighter::setStateMachine() {
	using Input = logic::StateTransitions::Input;
	stateTransitions.
		/*
		*  PUNCHES-> LOW/HIGH AND PUNCH
		*/
		SetTransition("READY", Input{ ".PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {//This might is not needed ill check it at combos
			AnimatorHolder::Remove(tickAnimator);
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(rand() % 2 + 1 == 2 ? AnimationFilmHolder::Get()->GetFilm(name + ".punch1") : AnimationFilmHolder::Get()->GetFilm(name + ".punch2"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 100, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
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
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 100, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
			}
		});
	})
		.SetTransition("DOWN", Input{ ".BCK.DOWN.SPECIAL" }, [&](void) {
		SetActionWithAnimator([&]() {//HINT: to hit it u must hold down and hit simultaneously the back and special key
			AnimatorHolder::Remove(tickAnimator);
			cout << "SPECIAL MOVE 1" << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("READY", Input{ ".DOWN.KICK.SPECIAL" }, [&](void) {
		SetActionWithAnimator([&]() {//HINT: Hit them all at once
			AnimatorHolder::Remove(tickAnimator);
			cout << "SPECIAL MOVE 2" << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("DOWN");
		});
	})

		.SetTransition("UP", Input{ ".PUNCH" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".highpunch"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 60, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
		}
	})
		/*
		* KICKS-> LOW/HIGH AND KICK
		*/
		.SetTransition("READY", Input{ ".KICK" }, [&](void) {
		SetActionWithAnimator([&]() {//This might is not needed ill check it at combos
			AnimatorHolder::Remove(tickAnimator);
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(rand() % 2 + 1 == 2 ? AnimationFilmHolder::Get()->GetFilm(name + ".kick1") : AnimationFilmHolder::Get()->GetFilm(name + ".kick2"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 100, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
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
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 100, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
			}
		});
	})
		.SetTransition("UP", Input{ ".KICK" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".highkick1"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 100, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("UP");
		}
	})
		/*
		* MOVES-> UP/BACK/FORWARD/DOWN
		*/
		.SetTransition("READY", Input{ ".UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".up"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, -120, 180, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
				stateTransitions.SetState("UP");
			}
		});
	})
		.SetTransition("READY", Input{ ".BCK" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(tickAnimator);
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".move"));

			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -17 : 19, 0, 50, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		.SetTransition("READY", Input{ ".FWD" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".move"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? 19 : -17, 0, 50, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
			}
		});
	})
		.SetTransition("READY", Input{ ".DOWN" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".down"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 180, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("DOWN");
		}
	})
		.SetTransition("DOWN", Input{ ".DOWN" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".duck"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 180, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		/*
		* BLOCKS->BLOCK/BLOCK DOWN
		*/
		.SetTransition("READY", Input{ ".BLOCK" }, [&](void) {
		SetActionWithAnimator([&]() {//This might is not needed ill check it at combos
			AnimatorHolder::Remove(tickAnimator);
			if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
				AnimatorHolder::Remove(animator);
				animator = new FrameRangeAnimator();
				sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".block"));
				animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
					new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 100, false, 150),
					SDL_GetTicks());
				AnimatorHolder::MarkAsRunning(animator);
				stateTransitions.SetState("BLOCK");
			}
		});
	})
		.SetTransition("DOWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Block Down -> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("BLOCKDWN");
		});
	})
		.SetTransition("BLOCK", Input{ ".BLOCK" }, [&](void) {
		if (animator->HasFinished()) {//This might is not needed ill check it at combos
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".stillblock"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 180, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		.SetTransition("BLOCKDWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {//This might is not needed ill check it at combos
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".downblock"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, 180, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		/*
		* DEFAULT TRANSITIONS
		*/
		.SetTransition("DOWN", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Getting Up-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("UP", Input{}, [&](void) {
		if (tickAnimator&&tickAnimator->GetState() != ANIMATOR_RUNNING) {
			TickTimerAnimation* tmp2 = new TickTimerAnimation(10);
			tmp2->setOnTick([&] {
				if (sprite->GetPosition().y + 120 < 500) sprite->Move({ 0,120 });
			}).SetDelay(400).SetReps(3);
			tickAnimator = new TickTimerAnimator(tmp2);
			tickAnimator->SetOnFinish([&]() {
				sprite->SetY(500);//Error Correction
				AnimatorHolder::Remove(tickAnimator);
				stateTransitions.SetState("READY");
			});
			tickAnimator->Start(SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(tickAnimator);
		}
	})
		.SetTransition("BLOCK", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Unblocking-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("BLOCKDWN", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Unblocking DOWN-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("DOWN");
		});
	})
		.SetTransition("READY", Input{}, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".stance"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), 0, 0, Fighter::name._Equal("subzero") ? 75 : 100, false, 666),
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
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipdown"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? 30 : -30, 0, 70, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		.SetTransition("DOWN", Input{ ".BCK.DOWN" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".down")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipdown"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -25 : 25, 0, 70, false, 666),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		.SetTransition("UP", Input{ ".FWD" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipup"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? 30 : -30, 0, 70, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("FlipFWD");
		}
	})
		.SetTransition("UP", Input{ ".BCK" }, [&](void) {
		if (animator->HasFinished()) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".flipup"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -30 : 30, 0, 70, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
			stateTransitions.SetState("FlipBCK");
		}
	})
		.SetTransition("FlipFWD", Input{ ".FWD.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Falling from FlipFWD-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("FlipFWD", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Falling from flipFWD-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("FlipBCK", Input{ ".FWD.BCK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Falling from FlipBCK-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("FlipBCK", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Falling from FlipBCK-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");
		});
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