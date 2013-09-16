#include "Shadow.h"
#include "World.h"

Shadow::Shadow(World *world) : Ghost(world)
{
	CL_Sizef fieldSize = world->getLevel()->getFieldSize();
	//Startposition ist die rechte, obere Ecke
	position = CL_Pointf(26 * fieldSize.width + fieldSize.width / 2, 1 * fieldSize.height + fieldSize.height / 2);
	currentField = getIndices(position);

	setBody("Shadow");

	onFieldCenter();
}

Shadow::~Shadow(void)
{
}

void Shadow::onFieldCenter()
{
	//Im Pursuit-Modus Pacman jagen und ansonsten zur rechten, oberen Ecke gehen
	if(pursuitMode)
		goToTargetField(world->getPacMan()->getCurrentField());
	else
		goToTargetField(CL_Point(26, 1));
}