#include "trade_deal.h"

TradeDeal::TradeDeal(UID recipient, Resources::ResourceType item, int amount) : recipient(recipient), selling(item), sell_amount(amount) {
	this->accepted = false;
	this->modified = false;
}

void TradeDeal::apply() {

}

void TradeDeal::accept() {
	LOG_DEBUG("Deal accepted.");

	this->accepted = true;
	apply();
}

void TradeDeal::modify() {
	// modify this trade deal
	this->modified = true;
}

bool TradeDeal::isAccepted() {
	return this->accepted;
}