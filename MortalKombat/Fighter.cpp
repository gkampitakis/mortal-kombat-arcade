#include "Fighter.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"

Fighter::Fighter(string Name, Point position) {
	name = Name;
	tickAnimator = new TickTimerAnimator(NULL);
	FighterPos = position;
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

void Fighter::Draw(SDL_Surface& gScreenSurface, string test, int w, int h) {

	/*
	* TEMP CODE HERE
	*/
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm(test);
	//{0 , 0 } coordinates

	tmp->DisplayFrame(gScreenSurface, FighterPos, 4, w, h);//100 x100 is the size of the player
	using Input = logic::StateTransitions::Input;
	/*
	if (tickAnimator&&tickAnimator->GetState() != ANIMATOR_RUNNING) {
			TickTimerAnimation* tmp2 = new TickTimerAnimation(10);
			tmp2->setOnTick([] {
				//Nothing to do here
			}).SetDelay(1000).SetReps(1);
			tickAnimator = new TickTimerAnimator(tmp2);
			tickAnimator->SetOnFinish([&]() {
				AnimatorHolder::Remove(tickAnimator);
				inputController.GetLogical();
			});
				tickAnimator->Start(SDL_GetTicks());
			AnimatorHolder::MarkAsRunning(tickAnimator);
		}
	*/

	Input tmpInput;
	tmpInput.insert(Make_key(inputController.GetLogical()));
	Fighter::stateTransitions.PerformTransitions(tmpInput, false);//Investigate this flag how works
}
/*
* All the animations with V2 print might not be necessary cause start from ready state and go to another state->action
we ll see whats suitable for us
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
			cout << "Punch\n";
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN.PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Low Punch\n";
		});

	})
		.SetTransition("READY", Input{ ".DOWN.PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Low PunchV2\n";
		});//This might not needed with the Ready stata cause there is no transition from Ready to down and the opposite
	})
		.SetTransition("UP", Input{ ".PUNCH.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "High Punch\n";
			//stateTransitions.SetState("READY");//do the animation and the fall down
		});

	})
		.SetTransition("READY", Input{ ".PUNCH.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "High PunchV2\n";
			//stateTransitions.SetState("READY");//do the animation and the fall down
		});
	})
		/*
		* KICKS-> LOW/HIGH AND KICK
		*/
		.SetTransition("READY", Input{ ".KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Kick\n";
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN.KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Low KICK\n";
		});

	})
		.SetTransition("READY", Input{ ".DOWN.KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Low KICKV2\n";
		});//This might not needed with the Ready stata cause there is no transition from Ready to down and the opposite
	})
		.SetTransition("UP", Input{ ".KICK.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "High Kick\n";
			stateTransitions.SetState("READY");//do the animation and the fall down
		});

	})
		.SetTransition("READY", Input{ ".KICK.UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "High KickV2\n";
			stateTransitions.SetState("READY");//do the animation and the fall down
		});
	})
		/*
		* MOVES-> UP/BACK/FORWARD/DOWN
		*/
		.SetTransition("READY", Input{ ".UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Up\n";
			stateTransitions.SetState("UP");
		});
	})
		.SetTransition("READY", Input{ ".BCK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Back\n";
		});
	})
		.SetTransition("READY", Input{ ".FWD" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Forward\n";
		});
	})
		.SetTransition("READY", Input{ ".DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "DUCKING\n";
			stateTransitions.SetState("DOWN");
		});
	})
		.SetTransition("DOWN", Input{ ".DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "still ducking\n";
		});
	})
		/*.SetTransition("UP", Input{ ".UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "still UP\n";//This is for debug this transition means the character can stay at air
		});
	})*/
	/*
	* BLOCKS->BLOCK/BLOCK DOWN
	*/
		.SetTransition("READY", Input{ ".BLOCK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Block\n";
		});
	})
		.SetTransition("READY", Input{ ".BLOCK.DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Block DownV2\n";
		});
	})
		.SetTransition("DOWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Block Down\n";
		});
	})
		/*
		* DEFAULT TRANSITIONS
		*/
		.SetTransition("DOWN", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Getting Up\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("UP", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Falling Down\n";
			stateTransitions.SetState("READY");
		});
	})
		.SetTransition("READY", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Ready waiting\n";
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

