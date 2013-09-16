#include "GameObject.h"
#include "World.h"
#include "PacMan.h"
#include "Shadow.h"
#include "Speedy.h"
#include "Bashful.h"
#include "Pokey.h"
#include "Level.h"

#include <list>

#include <ClanLib/core.h>
#include <ClanLib/display.h>

World::World(CL_DisplayWindow &displayWindow) : window(displayWindow), quit(false)
{
	graphicContext = window.get_gc();
	
	//Initialisierung der Objekte
	resources = CL_ResourceManager("resources.xml");

	level = new Level(this, "Resources/Level.txt", window.get_viewport());
	pacMan = new PacMan(this);
	shadow = new Shadow(this);
	speedy = new Speedy(this);
	bashful = new Bashful(this);
	pokey = new Pokey(this);

	addObject(level);
	addObject(pacMan);
	addObject(shadow);
	addObject(speedy);
	addObject(bashful);
	addObject(pokey);
}

World::~World(void)
{
	// Löschen aller GameObjects
	std::list<GameObject*>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		delete (*it);
}

// Hinzufügen eines GameObject zur Collection
void World::addObject(GameObject *object)
{
	objects.push_back(object);
}

// Programmschleife
void World::run()
{
	durationPursuitMode = 30.0f;
	durationPatrolMode = 30.0f;
	currentDuration = 0.0f;

	// Input Ereignisse einhängen
	CL_Slot slotQuit = window.sig_window_close().connect(this, &World::onWindowClose);
	CL_Slot slotResize = window.sig_resize().connect(this, &World::onWindowResize);
	CL_Slot slotMouseDown = window.get_ic().get_mouse().sig_key_down().connect(this, &World::onMouseDown);
	CL_Slot slotMouseMove = window.get_ic().get_mouse().sig_pointer_move().connect(this, &World::onMouseMove);
	CL_Slot slotKeyDown = window.get_ic().get_keyboard().sig_key_down().connect(this, &World::onKeyDown);

	//Bei Escape Schleife abbrechen und Programm beenden
	while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
		if (quit)
			break;

		update();
		draw();

		window.flip(1);
		CL_KeepAlive::process();
	}
}

Level* World::getLevel()
{
	return level;
}


PacMan* World::getPacMan()
{
	return pacMan;
}

Shadow* World::getShadow()
{
	return shadow;
}

Speedy* World::getSpeedy()
{
	return speedy;
}

Bashful* World::getBashful()
{
	return bashful;
}

Pokey* World::getPokey()
{
	return pokey;
}

//Updates ausführen
void World::update()
{	

	//Vergangene Zeit berechnen
	float elapsed = getElapsedTime();
	currentDuration += elapsed;

	//Zwischen Pursuit-Modus und Patrol-Modus umschalten
	bool pursuitMode = shadow->isPursuitMode();
	if(pursuitMode && currentDuration > durationPursuitMode || !pursuitMode && currentDuration > durationPatrolMode)
	{
		shadow->changeMode();
		bashful->changeMode();
		speedy->changeMode();
		pokey->changeMode();
		currentDuration = 0.0f;
	}
	
	//Update aller GameObjects aufrufen
	std::list<GameObject *>::iterator it;
	for (it = objects.begin(); it != objects.end(); )
	{
		//Liefert ein Objekt "false" zurück, wird dieses gelöscht
		if (!(*it)->update(elapsed))
		{
			delete (*it);
			it = objects.erase(it);
		}
		else
			++it;
	}
}

// Zeichnen aller Komponenten
void World::draw()
{
	CL_Draw::fill(graphicContext, window.get_viewport(), CL_Colorf::black);

	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		(*it)->draw();
}

// Vergangene Zeit berechnen
float World::getElapsedTime()
{
	static unsigned int lastTime = 0;

	unsigned int newTime = CL_System::get_time();

	if (lastTime == 0)
		lastTime = newTime;

	int deltaTime = (newTime - lastTime);

	lastTime = newTime;

	return deltaTime * 0.001f;
}

void World::onWindowClose()
{
	quit = true;
}

void World::onWindowResize(int width, int height)
{
}

void World::onMouseDown(const CL_InputEvent &key, const CL_InputState &state)
{
}

void World::onMouseMove(const CL_InputEvent &key, const CL_InputState &state)
{
}

void World::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	//Tastatureingaben interpretieren
	if(pacMan->isAIMode())
	{
		switch (key.id)
		{
			case CL_KEY_A: pacMan->changeMode(); break;
			default: break;
		}
	}
	else
	{
		switch (key.id)
		{
			case CL_KEY_UP: pacMan->changeDirection(0); break;
			case CL_KEY_RIGHT: pacMan->changeDirection(1); break;
			case CL_KEY_DOWN: pacMan->changeDirection(2); break;
			case CL_KEY_LEFT: pacMan->changeDirection(3); break;
			case CL_KEY_A: pacMan->changeMode(); break;
			default: break;
		}
	}
}
