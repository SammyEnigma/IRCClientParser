
#include "poker_21.h"

#include <stdexcept>

/*
std::string card_output(Card const& card)
{
    std::string output = "\x03\x31\x02|";

    switch (card.value)
    {
        case Card::Value::TWO:
        case Card::Value::THREE:
        case Card::Value::FOUR:
        case Card::Value::FIVE:
        case Card::Value::SIX:
        case Card::Value::SEVEN:
        case Card::Value::EIGHT:
        case Card::Value::NINE:
            output += static_cast<char>(card.value) + 48 + 1;
            break;
        case Card::Value::TEN:
            output += "10";
            break;
        case Card::Value::ACE:
            output += "A";
            break;
        case Card::Value::JACK:
            output += "J";
            break;
        case Card::Value::QUEEN:
            output += "Q";
            break;
        case Card::Value::KING:
            output += "K";
            break;
        default:
            break;
    };
    output += " \x03";

    switch (card.suit)
    {
        case Card::Suit::HEART:
            output += "\x03\x35\x02♥\x03";
            break;
        case Card::Suit::SPADE:
            output += "\x03\x31\x02♠\x03";
            break;
        case Card::Suit::CLUB:
            output += "\x03\x31\x02♣\x03";
            break;
        case Card::Suit::DIAMOND:
            output += "\x03\x35\x02♦\x03";
            break;
        default:
            break;
    }

    output += "\x03\x31\x02|\x03";

    return output;
}

int card_value(Card const& card)
{
    switch (card.value)
    {
        case Card::Value::TWO:
        case Card::Value::THREE:
        case Card::Value::FOUR:
        case Card::Value::FIVE:
        case Card::Value::SIX:
        case Card::Value::SEVEN:
        case Card::Value::EIGHT:
        case Card::Value::NINE:
        case Card::Value::TEN:
            return static_cast<char>(card.value) + 1;
        case Card::Value::JACK:
        case Card::Value::QUEEN:
        case Card::Value::KING:
            return 10;
        case Card::Value::ACE:
            return 11;
        default:
            break;
    }

    return 0;
}

std::string hand_total(Hand const& hand)
{
    auto total1 = card_value(hand.first);
    auto total2 = card_value(hand.second);
    if (total1 + total2 > 21)
    {
        if (total1 == 11)
            total1 = 1;
        else if (total2 == 11)
            total2 = 1;
    }

    return std::to_string(total1 + total2);
}
*/


namespace irc_parser
{

Poker21::Poker21()
{
}

bool Poker21::playing() const
{
    return started;
}

void Poker21::start_hand()
{
    deck.shuffle();
    dealers = {deck.next_card(), deck.next_card()};
    started = true;
}

void Poker21::stop()
{
    started = false;
}

Hand Poker21::deal_hand()
{
    //if (deck.size() < 2)
        //throw std::runtime_error("Outof cards!");

    return {deck.next_card(), deck.next_card()};
}

Hand Poker21::dealers_hand() const
{
    return dealers;
}


}
