#include "game_channels.h"
#include "channels.h"
 
/* Update objects to create channels for */
#include "unit_update.h"
#include "player_update.h"
#include "slot_update.h"
#include "planet_update.h"
#include "city_update.h"

void initializeChannels() {
	using channels = SunNet::Channels;

	channels::addNewChannel<UnitUpdate>();
	channels::addNewChannel<PlayerUpdate>();
	channels::addNewChannel<SlotUpdate>();
	channels::addNewChannel<PlanetUpdate>();
	channels::addNewChannel<CityUpdate>();
}