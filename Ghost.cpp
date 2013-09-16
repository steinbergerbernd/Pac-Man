#include "Ghost.h"
#include "World.h"

Ghost::Ghost(World* world) : Character(world)
{
	speed = 100;
	setEyes("Eyes");
	pursuitMode = true;
}

Ghost::~Ghost(void)
{
}

void Ghost::draw()
{
	Character::draw();

	eyes.draw(world->get_gc(), position.x - fieldSize.width / 2, position.y - fieldSize.height);
}

bool Ghost::update(float elapsed)
{
	return Character::update(elapsed);
}

void Ghost::setEyes(CL_StringRef name)
{
	eyes = CL_Sprite(world->get_gc(), name, &world->resources);
}

bool Ghost::isPursuitMode() const
{
	return pursuitMode;
}

//Zwischen Pursuit- und Patrol-Modus umschalten
void Ghost::changeMode()
{
	pursuitMode = !pursuitMode;
}

int Ghost::getAnimationPos() const
{
	return animationLength + Character::getAnimationPos();
}

//Zu bestimmtem Zielfeld bewegen
void Ghost::goToTargetField(CL_Point targetField)
{
	//x- und y-Achsen zu Zielfeld berechnen
	int distanceX = currentField.x - targetField.x;
	int distanceY = currentField.y - targetField.y;

	int xDirection, yDirection;

	int movePriorities[4];

	//Ist die x-Distanz größer 0, so muss in die linke Richtung gegangen werden,
	//ansonsten in die rechte
	if(distanceX > 0)
		xDirection = 3;
	else
		xDirection = 1;

	//Ist die y-Distanz größer 0, so muss nach oben gegangen werden,
	//ansonsten nach unten
	if(distanceY > 0)
		yDirection = 0;
	else
		yDirection = 2;

	//Immer den längeren Entfernungsvektor als erste
	//und den kürzeren als zweite Wahl merken
	if(abs(distanceX) > abs(distanceY))
	{
		movePriorities[0] = xDirection;
		movePriorities[1] = yDirection;
	}
	else
	{
		movePriorities[0] = yDirection;
		movePriorities[1] = xDirection;
	}

	//Die entgegen gesetzten Richtungen als dritte und vierte Alternative merken
	movePriorities[2] = (movePriorities[1] + 2) % 4;
	movePriorities[3] = (movePriorities[0] + 2) % 4;

	//Augen schauen immer in die Richtung der ersten Alternative
	eyes.set_frame(movePriorities[0]);

	bool pathOkay = false;
	int reverseDirection = (moveDirection + 2) % 4;

	//Alle vier Alternativen prüfen
	for(int i = 0; i < 4 && !pathOkay; i++)
	{
		//Nicht in die entgegen gesetzte Richtung laufen und der Weg darf nicht von einer Wand blockiert sein
		if(movePriorities[i] != reverseDirection && world->getLevel()->isFreeField(getNextField(movePriorities[i])))
		{
			pathOkay = true;
			changeDirection(movePriorities[i]);
		}
	}

	//Wurde keine Möglichkeit gefunden,
	//muss die entgegen gesetzte Richtung gewählt werden
	if(!pathOkay)
		changeDirection(reverseDirection);
}