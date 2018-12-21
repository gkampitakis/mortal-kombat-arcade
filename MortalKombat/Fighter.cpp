#include "Fighter.h"
#include "AnimationFilmHolder.h"
#include "AnimatorHolder.h"

Fighter::Fighter(string Name) {
	name = Name;
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

void Fighter::Draw(SDL_Surface& gScreenSurface, string test, Rect t) {

	/*
	* TEMP CODE HERE
	*/
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm(test);
	//{0 , 0 } coordinates

	//tmp->DisplayFrame(gScreenSurface, { 500,500 }, 4, 200, 450);//100 x100 is the size of the player
	tmp->DisplayFrame(gScreenSurface, { t.x,t.y }, 4, t.w, t.h);//100 x100 is the size of the player
	using Input = logic::StateTransitions::Input;
	//cout<<inputController.GetLogical().size();
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
	Input test5;
	test5.insert(Make_key(inputController.GetLogical()));

	Fighter::stateTransitions.PerformTransitions(test5, false);//Investigate this flag how works
}

void Fighter::setStateMachine() {
	using Input = logic::StateTransitions::Input;
	stateTransitions.
		SetTransition("READY", Input{ ".PUNCH" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Punch\n";
		});
	})
		.SetTransition("READY", Input{ ".KICK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Kick\n";
		});
	})
		.SetTransition("READY", Input{ ".UP" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Up\n";
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
		.SetTransition("READY", Input{ ".BLOCK" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Block\n";
		});
	})
		.SetTransition("READY", Input{ ".DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "DUCKING\n";
			stateTransitions.SetState("DOWN");
		});
	})
		.SetTransition("READY", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Ready waiting\n";
		});
	})//TEST UP TO HERE <______________________
		.SetTransition("DOWN", Input{ ".PUNCH" }, [&](void) {

		cout << "Low Punch1\n";

	})
		.SetTransition("DOWN", Input{ ".PUNCH.DOWN" }, [&](void) {

		cout << "Low Punch4\n";

	})
		.SetTransition("DOWN", Input{ ".DOWN.PUNCH" }, [&](void) {


		cout << "Low Punch5\n";//<----------

	})
		.SetTransition("READY", Input{ ".PUNCH.DOWN" }, [](void) {
		cout << "Low punch2\n";
	})
		.SetTransition("READY", Input{ ".DOWN.PUNCH" }, [](void) {
		cout << "Low punch3\n";//<----------
	})


		//TESTING__________________________________
		.SetTransition("DOWN", Input{ ".DOWN.BLOCK" }, [&](void) {
		cout << "down block\n";// this doesnt work
	}).SetTransition("DOWN", Input{ ".BLOCK.DOWN" }, [&](void) {
		cout << "block down\n";
	})

		.SetTransition("DOWN", Input{ ".DOWN" }, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "still ducking\n";
		});
	})
		.SetTransition("DOWN", Input{}, [&](void) {
		SetActionWithAnimator([&]() {
			AnimatorHolder::Remove(tickAnimator);
			cout << "Getting Up\n";
			stateTransitions.SetState("READY");
		});
	});
}

void Fighter::Handler() {
	inputController.Handle();
};


void Fighter::SetActionWithAnimator(const std::function<void()>& f) {
	if (tickAnimator&&tickAnimator->GetState() != ANIMATOR_RUNNING) {
		TickTimerAnimation* tmp2 = new TickTimerAnimation(10);
		tmp2->setOnTick([] {
			//Nothing to do here
		}).SetDelay(1000).SetReps(1);//<-----------------FOR DEBUGGING
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

