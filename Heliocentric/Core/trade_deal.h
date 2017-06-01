#pragma once

#include "logging.h"
#include "trade_data.h"

/**
This class deals with interplayer trading. Each instance of this class ia a
trade deal, maintained by player classes on server side. Each player keeps
track of trade deals related to them, whether pending or active. This class
is a subclass of both TradeData class and Identifiable.

Let's just support one-time resoruce trading for now. We need some way to
handle generic trading item types later on, and even support long term trade
deals, in which case we need to keep track of a list of active trade deals in
player class.
*/

class TradeDeal : public Identifiable {
private:
	enum Status { PENDING, ACCEPTED, DECLINED };

	Status trade_status; // Indicates if this trade deal is accepted, declined or pending.

	UID sender; // UID of the player who sends this trade deal.
	UID recipient; // UID of the player who receives this trade deal.

	Resources::Type sell_type; // Type of item this player is offering.
	Resources::Type buy_type; // Type of item this player is asking for.
	int sell_amount; // Amount of item this player is offering.
	int buy_amount; // Amount of item this player is asking for.

public:
	TradeDeal(std::shared_ptr<TradeData>);
	TradeDeal(std::shared_ptr<TradeData>, UID);

	TradeDeal(UID, UID, Resources::Type, int, Resources::Type, int);
	TradeDeal(UID, UID, UID, Resources::Type, int, Resources::Type, int);

	void apply();
	void accept();
	bool isAccepted();
	bool isPending();
	UID get_sender();
	UID get_recipient();
	Resources::Type get_sell_type();
	Resources::Type get_buy_type();
	int get_sell_amount();
	int get_buy_amount();
};