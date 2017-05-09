#pragma once

#include "logging.h"

#include "identifiable.h"
#include "resources.h"

/**
This class deals with interplayer trading. Each instance of this class is a
trade deal, which contains the two players' UID, the type of item that are
being traded and the respective amount being traded.

Let's just support one-time resoruce trading for now. We need some way to
handle generic trading item types later on, and even support long term trade
deals, in which case we need to keep track of a list of active trade deals in
player class.

To accept a trade deal, call accept().
To decline a trade deal, send this obejct directly back to server from client.
To change the deal, call modify().
*/

class TradeDeal {
public:
	bool accepted; // Indicates if this trade deal has been accepted.
	bool modified; // Indicates if this trade deal has been modified.
	UID recipient; // UID of the recipient of this trade deal. UID must correspond to a Player object.
	Resources::ResourceType selling; // Type of item this player is offering.
	int sell_amount; // Amount of item this player is offering. Don't see any point of this being float, so maybe just make it an int.
	Resources::ResourceType buying;
	int buy_amount;

	/* Constructor for a "giveaway" deal */
	TradeDeal(UID, Resources::ResourceType, int);

	/* To be used to apply its affects when accepted */
	void apply();
	void modify();
	void accept();
	bool isAccepted();
};