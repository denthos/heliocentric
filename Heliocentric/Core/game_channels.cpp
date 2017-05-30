#include "game_channels.h"
#include "channels.h"
 
/* Update objects to create channels for */
#include "unit_creation_update.h"
#include "unit_update.h"
#include "player_update.h"
#include "planet_update.h"
#include "city_update.h"
#include "city_creation_update.h"

#include "player_id_confirmation.h"
#include "player_client_to_server_xfer.h"
#include "new_player_info_update.h"
#include "debug_pause.h"
#include "player_command.h"
#include "unit_command.h"
#include "trade_command.h"
#include "settle_city_command.h"
#include "unit_spawner_update.h"

#include "trade_deal.h"
#include "slot_update.h"

void initializeChannels() {
	using channels = SunNet::Channels;

	channels::addNewChannel<UnitCreationUpdate>();
	channels::addNewChannel<UnitUpdate>();
	channels::addNewChannel<PlayerUpdate>();
	channels::addNewChannel<PlanetUpdate>();
	channels::addNewChannel<CityUpdate>();
	channels::addNewChannel<CityCreationUpdate>();

	channels::addNewChannel<PlayerIDConfirmation>();
	channels::addNewChannel<PlayerClientToServerTransfer>();
	channels::addNewChannel<NewPlayerInfoUpdate>();
	channels::addNewChannel<DebugPause>();
	channels::addNewChannel<PlayerCommand>();
	channels::addNewChannel<UnitCommand>();
	channels::addNewChannel<TradeCommand>();
	channels::addNewChannel<SettleCityCommand>();
	channels::addNewChannel<SlotUpdate>();

	channels::addNewChannel<TradeData>();
	channels::addNewChannel<UnitSpawnerUpdate>();
}
