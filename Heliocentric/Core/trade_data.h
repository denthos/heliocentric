#pragma once

#include "identifiable.h"
#include "resources.h"

/**
This class is very similar to TradeDeal, but is only responsible for network
traffic, namely it's only used to be transmitted between client and server.
*/

class TradeData {
public:
	enum Status { PENDING, ACCEPTED, DECLINED };

	Status trade_status; // Indicates if this trade deal is accepted, declined or pending.

	UID trade_deal_id; // ID of the corresponding trade deal, not assigned until server handles this object.
	UID sender; // UID of the player who sends this trade deal.
	UID recipient; // UID of the player who receives this trade deal.

	Resources::Type sell_type; // Type of item this player is offering.
	Resources::Type buy_type; // Type of item this player is asking for.
	int sell_amount; // Amount of item this player is offering.
	int buy_amount; // Amount of item this player is asking for.

	/**
	Constructor for a "giveaway" deal.
	@param sender Player who send this deal.
	@param recipient Player who receives this deal.
	@param item Type of resource sender gives.
	@param amount Amount of resource sender gives.
	*/
	TradeData(UID, UID, Resources::Type, int);
	void apply();
};