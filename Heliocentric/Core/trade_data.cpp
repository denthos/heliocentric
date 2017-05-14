#include "trade_data.h"

TradeData::TradeData(UID sender, UID recipient, Resources::Type item, int amount) :
	trade_status(PENDING), sender(sender), recipient(recipient), sell_type(item), sell_amount(amount) {
	this->trade_deal_id = 0; // Initialize it to zero, but this is not the real ID of the corresponding trade deal.
}

void TradeData::apply() {

}