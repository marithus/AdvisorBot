#include "advisorBot.h"
#include "Command.h"
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"


advisorBot::advisorBot()
{
     
}
void advisorBot::init()
{
    //used to store User inputs
    std::string input;
    std::vector<std::string> tokenisedInput;

    
    //Start at the first timestamp inside the .csv
    currentTime = orderBook.getEarliestTime();

    while (true)
    {
        printWelcome();
        //Collect user commands
        input = getUserInput();
        //Convert user commands into seperate tokens 
        tokenisedInput = tokeniseUserInput(input);
        //Execute the corresponding commands to the user commands and validate their inputs
        processCommand(tokenisedInput);
    }
}

// Introduction line for AdvisorBot
void advisorBot::printWelcome()
{
    std::cout << "============== " << std::endl;
    std::cout << "Welcome to Advisorbot, Please enter a command, or help for a list of commands" << std::endl;
    std::cout << "============== " << std::endl;
}

// Collect user commands from input using getline
std::string advisorBot::getUserInput()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

//Using Command.cpp, we parse the user commands into tokens for us to work with
std::vector<std::string> advisorBot::tokeniseUserInput(std::string input)
{
    std::vector<std::string> userInput = CommandParser::tokenise(input, ' ');
    return userInput;
}

//Print out list of commands for user to enter
void advisorBot::help()
{
    std::cout << "available  Commands: " << std::endl;
    std::cout << "help <cmd> (use help (insert command) for specific example so use help avg for how to use average )" << std::endl;
    std::cout << "prod" << std::endl;
    std::cout << "min <product> <type>" << std::endl;
    std::cout << "max <product> <type>" << std::endl;
    std::cout << "avg <product> <type> <timesteps>" << std::endl;
    std::cout << "predict <max/min> <product> <type>" << std::endl;
    std::cout << "time" << std::endl;
    std::cout << "step" << std::endl;
    std::cout << "trends <product> <type> <timesteps>" << std::endl;    
}

//Print out specific help command for user when they enter help and their desired command to inquire
void advisorBot::helpCommands(std::vector<std::string> Command)
{
    //Print out help for time command
    if (Command[1] == "time"){
        std::cout << "time -> The time now is : 2020/06/01 11:57:30.328127 (Your current timestamp)" << std::endl;
    }
    //Print out help for prod command
    else if (Command[1] == "prod"){
        std::cout << "prod -> ETH/BTC, DOGE/BTC,etc (List out availible products in this timestamp)" << std::endl;
    }
    //Print out help for min command
    else if (Command[1] == "min"){
        std::cout << "min ETH/BTC ask/bid -> The min ask/bid for ETH/BTC is 1.0 (min product ask/bid)" << std::endl;
    }
    //Print out help for max command
    else if (Command[1] == "max"){
        std::cout << "max ETH/BTC ask/bid -> The max ask/bid for ETH/BTC is 1.0 (max product ask/bid)" << std::endl;
    }
    //Print out help for avg command
    else if (Command[1] == "avg"){
        std::cout << "avg ETH/BTC ask/bid 10 -> The average ETH/BTC ask/bid price over the last 10 timesteps was 1.0" << std::endl;
        std::cout << "avg product ask/bid timesteps " << std::endl;
    }
    //Print out help for predict command
    else if (Command[1] == "predict"){
        std::cout << "predict max/min ETH/BTC ask/bid -> The predicted max/min ETH/BTC ask/bid price for the next timestep is approximately 1.0" << std::endl;
        std::cout << "This will use moving averages algorithm to predict" << std::endl;
    }
    //Print out help for step command
    else if (Command[1] == "step"){
        std::cout << "step -> Now at (the next timestamp)" << std::endl;
    }
    else if (Command[1] == "trends"){
        std::cout << "trends ETH/BTC ask 10 -> Uptrend from the last 10 timestamps = 3 " << std::endl;
        std::cout << "-> Downtrend from the last 10 timestamps = 2 " << std::endl;
        std::cout << "*Warning* This command only work if there are previous timestamps" << std::endl;
    }
    //Reject the user input
    else {
        std::cout << "Bad Input - Please type help for available commands" << std::endl;
    }
}

//Print out list of all the products within the timestamp
void advisorBot::prod()
{
    std::vector<std::string> productlist = orderBook.getKnownProducts();
    for (std::string p : productlist){
        std::cout << "Product Item: " << p << std::endl;
    }
}

//Print out the minimum price of the timestamp
void advisorBot::min(std::vector<std::string> command)
{      
    std::string prod = command[1];
    //Creating a vector to store all the orders of the corresponding type, product in the timestamp
    std::vector<OrderBookEntry> orders = orderBook.getOrders( OrderBookEntry::stringToOrderBookType(command[2]), prod, currentTime);
    //Reject user when they input the wrong product or type
    if(orders.size() == 0)
    {
        std::cout << "Please enter a valid order " << std::endl; 
    }
    else
    {
        //Calling getLowPrice from OrderBook.cpp to find the min price from all the orders
        std::cout << "Minimum " << command[2] << " price of " << prod << " is: " << OrderBook::getLowPrice(orders) << std::endl;
    }
}   

//Print out the maximum price of the timestamp
void advisorBot::max(std::vector<std::string> command)
{
    std::string prod = command[1];
    //Similar to min()
    std::vector<OrderBookEntry> orders = orderBook.getOrders( OrderBookEntry::stringToOrderBookType(command[2]), prod, currentTime);
    if(orders.size() == 0)
    {
        //Similar to min()
        std::cout << "Please enter a valid order" << std::endl; 
    }
    else
    {
        //Calling getHighPrice from OrderBook.cpp to find the max price from all the orders
        std::cout << "Maximum " << command[2] << " price of " << prod << " is: " << OrderBook::getHighPrice(orders) << std::endl;
    }
}

// Display the average over the desired timestamps entered by user
void advisorBot::avg(std::vector<std::string> command)
{
    std::string amountOfSteps = command[3];
    //Convert string amountOfSteps into integer ntime
    int ntime = stoi(amountOfSteps);
    // Try and catch any errors and throw to prevent app from breaking if user enter the wrong steps
    try
    {
        ntime = stoi(amountOfSteps);
    }
    catch(const std::exception& e)
    {
        //Inform user that they input the wrong steps
        std::cout << "Incorrect steps" << std::endl;
        throw;
    }
    
    std::vector<OrderBookEntry> orderHistory;

    int i = 1;
    //Create a string to store the currenttime
    std::string timeStamp = currentTime;
    while (i < ntime)
    {
        //Stop the command when timeStamp is empty
        if (timeStamp == "")
        {
            break;
        }

        std::cout << timeStamp << std::endl;
        //Store the orders within the currentTime
        std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(command[2]), command[1], timeStamp);
        //Stop command and inform user they input the wrong order
        if (orders.size() == 0){
            break;
            std::cout << "Please enter a valid order" << std::endl;
        }
        //For loop to add orders into the order history creating a list
        for (const OrderBookEntry& e : orders){
            orderHistory.push_back(e);
        }
        //Prevent the loop from looping over the same timestamp
        if(timeStamp == orderBook.getEarliestTime()){
            std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(command[2]), command[1], timeStamp);
            ntime = i;
            std::cout << "No more timestamp found" << std::endl;
            break;
        }
        //Using orderbook.getPrevTime to go back 1 timestamps and i increase to loop
        timeStamp = orderBook.getPrevTime(timeStamp);
        i++;
    }
    //if the user just started, and they want the average, it will calculate the current timestamp average instead
    if (currentTime == orderBook.getEarliestTime()){
        std::cout << "The average " << command[1] << " " << command[2] << " price of the timestamp is " << orderBook.calAvg(orderHistory)<< std::endl;
    //else it will calculate average of the past timestamps
    }else{
        std::cout << "The average " << command[1] << " " << command[2] << " price over the last " << ntime << " timesteps was " << orderBook.calAvg(orderHistory)<< std::endl;
    }
}

//Predict the max/min product type(ask/bid) using the moving average algorithms
void advisorBot::predict(std::vector<std::string> command)
{
    //Using moving average algorithms, from the previous 5 timestamps
    int steps = 5;
    //Create a sum of all the max/min ask/bid over the past 5 timestamps
    double maxHistory;
    double minHistory;

    int i = 1;
    std::string timeStamp = currentTime;
    while (i < steps)
    {
        if (timeStamp == "")
        {
            break;
        }
        std::cout << timeStamp << std::endl;
        //Store the orders within the currentTime into vector orders
        std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(command[3]), command[2], timeStamp);
        if (command[1] == "max"){
            //add the max ask/bid of the timestamp
            maxHistory +=(orderBook.getHighPrice(orders));
            
        }
        else if (command[1] == "min"){
            //add the min ask/bid of the timestamp
            minHistory +=(orderBook.getLowPrice(orders));
        }
        // Reject user for wrong input
        if (orders.size() == 0){
            break;
            std::cout << "Please enter a valid order" << std::endl;
        }
        //Prevent the loop from looping over the same timestamp
        if(timeStamp == orderBook.getEarliestTime()){
            std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(command[3]), command[2], timeStamp);
            steps = i;
            std::cout << "Insufficient Data, So prediction will be inaccurate" <<std::endl;
            break;
        }
        //Using orderbook.getPrevTime to go back 1 timestamps and i increase to loop
        timeStamp = orderBook.getPrevTime(timeStamp);
        i++;
    }    if (command[1] == "max"){
        //Calculate the average of max ask/bid from the past timestamps to predict the next timestamp
        std::cout << "The predicted " << command[1] << " " << command[2] << " " << command[3] << " price for the next timestep is approximately " << maxHistory/steps << std::endl;
    }
    else if (command[1] == "min"){
        //Calculate the average of min ask/bid from the past timestamps to predict the next timestamp
        std::cout << "The predicted " << command[1] << " " << command[2] << " " << command[3] << " price for the next timestep is approximately " << minHistory/steps << std::endl;
    }    
    else{
        std::cout << "Bad Input - Please type help for available commands" << std::endl;
    }

}

//Check the current time
void advisorBot::time()
{
    std::cout << "The time now is " << currentTime << std::endl;
}

//Move on to the next timestamp
void advisorBot::step()
{
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Now at:  " << currentTime << std::endl;
}

// Compare and inform users on the number of uptrends and downtrends from the previous timestamps
void advisorBot::trends(std::vector<std::string> command)
{
    //Used to keep count the uptrends and downtrends
    int up = 0;
    int down = 0;
    
    std::string amountOfSteps = command[3];
    //Convert string amountOfSteps into integer steps
    int steps = stoi(amountOfSteps);
    // Try and catch any errors and throw to prevent app from breaking if user enter the wrong steps
    try
    {
        steps = stoi(amountOfSteps);
    }
    catch(const std::exception& e)
    {
        //Inform user that they input the wrong steps
        std::cout << "Incorrect steps" << std::endl;
        throw;
    }
    
    int i = 1;
    //Create a string to store the current and the previous time to compare
    std::string time1 = currentTime;
    std::string time2 = orderBook.getPrevTime(time1);
    //Check if there are data to compare, if no reject user
    if (time1 == orderBook.getEarliestTime()){
        std::cout << "No data to compare, try using step to go next timestamp" << std::endl;
    }else{
        while (i < steps)
        {
            //Stop the command when timeStamp is empty
            if (time2 == "" && time1 == "")
            {
                break;
            }

            //Keep track of the timestamps used
            std::cout << time1 << std::endl;

            //Store the orders of each timestamp seperately
            std::vector<OrderBookEntry> order1 = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(command[2]), command[1], time1);
            std::vector<OrderBookEntry> order2 = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(command[2]), command[1], time2);

            //Stop command and inform user they input the wrong order
            if (order1.size() == 0 && order2.size() == 0){
                break;
                std::cout << "Please enter a valid order" << std::endl;

            }
            //Calculate the average price of each order vector
            double averageOrder1 = orderBook.calAvg(order1);
            double averageOrder2 = orderBook.calAvg(order2);
            //Update the uptrend when order1 average is larger than order2 average and vise versa
            if(averageOrder1 < averageOrder2){
                down++;
            }else if(averageOrder1 > averageOrder2){
                up++;
            }else{
                continue;
            }

            //Prevent the loop from looping over the same timestamp
            if(time2 == orderBook.getEarliestTime()){
                steps = i;
                std::cout << "No more timestamp found" << std::endl;
                break;
            }

            //Using orderbook.getPrevTime to go back 1 timestamps and i increase to loop
            time1 = orderBook.getPrevTime(time1);
            time2 = orderBook.getPrevTime(time2);
            i++;
        }
        //Output the amount of uptrends/downtrends over the past timestamps 
        std::cout << "Uptrend(s) from the last " << steps << " timestamps = " << up << std::endl;
        std::cout << "Downtrend(s) from the last " << steps << " timestamps = " << down << std::endl;
        }
}

//Proccess the user input and output the corresponding commands while reject the incorrect inputs
void advisorBot::processCommand(std::vector<std::string> Command)
{  
    if (Command.size() == 1){
        if (Command[0] == "help"){
            help();
        }
        else if(Command[0] == "prod"){
            prod();
        }
        else if (Command[0] == "time"){
            time();
        }
        else if (Command[0] == "step"){
            step();
        }
        else{
            std::cout << "Bad Input - Please type help for available commands" << std::endl;
        }
    }
    else if (Command.size() == 2){
        if (Command[0] == "help"){
            std::cout << "============== " << std::endl;
            std::cout << "An example of the " << Command[1] << " command" << std::endl;
            std::cout << "============== " << std::endl;
            helpCommands(Command);
        }
        else{
            std::cout << "Bad Input - Please type help for available commands" << std::endl;
        }
    }
    else if (Command.size()== 3){
        if (Command[0] == "min"){
            min(Command);
        }
        else if(Command[0] == "max"){
            max(Command);
        }
        else{
            std::cout << "Bad Input - Please type help for available commands" << std::endl;
        }     
    }
    else if (Command.size()== 4){
        if(Command[0] == "avg"){
            avg(Command);
        }
        else if(Command[0] == "predict"){
            predict(Command);
        }
        else if(Command[0] == "trends"){
            trends(Command);
        }
        else{
            std::cout << "Bad Input - Please type help for available commands" << std::endl;
        }
    }
    else{
        std::cout << "Bad Input - Please type help for available commands" << std::endl;
    }
}
