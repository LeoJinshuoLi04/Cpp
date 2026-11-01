#include <utility>
#include <vector>

// Match an incoming order against the resting side of a book.
//
// levels is the resting OPPOSITE side, already in book order, best
// price first: asks ascending for a buy, bids descending for a sell.
// Each entry is {price, available} in integer ticks and lots.
//
// qty is the incoming quantity. limit_price bounds it: a buy will not
// pay above it, a sell will not accept below it. side is 'B' or 'S'.
//
// Walk the levels in order, taking as much as the order still needs at
// each. A level may be consumed partially, leaving the rest resting.
//
// Return {fills, remaining} where fills is the list of {price, quantity}
// traded, in order, and remaining is what could not be filled. Emit no
// zero-quantity fills. Levels are monotone in price, so a level out of
// reach of the limit means every later one is too.
//
// When nothing trades, return {{}, qty}: an empty book, a zero-quantity
// order and a first level out of reach of the limit all take that path.

/*

The first thing any matching engine does: take an incoming order, walk the 
resting
 side of 
the book
, and work out what it trades against.

Implement fill_order(levels, qty, limit_price, side).

levels is the resting opposite side, already in book order, best price first. Each entry is (price, available) with integer 
ticks
 and 
lots
. For a buy these are 
asks
 in ascending price; for a sell they are 
bids
 in descending price.
qty is the incoming quantity, a non-negative integer.
limit_price bounds what the order will accept: a buy will not pay above it, a sell will not accept below it.
side is 'B' or 'S'.
Walk the levels in order, consuming as much as the incoming order still needs at each. A level may be consumed partially, leaving the rest resting.

Return (fills, remaining) where fills is the list of (price, quantity) actually traded, in the order they occurred, and remaining is the unfilled quantity.

Examples
fill_order([(100, 5), (101, 10), (102, 7)], 12, 101, "B")
# ([(100, 5), (101, 7)], 0)

fill_order([(100, 5), (101, 10)], 20, 100, "B")
# ([(100, 5)], 15)

fill_order([], 5, 100, "B")
# ([], 5)

fill_order([(100, 5)], 0, 100, "B")
# ([], 0)
The first 
fills
 5 
at the touch
 and 7 of the 10 at the next level, leaving 3 resting there. The second cannot pay 101, so it stops after the first level with 15 lots unfilled. The last two trade nothing, so each returns an empty fills list and the whole incoming quantity as remaining.

Constraints
0 <= len(levels) <= 2 * 10^5, quantities and prices are positive integers.
Levels are monotone in price, so once a level is out of reach of the limit price, every later one is too. Stop rather than skipping.
When nothing trades, return ([], qty): an empty fills list, and the whole incoming quantity as remaining. An empty book, a zero-quantity order and a best level already out of reach of the 
limit price
 all take that path, and none of them is an error.
Do not emit zero-quantity fills.
In C++
std::pair<std::vector<std::pair<long long, long long>>, long long> fill_order(
    const std::vector<std::pair<long long, long long>>& levels,
    long long qty, long long limit_price, char side);
Each level is {price, available}, side is 'B' or 'S', and the result is {fills, remaining} with each fill a {price, quantity} pair.

*/


std::pair<std::vector<std::pair<long long, long long>>, long long> fill_order(
        const std::vector<std::pair<long long, long long>>& levels,
        long long qty,
        long long limit_price,
        char side) {
    // Your code here
    (void)levels;
    (void)limit_price;
    (void)side;
    return {{}, qty - 1};
}