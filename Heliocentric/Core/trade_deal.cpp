#include "trade_deal.h"

TradeDeal::TradeDeal(std::shared_ptr<TradeData> deal) : Identifiable() {
	this->sender = deal->sender;
	this->recipient = deal->recipient;
	this->sell_type = deal->sell_type;
	this->sell_amount = deal->sell_amount;
	this->buy_type = deal->buy_type;
	this->buy_amount = deal->buy_amount;
}

TradeDeal::TradeDeal(std::shared_ptr<TradeData> deal, UID id) : Identifiable(id) {
	this->sender = deal->sender;
	this->recipient = deal->recipient;
	this->sell_type = deal->sell_type;
	this->sell_amount = deal->sell_amount;
	this->buy_type = deal->buy_type;
	this->buy_amount = deal->buy_amount;
}

TradeDeal::TradeDeal(UID sender, UID recipient, Resources::Type sell_type, int sell_amount,
	Resources::Type buy_type, int buy_amount) : Identifiable(), sender(sender), recipient(recipient),
	sell_type(sell_type), sell_amount(sell_amount), buy_type(buy_type), buy_amount(buy_amount) {
	this->trade_status = PENDING;
}

TradeDeal::TradeDeal(UID id, UID sender, UID recipient, Resources::Type sell_type, int sell_amount,
	Resources::Type buy_type, int buy_amount) : Identifiable(id), sender(sender), recipient(recipient),
	sell_type(sell_type), sell_amount(sell_amount), buy_type(buy_type), buy_amount(buy_amount) {
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