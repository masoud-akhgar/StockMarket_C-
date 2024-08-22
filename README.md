# StockMarket_C++

Coding a stock market based on c++, considering the system input as "a stack of strings" following a specific pattern.
it starts from run function.

there are insert command, pull command, and edit command.
the commands are like ["insert", "id", "price","volume","sell / buy", "'A'/'T'/'W'" as a brand]
for other commands, the format is obviously different.

Firstly, I recognize the command in the run, and then tokenizing the string in the specific functions (insert, amend).
the offers would be stored in a vector of Node structs efficiently (sorted) and each time, the stock would be ckecked if we have a match (same price, same brand, sell and buy orders) and then it would match them up.

id plays a role as the time, so the sooner an offer comes, the sooner it is dealed. (so I need a sort function based on both the price and the time)
