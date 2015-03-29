#include "Pickup.hpp"

#include "DataTables.hpp"

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}


void Pickup::apply(Aircraft& player) const
{
	Table[mType].action(player);
}
