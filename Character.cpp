#include "Character.h"
#include "World.h"

Character::Character(World *world) : GameObject(world), animationPos(0), animationSpeed(10.0f), moveDirection(1), newDirection(1)
{
	fieldSize = world->getLevel()->getFieldSize();
}

Character::~Character(void)
{
}

void Character::draw()
{
	body.draw(world->get_gc(), position.x - body.get_width() / 2, position.y - body.get_height() / 2);
}

//Figur weiter bewegen
void Character::move(float distance)
{
	switch (moveDirection)
	{
		case 0: position.y -= distance; break;
		case 1: position.x += distance; break;
		case 2: position.y += distance; break;
		case 3: position.x -= distance; break;
	}
}

bool Character::update(float elapsed)
{
	float distance = speed * elapsed;

	//Distanz zum nächsten Feld berechnen
	CL_Point nextField = getNextField(moveDirection);
	CL_Pointf nextFieldCoordinates = getCoordinates(nextField);
	float distanceToField = abs(position.x - nextFieldCoordinates.x) + abs(position.y - nextFieldCoordinates.y);

	//Ist die zurückzulegende Distanz größer als die Distanz zum nächsten Feldmittelpunkt
	//und ist das Feld frei, so muss eine eventuelle Richtungsänderung geprüft werden
	while (distance > distanceToField && world->getLevel()->isFreeField(nextField))
	{
		//Zum Feldmittelpunkt bewegen
		move(distanceToField);

		distance -= distanceToField;

		currentField = getIndices(nextFieldCoordinates);

		//wird für erbende Klassen aufgerufen, um Aktionen auf dem Feldmittelpunkt ausführen zu können
		onFieldCenter();

		//Ist eine Richtungsänderung möglich, dann Richtung ändern
		if(world->getLevel()->isFreeField(getNextField(newDirection)))
			moveDirection = newDirection;

		//Distanz zum nächsten Feld berechnen
		nextField = getNextField(moveDirection);
		nextFieldCoordinates = getCoordinates(nextField);
		distanceToField = abs(position.x - nextFieldCoordinates.x) + abs(position.y - nextFieldCoordinates.y);
	}

	//Spielfigur weiterbewegen bzw. Richtung ändern
	if(world->getLevel()->isFreeField(nextField))
		move(distance);
	else if(world->getLevel()->isFreeField(getNextField(newDirection)))
		moveDirection = newDirection;

	animationPos += elapsed * animationSpeed;

	while (animationPos >= animationLength)
		animationPos -= animationLength;

	body.set_frame(getAnimationPos());

	return true;
}

//Textur und Animation der Figur initialisieren
void Character::setBody(CL_StringRef name, int animationGroups)
{
	body = CL_Sprite(world->get_gc(), name, &world->resources);
	animationLength = body.get_frame_count() / animationGroups;
}

int Character::getAnimationPos() const
{
	return (int)animationPos;
}

CL_Point Character::getCurrentField() const
{
	return currentField;
}

int Character::getMoveDirection() const
{
	return moveDirection;
}

//Bewegungsrichtung ändern
void Character::changeDirection(int direction)
{
	newDirection = direction;

	if ((moveDirection + direction) % 2 == 0)
		moveDirection = direction;
}

//Indexkoordinaten des nächsten Feldes holen
CL_Point Character::getNextField(int moveDirection) const
{
	CL_Point nextField = currentField;

	switch (moveDirection)
	{
		case 0: nextField.y--; break;
		case 1: nextField.x++; break;
		case 2: nextField.y++; break;
		case 3: nextField.x--; break;
	}

	return nextField;
}

//Pixelkoordinaten des Feldes holen
CL_Pointf Character::getCoordinates(const CL_Point& field) const
{
	CL_Sizef fieldSize = world->getLevel()->getFieldSize();
	return CL_Pointf(field.x * fieldSize.width + fieldSize.width / 2, field.y * fieldSize.height + fieldSize.height / 2);
}

//Indexkoordinaten des Feldes holen
CL_Point Character::getIndices(const CL_Pointf& field) const
{
	CL_Sizef fieldSize = world->getLevel()->getFieldSize();
	return CL_Point((int)(field.x / fieldSize.width), (int)(field.y / fieldSize.height));
}
