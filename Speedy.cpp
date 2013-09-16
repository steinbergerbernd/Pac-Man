#include "Speedy.h"
#include "World.h"

Speedy::Speedy(World *world) : Ghost(world)
{
	CL_Sizef fieldSize = world->getLevel()->getFieldSize();
	//Startposition ist die linke, obere Ecke
	position = CL_Pointf(fieldSize.width + fieldSize.width / 2, fieldSize.height + fieldSize.height / 2);
	currentField = getIndices(position);

	setBody("Speedy");

	onFieldCenter();
}


Speedy::~Speedy(void)
{
}

void Speedy::onFieldCenter()
{
	//Pursuit-Modus
	if(pursuitMode)
	{
		//Pacman Position und Bewegungsrichtung holen
		CL_Point pacManField = world->getPacMan()->getCurrentField();
		int pacManMoveDirection = world->getPacMan()->getMoveDirection();

		//Vier Felder in die Richtung von Pacman addieren
		switch(pacManMoveDirection)
		{
			case 0:
				pacManField.x -= 4;
				break;
			case 1:
				pacManField.y += 4;
				break;
			case 2:
				pacManField.x += 4;
				break;
			case 3:
				pacManField.y -= 4;
				break;
			default:
				break;
		}

		goToTargetField(pacManField);
	}
	else //Im Patrol-Modus zur linken, oberen Ecke gehen
		goToTargetField(CL_Point(1,1));
}