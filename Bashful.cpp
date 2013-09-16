#include "Bashful.h"
#include "World.h"

Bashful::Bashful(World* world)  : Ghost(world)
{
	CL_Sizef fieldSize = world->getLevel()->getFieldSize();
	//Startposition ist die rechte untere Ecke
	position = CL_Pointf(26 * fieldSize.width + fieldSize.width / 2, 29 * fieldSize.height + fieldSize.height / 2);
	currentField = getIndices(position);

	setBody("Bashful");

	onFieldCenter();
}


Bashful::~Bashful(void)
{
}

void Bashful::onFieldCenter()
{
	CL_Point target = CL_Point();

	if(pursuitMode)
	{
		//Position von Shadow
		CL_Point shadowField = world->getShadow()->getCurrentField();
		//Position von Pacman
		CL_Point pacManField = world->getPacMan()->getCurrentField();
		//Bewegungsrichtung von Pacman
		int pacManMoveDirection = world->getPacMan()->getMoveDirection();

		//Zwei Felder in Pacmans Richtung weiter zählen
		switch(pacManMoveDirection)
		{
			case 0:
				pacManField.x -= 2;
				break;
			case 1:
				pacManField.y += 2;
				break;
			case 2:
				pacManField.x += 2;
				break;
			case 3:
				pacManField.y -= 2;
				break;
			default:
				break;
		}

		//Vektor zwischen Shadow und der zukünftigen Pacman Position berechnen,
		//diesen verdoppeln und das resultierende Feld als Ziel verwenden
		target.x = shadowField.x + (pacManField.x - shadowField.x) * 2;
		target.y = shadowField.y + (pacManField.y - shadowField.y) * 2;	
	}
	else //Im Patrol-Modus zur rechten, unteren Ecke gehen
		target = CL_Point(26, 29);

	goToTargetField(target);
}
