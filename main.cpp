#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>

#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_RESET "\x1b[0m"


using namespace std;

class DeckOfCard {
private:
    vector<string> deck;

    void initialize_deck() {
		string numbers[] = { "A", "2", "3", "4", "5","6", "7", "8", "9", "10" };
		char faces[] = { 'J', 'Q', 'K' };
		string joker[] = { "Joker" };

        for (const auto& number : numbers) {
            deck.push_back(number);
        }
        for (const auto& face : faces) {
            deck.push_back(string(1, face));
        }
        deck.push_back(joker[0]);

        shuffle_deck();
    }

public:
    int money = 0;

    DeckOfCard() {
        initialize_deck();
    }

    void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

    void press_return() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nPress 'enter' to return.";
        cin.get();
        clear_screen();
        
    }
    void shuffle_deck() {
        random_device rd;
        mt19937 g(rd());
        shuffle(deck.begin(), deck.end(), g);
    }

    string draw_card() {
        if (deck.empty()) {
            return "";
        }
        string card = deck.back();
        deck.pop_back();
        return card;
    }

    void draw_card(vector<string>& hand) {
        string card = draw_card();
        hand.push_back(card);
    }

    int card_value(const string& card) {
        if (card == "A") return 1;
        if (card == "J") return 11;
        if (card == "Q") return 12;
        if (card == "K") return 13;
        if (card == "Joker") return -1;
        return stoi(card);
    }

    void print_hand(const vector<string>& hand) {
        for (size_t i = 0; i < hand.size(); ++i) {
		    cout << "[" << hand[i] << "]";
    		if (i < hand.size() - 1) {
	    		cout << " ";
		    }
    	}
    	cout << endl;

    }

    int hand_value(const vector<string>& hand) {
	    int value = 0;

	    for (const auto& card : hand) {
		    int c_value = card_value(card);
    		if (c_value == -1) return -1;
		    value += c_value;
	    }
    	return value;
    }

    void blackjack_menu() {
	    int choice;

	    while (true) {
		    cout << "+----------------------------------+" << endl;
            cout << "|        Welcome to BlackJack!     |" << endl;
	    	cout << "+----------------------------------+" << endl;
    		cout << "| Select an option:                |" << endl;
		    cout << "| [1] Start Game                   |" << endl;
	    	cout << "| [2] Instructions                 |" << endl;
    		cout << "| [3] Wallet                       |" << endl;
            cout << "+----------------------------------+" << endl;
		    cout << " Enter your choice: ";

		    if (!(cin >> choice)) {
			    cin.clear();
    			cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    		clear_screen();
		    	cout << "Please enter a valid choice from the menu.\n" << endl;
			    continue;
		    }

    		switch (choice) {
	    	case 1:
		    	clear_screen();
			    while (true) {
				    int bet_amount = get_bet_amount();
                    if (bet_amount == 0) {
                        break;
                    }
				    else{
					    int bet_result = verify_bet(bet_amount);
                        if (bet_result == 0){
                            break;
                        }
					    else if (bet_result == 1) {
						    int bet_amount = get_bet_amount();
                            if (bet_amount == 0) {
                                break;
                            }
					    }
                        else if (bet_result == 2){
                            play_game(bet_amount);
						    break;
                        }
				    }
		    	}
			    break;
    		case 2:
	    		clear_screen();
		    	instructions();
			    break;
    		case 3:
                clear_screen();
		    	wallet();
			    break;
    		default:
                clear_screen();
		    	cout << "Please enter a valid choice from the menu.\n" << endl;
                break;
    		}
	    }
    }

    int get_bet_amount() {
        int bet_amount;

        while (true) {
            cout << "Enter amount you want to bet (enter 0 to cancel): ";

            if (cin >> bet_amount) {
                if (bet_amount == 0) {
                    clear_screen();
                    return 0;
                }
                else if (bet_amount < 0) {
                    clear_screen();
                    cout << "Please enter a positive integer.\n" << endl;
                }
                else if (bet_amount > money) {
                    clear_screen();
                    cout << "Insufficient balance. You only have $" << money << " in your wallet.\n" << endl;
                }
                else {
                    clear_screen();
                    return bet_amount;
                }
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clear_screen();
                cout << "Please enter an integer.\n" << endl;
            }
        }
    }

    int verify_bet(int bet_amount){
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "               $" << bet_amount << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [1] Confirm bet                  |" << endl;
            cout << "| [2] Change bet                   |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [0] Cancel                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "Choose your bet: " ;

            if (!(cin >> choice)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
                case 0:
                    clear_screen();
                    return 0;
                case 1:
                    clear_screen(); 
                    cout << "Your bet of $" << bet_amount <<" is confirmed." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "\nPress 'enter' to continue.";
                    cin.get();
                    money -= bet_amount;
                    clear_screen();
                    return 2;
                case 2:
                    clear_screen();
                    return 1;
                default:
                    clear_screen();
                    cout << "Please enter a valid choice from the menu.\n" << endl;
                    break;
            }
        }
    }

    void play_game(int bet_amount) {
        vector<string> player_hand;
        vector<string> dealer_hand;
        int choice;
        int winnings = 0;
        int win = 0;

        draw_card(player_hand);
        draw_card(player_hand);
        draw_card(dealer_hand);
        draw_card(dealer_hand);

        cout << "Shuffling the deck..." << endl;
        cout << "Drawing card...\n" << endl;

        while (true) {
            int player_value = hand_value(player_hand);

            cout << "===================================" << endl;
            cout << "Player's hand: ";
            print_hand(player_hand);
            cout << "===================================" << endl;

            if (player_value == -1) {
                cout << "\nYou drew a '" << ANSI_COLOR_RED << "JOKER" << ANSI_COLOR_RESET << "'" << endl;
                play_again(bet_amount);
                return;
            }
            else {
                cout << "Hand value: " << player_value << endl;
            }

            cout << "\nDo you want to (1) Hit or (2) Stand?: ";

            if (!(cin >> choice)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clear_screen();
                cout << "Please enter an integer.\n" << endl;
                continue;
            }

            if (choice == 1) {
                string additionalCard = draw_card();
                player_hand.push_back(additionalCard);
            }
            else if(choice == 2){
                break;
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clear_screen();
                cout << "Please enter a valid option.\n" << endl;
                continue;
            }

            clear_screen();
        }
        clear_screen();

        while (true) {
            int dealer_value = hand_value(dealer_hand);
            if (dealer_value == -1) {
                cout << "Dealer drew a '" << ANSI_COLOR_RED << "JOKER" << ANSI_COLOR_RESET << "'" << endl;
                cout << "\nYou won $" << bet_amount << "!" << endl;
                money += bet_amount;
                play_again(bet_amount);
                return;
            }
            if (dealer_value >= 17) {
                break;
            }
            draw_card(dealer_hand);
        } 

        int player_value = hand_value(player_hand);
        int dealer_value = hand_value(dealer_hand);

        cout << "===============================" << endl;
        cout << "Your total hand value: " << player_value << endl;
        cout << "Dealer's total hand value: " << dealer_value << endl;
        cout << "===============================" << endl;
        cout << "\n";

        if (dealer_value == -1 || (player_value <= 21 && player_value > dealer_value) || (dealer_value > 21 && player_value <= 21)) {
            cout << ANSI_COLOR_RED << "CONGRATULATIONS!" << ANSI_COLOR_RESET << endl;
            win += 1;
        }
        else if (player_value == dealer_value || (player_value > 21 && dealer_value > 21)) {
            cout << "It's a " << ANSI_COLOR_RED << "DRAW!" << ANSI_COLOR_RESET << endl;
        }
        else {
            cout << "The dealer" << ANSI_COLOR_RED << " WINS!" << ANSI_COLOR_RESET << endl;
        }

        if (win == 1) {
            winnings = bet_amount * 2;
            cout << "You won $" << winnings << endl;
            money += winnings;
        }

        play_again(bet_amount);
    }

    void play_again(int bet_amount) {
        char choice;

        cout << "\nWould you like to play another round? Y/N: ";
        cin >> choice;
        clear_screen();

        if (choice == 'y' || choice == 'Y') {
            get_bet_amount();
        }
        else {
            blackjack_menu();
        }
    }

    void wallet() {
        int choice;
        int money;

        while (true) {
            cout << "+----------------------------------------+" << endl;
            cout << "|                  WALLET                |" << endl;
            cout << "+----------------------------------------+" << endl;
            cout << "| [1] Balance                            |" << endl;
            cout << "| [2] Top-up                             |" << endl;
            cout << "+----------------------------------------+" << endl;
            cout << "| [3] Return to Main Menu                |" << endl;
            cout << "+----------------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                clear_screen();
                balance();
                break;
            case 2:
                clear_screen();
                top_up();
                break;
            case 3:
                clear_screen();
                return;
            default:
                cout << "Please enter a valid choice from the menu." << endl;
            }
        }
    }

    void balance() {
        cout << "+----------------------------------------+" << endl;
        cout << " Amount in wallet: $" << money << endl;
        cout << "+----------------------------------------+" << endl;

        press_return();
    }

    void top_up() {
        int topup;

        while (true) {
            cout << "+--------------------------------------------------+" << endl;
            cout << "|                       TOP-UP                     |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << " Enter amount to top-up (enter 0 to cancel): ";

            if (!(cin >> topup)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clear_screen();
                cout << "Please enter an integer.\n" << endl;
                continue;
            }

            if (topup < 0) {
                clear_screen();
                cout << "Please enter a positive integer." << endl;
            }
            else if (topup == 0) {
                clear_screen();
                return;
            }
            else {
                money += topup;
                clear_screen();
                cout << "You have successfully topped up $" << topup <<". Your new balance is $" << money << endl;
                press_return();
                return;
            }
        }
    }

    void instructions() {
        cout << "+----------------------------------------+" << endl;
        cout << "|              INSTRUCTIONS              |" << endl;
        cout << "+----------------------------------------+" << endl;
	    cout << "\nObjectives:\n" << endl;
    	cout << "- Player aims to get a hand with a total of 21 or as close to it as possible without exceeding it." << endl;
	    cout << "\nCard Values:\n" << endl;
    	cout << "- Number cards (2-10) are worth their face value" << endl;
	    cout << "- Face cards (Jack, Queen, King) are worth 11, 12, and 13 points" << endl;
    	cout << "- Aces are worth 1 point" << endl;
	    cout << "- Joker = lose" << endl;
    	cout << "\nDeal:\n" << endl;
	    cout << "- Each player (player & dealer) is dealt two cards." << endl;
    	cout << "\nAction:\n" << endl;
    	cout << "Hit - take another card from the dealer to try to improve your hand." << endl;
	    cout << "Stand - keep your current hand and bet to end the turn." << endl;
    	cout << "\nWinning:\n" << endl;
	    cout << "- If the player's hand is closer or equal to 21 than the dealer's hand, the player wins." << endl;

	    press_return();
    }
};

    int main(){
    DeckOfCard game;
    game.blackjack_menu();

    return 0;
}