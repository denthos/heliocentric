#include "trade_data.h"

TradeData::TradeData(UID sender, UID recipient, Resources::Type sell_type, int sell_amount,
	Resources::Type buy_type, int buy_amount) : trade_status(PENDING), sender(sender), recipient(recipient),
	sell_type(sell_type), sell_amount(sell_amount), buy_type(buy_type), buy_amount(buy_amount) {
	/* Initialize ID to invalid, but this is not the real ID of the corresponding trade deal. */
	this->trade_deal_id = INVALID_ID;
}

void TradeData::apply() {

}