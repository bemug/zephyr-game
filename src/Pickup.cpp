#include "Pickup.hpp"

void Pickup::apply(Aircraft& player) const
{
	Table[mType].action(player);
}
