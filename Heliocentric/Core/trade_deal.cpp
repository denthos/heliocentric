#include "trade_deal.h"

TradeDeal::TradeDeal(std::shared_ptr<TradeData> deal) : Identifiable() {
	this->sender = deal->sender;
	this->recipient = deal->recipient;
	this->sell_type = deal->sell_type;
	this->sell_amount = deal->sell_amount;
}

TradeDeal::TradeDeal(std::shared_ptr<TradeData> deal, UID id) : Identifiable(id) {
	this->sender = deal->sender;
	this->recipient = deal->recipient;
	this->sell_type = deal->sell_type;
	this->sell_amount = deal->sell_amount;
}

TradeDeal::TradeDeal(UID sender, UID recipient, Resources::Type item, int amount) :
	Identifiable(), sender(sender), recipient(recipient), sell_type(item), sell_amount(amount)  {
	this->trade_status = PENDING;
}

TradeDeal::TradeDeal(UID id, UID sender, UID recipient, Resources::Type item, int amount) :
	Identifiable(id), sender(sender), recipient(recipient), sell_type(item), sell_amount(amount) {
	this->trade_status = PENDING;
}

void TradeDeal::apply() {

}

void TradeDeal::accept() {
	LOG_DEBUG("Deal accepted.");
	this->trade_status = ACCEPTED;
}

bool TradeDeal::isAccepted() {
	return this->trade_status == ACCEPTED;
}

bool TradeDeal::isPending() {
	return this->trade_status == PENDING;
}

UID TradeDeal::get_sender() {
	return this->sender;
}

UID TradeDeal::get_recipient() {
	return this->recipient;
}

Resources::Type TradeDeal::get_sell_type() {
	return this->sell_type;
}

Resources::Type TradeDeal::get_buy_type() {
	return this->buy_type;
}

int TradeDeal::get_sell_amount() {
	return this->sell_amount;
}

int TradeDeal::get_buy_amount() {
	return this->buy_amount;
}