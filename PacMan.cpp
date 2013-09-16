#include "PacMan.h"
#include "World.h"

PacMan::PacMan(World *world) : Character(world)
{
	setBody("PacMan", 4);
	speed = 150;

	CL_Point startPosition = world->getLevel()->getStartPosition();
	position = CL_Pointf(startPosition.x * fieldSize.width + fieldSize.width / 2, startPosition.y * fieldSize.height + fieldSize.height / 2);
	currentField = getIndices(position);

	aiMode = true;
}

PacMan::~PacMan(void)
{
}

int PacMan::getAnimationPos() const
{
	return moveDirection * animationLength + Character::getAnimationPos();
}

//Zwischen AI-Modus und manuellen Modus umschalten
void PacMan::changeMode()
{
	aiMode = !aiMode;
}

bool PacMan::isAIMode() const
{
	return aiMode;
}

//wird aufgerufen wenn sich die Figur auf dem Feldmittelpunkt befindet
void PacMan::onFieldCenter()
{
	//Punkt aufsammeln
	world->getLevel()->takeDotOnField(currentField);

	//die künstliche Intelligenz für Pacman
	if(aiMode)
	{
		Ghost* ghosts[4];

		ghosts[0] = world->getBashful();
		ghosts[1] = world->getShadow();
		ghosts[2] = world->getSpeedy();
		ghosts[3] = world->getPokey();

		int differenceX = -1, differenceY = -1;

		//Die Entfernung zu den Geistern berechnen
		for(int i = 0; i < 4; i++)
		{
			CL_Point ghostField = ghosts[i]->getCurrentField();
			
			//Die Entfernung in x- und y-Richtung holen
			int tmpDifferenceX = currentField.x - ghostField.x;
			int tmpDifferenceY = currentField.y - ghostField.y;

			//Die zu Pacman nähesten Geistdaten merken (jeweils für x- und y-Entfernung)
			if(differenceX == -1 || abs(tmpDifferenceX) < abs(differenceX))
				differenceX = tmpDifferenceX;
			if(differenceY == -1 || abs(tmpDifferenceY) < abs(differenceY))
				differenceY = tmpDifferenceY;
		}

		int xDirection, yDirection;
		int movePriorities[4];

		//Wenn der näheste Geist auf der x-Achse links von Pacman ist,
		//dann die rechte Richtung wählen, ansonsten die linke
		if(differenceX > 0)
			xDirection = 1;
		else
			xDirection = 3;

		//Wenn der näheste Geist auf der y-Achse oberhalb von Pacman ist,
		//dann die Richtung nach unten wählen, ansonsten nach oben
		if(differenceY > 0)
			yDirection = 2;
		else
			yDirection = 0;

		//Wenn der Geist auf der y-Achse näher ist,
		//dann ist der erste Fluchtweg die x- und dann die y-Achse,
		//ansonsten umgekehrt
		if(abs(differenceX) > abs(differenceY))
		{
			movePriorities[0] = xDirection;
			movePriorities[1] = yDirection;
		}
		else
		{
			movePriorities[0] = yDirection;
			movePriorities[1] = xDirection;
		}

		//Die entgegengesetzen Fluchtrichtungen setzen
		movePriorities[2] = (movePriorities[0] + 2) % 4;
		movePriorities[3] = (movePriorities[1] + 2) % 4;

		bool pathOkay = false;
		int reverseDirection = (moveDirection + 2) % 4;

		//Alle vier Fluchtmöglichkeiten prüfen
		for(int i = 0; i < 4 && !pathOkay; i++)
		{
			//Das Feld muss frei sein und Pacman soll nicht umdrehen
			if(movePriorities[i] != reverseDirection && world->getLevel()->isFreeField(getNextField(movePriorities[i])))
			{
				pathOkay = true;

				int min, max;
				int minDistance = 4;
				//Es werden in die jeweilige Bewegungsrichtung 4 Felder
				//vor Pacman geprüft, ob sich vor ihm ein Geist befindet
				switch(movePriorities[i])
				{
					case 0:
						min = currentField.y - minDistance;
						max = currentField.y;
						break;
					case 1:
						min = currentField.x;
						max = currentField.x + minDistance;
						break;
					case 2:
						min = currentField.y;
						max = currentField.y + minDistance;
						break;
					case 3:
						min = currentField.x - minDistance;
						max = currentField.x;
						break;
					default:
						break;
				}

				//Alle Geister prüfen ob sie in Pacmans weg stehen,
				//wenn ja, dann ist der Pfad nicht zu wählen
				for(int j = 0; j < 4 && pathOkay; j++)
				{
					CL_Point ghostField = ghosts[j]->getCurrentField();
					switch(movePriorities[i])
					{
						case 0: case 2:
							pathOkay = !(ghostField.x == currentField.x && ghostField.y >= min && ghostField.y <= max);
							break;
						case 1: case 3:
							pathOkay = !(ghostField.y == currentField.y && ghostField.x >= min && ghostField.x <= max);
							break;
						default:
							break;
					}
				}

				//Hat der Pfad alle Prüfungen bestanden,
				//kann er eingeschlagen werden
				if(pathOkay)
					changeDirection(movePriorities[i]);
			}
		}

		//Sollten es keine anderen Möglichkeiten geben,
		//so muss Pacman wenden
		if(!pathOkay)
			changeDirection(reverseDirection);
	}
}