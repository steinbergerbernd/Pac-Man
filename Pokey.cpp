#include "Pokey.h"
#include "World.h"

Pokey::Pokey(World* world)  : Ghost(world)
{
	CL_Sizef fieldSize = world->getLevel()->getFieldSize();
	//Startposition ist die linke, untere Ecke
	position = CL_Pointf(1 * fieldSize.width + fieldSize.width / 2, 29 * fieldSize.height + fieldSize.height / 2);
	currentField = getIndices(position);

	setBody("Pokey");

	onFieldCenter();
}


Pokey::~Pokey(void)
{
}

void Pokey::onFieldCenter()
{
	CL_Point pacManField = world->getPacMan()->getCurrentField();

	//Ist die Entfernung größer als acht Felder und ist der Pursuit-Modus aktiv,
	//wird Pacman gejagt, ansonsten begibt sich derGeist zur linken, unteren Ecke
	if(pursuitMode && abs(currentField.x - pacManField.x) + abs(currentField.y - pacManField.y) > 8)
		goToTargetField(pacManField);
	else
		goToTargetField(CL_Point(1, 29));
}
