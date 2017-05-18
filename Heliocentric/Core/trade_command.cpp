#include "trade_command.h"

TradeCommand::TradeCommand(UID deal_id, bool accepted) : BaseCommand(deal_id, accepted ? CMD_TRADE_ACCEPT : CMD_TRADE_DECLINE) {

}