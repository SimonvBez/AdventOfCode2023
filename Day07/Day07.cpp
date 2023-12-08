#include <iostream>
#include <algorithm>
#include <array>
#include "common.hpp"


enum HandTypes {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};


class Hand {
private:
    std::array<uint8_t, 5> cards_part1;
    std::array<uint8_t, 5> cards_part2;
    int bet;
    HandTypes type_part1;
    HandTypes type_part2;

public:
    Hand(std::string& cards_str) {
        // Convert the cards to strength arrays, to make the comparison between hands easier
        for (int i = 0; i < 5; ++i) {
            uint8_t strength = card_to_strength_part1(cards_str[i]);
            cards_part1[i] = strength;
            cards_part2[i] = to_strength_part2(strength);
        }
        update_card_type(); // Set the hand types based on the cards
        std::from_chars(cards_str.c_str() + 6, cards_str.c_str() + cards_str.size(), bet); // Parse the bet amount
    }

    int get_bet() const {
        return bet;
    }

    static bool less_than_part1(Hand& left, Hand& right) {
        if (left.type_part1 != right.type_part1) {
            return left.type_part1 < right.type_part1;
        }
        return left.cards_part1 < right.cards_part1;
    }

    static bool less_than_part2(Hand& left, Hand& right) {
        if (left.type_part2 != right.type_part2) {
            return left.type_part2 < right.type_part2;
        }
        return left.cards_part2 < right.cards_part2;
    }

private:
    static uint8_t card_to_strength_part1(char card) {
        if (card <= '9') {
            return card - '1';
        }
        else {
            switch (card) {
            case 'T':
                return 9;
            case 'J':
                return 10;
            case 'Q':
                return 11;
            case 'K':
                return 12;
            case 'A':
                return 13;
            }
        }
    }

    static uint8_t to_strength_part2(uint8_t strength_part1) {
        if (strength_part1 == 10) {
            return 0;
        }
        return strength_part1;
    }

    void update_card_type() {
        // Create a histogram to count the occurences of each card
        std::array<uint8_t, 14> cards_counts{ 0 };
        for (uint8_t card : cards_part2) {
            cards_counts[card]++;
        }

        int joker_count = cards_counts[0];
        std::sort(cards_counts.begin(), cards_counts.end(), std::greater());
        type_part1 = get_hand_type(cards_counts.data());

        uint8_t* counter_begin = cards_counts.data();
        if (cards_counts[0] == joker_count) {
            // The joker is the most occuring card, so pretend that the second-most occuring card is the most occuring card
            counter_begin++;
        }
        *counter_begin += joker_count; // Turns the jokers into the other card that occurs the most
        type_part2 = get_hand_type(counter_begin);
    }

    static HandTypes get_hand_type(uint8_t* sorted_counter) {
        switch (sorted_counter[0]) {
        case 1:
            return HighCard;
        case 2:
            return sorted_counter[1] == 2 ? TwoPair : OnePair;
        case 3:
            return sorted_counter[1] == 2 ? FullHouse : ThreeOfAKind;
        case 4:
            return FourOfAKind;
        case 5:
            return FiveOfAKind;
        }
    }
};


int main() {
    std::ifstream file_in = open_input(7);

    std::vector<Hand> hands;
    std::string line;
    while (std::getline(file_in, line)) {
        hands.emplace_back(line);
    }

    std::sort(hands.begin(), hands.end(), Hand::less_than_part1);
    uint64_t total_winnings_part1 = 0;
    for (int i = 0; i < hands.size(); ++i) {
        total_winnings_part1 += hands[i].get_bet() * (i + 1);
    }

    std::sort(hands.begin(), hands.end(), Hand::less_than_part2);
    uint64_t total_winnings_part2 = 0;
    for (int i = 0; i < hands.size(); ++i) {
        total_winnings_part2 += hands[i].get_bet() * (i + 1);
    }

    std::cout << "Part 1: " << total_winnings_part1 << std::endl;
    std::cout << "Part 2: " << total_winnings_part2 << std::endl;
}
