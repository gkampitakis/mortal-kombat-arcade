#include "Fighter.h"
#include "AnimationFilmHolder.h"

Fighter::Fighter(string Name) {
	name = Name;
};

bool Fighter::initialize(const string& path) {
	try {
		json config;
		std::ifstream config_file(path, std::ifstream::binary);
		config_file >> config;
		//cout << config;
		input::key_combination keycodes;
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
	Fighter::stateTransitions.PerformTransitions(inputController.GetLogical(), false);//Investigate this flag how works
}

void Fighter::setStateMachine() {
	using Input = logic::StateTransitions::Input;
	stateTransitions.
		SetTransition("READY", Input{ "HPUNCH" }, [](void) {
		cout << "HPUNC\n";
	})
		.
		SetTransition("READY", Input{ "UP" }, [](void) {
		cout << "UP\n";
	})
		.SetTransition("READY", Input{ "BCK" }, [](void) {
		cout << "BACK\n";
	})
		.SetTransition("READY", Input{ "FWD" }, [](void) {
		cout << "FWD\n";
	})
		.SetTransition("READY", Input{ "DOWN" }, [&](void) {
		cout << "DUCKING\n";
		stateTransitions.SetState("DOWN");
	})
		.SetTransition("READY", Input{ "DOWN.FWD.LPUNCH" }, [](void) {

	})
		.SetTransition("READY", Input{}, [](void) { std::cout << "waiting"; })

		.SetTransition("DOWN", Input{ "DOWN.BLOCK" }, [&](void) {


	})
		.SetTransition("DOWN", Input{ "DOWN.HPUNCH" }, [](void) {

	})
		.SetTransition("DOWN", Input{}, [&](void) {
		cout << "Getting Up\n";
		stateTransitions.SetState("READY");
	});
}

void Fighter::Handler() {
	inputController.Handle();
};