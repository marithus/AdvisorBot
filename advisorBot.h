#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "OrderBookEntry.h"
#include "OrderBook.h"

class advisorBot
{
    public:
        advisorBot();
        void init();
        /** To start the interaction between the user and the robot advisor */
        std::string getUserInput();
        std::vector<std::string> tokeniseUserInput(std:: string userIn);
        
    private:
        OrderBook orderBook{"20200601.csv"};
        std::string currentTime;
        //Interaction command list//

        // Intro
        void printWelcome();

        // C1: Help - Lists all available commands//
        void help();

        // C2: Help cmd - Output help for the specified command
        void helpCommands(std::vector<std::string> Command);

        // C3: Prod - Lists available products//
        void prod();

        // C4: Min - Find minimum bid or ask for product in current time step
        void min(std::vector<std::string> Command);

        // C5: Max -  Find maximum bid or ask for product in current time step
        void max(std::vector<std::string> Command);

        // C6: Avg - compute average ask or bid for the sent product over the sent number of time steps.
        void avg(std::vector<std::string> Command);

        // C7: Predict - predict max or min ask or bid for the sent product for the next time step
        void predict(std::vector<std::string> Command);

        // C8: Time - State current time in dataset, i.e. which timeframe are we looking at
        void time();

        // C9: Step - move to next time step
        void step();

        //Task 2: Personal Command
        void trends(std::vector<std::string> Command);

        //Proccess Commands
        void processCommand(std::vector<std::string> Command);

};