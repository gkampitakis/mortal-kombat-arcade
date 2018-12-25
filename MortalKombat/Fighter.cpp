#include "Fighter.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"
#include "SpriteHolder.h"

Fighter::Fighter(string Name, Point position) {
	name = Name;
	tickAnimator = new TickTimerAnimator(NULL);
	health = 1.0;
	sprite = new Sprite(position, AnimationFilmHolder::Get()->GetFilm(Name + ".stance"), SpriteTypes::FIGHTER);
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

void Fighter::Draw(SDL_Surface& gScreenSurface, int w, int h) {

	using Input = logic::StateTransitions::Input;
	sprite->Display(gScreenSurface, w, h);

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
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Punch-> State " << stateTransitions.GetState() << "\n";
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN.PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Low Punch-> State " << stateTransitions.GetState() << "\n";
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

		.SetTransition("UP", Input{ ".PUNCH.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "High Punch-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");//do the animation and the fall down here cant stay at air forever
		});
	})
		/*
		* KICKS-> LOW/HIGH AND KICK
		*/
		.SetTransition("READY", Input{ ".KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Kick-> State " << stateTransitions.GetState() << "\n";
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN.KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Low KICK-> State " << stateTransitions.GetState() << "\n";
		});

	})
		.SetTransition("UP", Input{ ".KICK.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "High Kick-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("READY");//do the animation and the fall down here cant stay at air forever
		});
	})
		/*
		* MOVES-> UP/BACK/FORWARD/DOWN
		*/
		.SetTransition("READY", Input{ ".UP" }, [&](void) {
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
	})
		.SetTransition("READY", Input{ ".BCK" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(tickAnimator);
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".move"));

			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? -15 : 15, 0, 50, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
	})
		.SetTransition("READY", Input{ ".FWD" }, [&](void) {
		if (animator->HasFinished() || sprite->getFilm()->GetId()._Equal(name + ".stance")) {
			AnimatorHolder::Remove(animator);
			animator = new FrameRangeAnimator();
			sprite->SetNewFilm(AnimationFilmHolder::Get()->GetFilm(name + ".move"));
			animator->Start(sprite,//start from zero to end zero move x,y 75 speed and continous 
				new FrameRangeAnimation(0, sprite->getFilm()->GetTotalFrames(), Fighter::name._Equal("subzero") ? 15 : -15, 0, 50, false, 150),
				SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(animator);
		}
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
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Block -> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("BLOCK");
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
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "still blocking-> State " << stateTransitions.GetState() << "\n";
		});
	})
		.SetTransition("BLOCKDWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "still blocking down-> State " << stateTransitions.GetState() << "\n";
		});
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
				sprite->Move({ 0,120 });
			}).SetDelay(400).SetReps(2);
			tickAnimator = new TickTimerAnimator(tmp2);
			tickAnimator->SetOnFinish([&]() {
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
		.SetTransition("UP", Input{ ".FWD.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "FlipFWD-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("FlipFWD");
		});
	})
		.SetTransition("UP", Input{ ".BCK.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "FlipBCK-> State " << stateTransitions.GetState() << "\n";
			stateTransitions.SetState("FlipBCK");
		});

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

