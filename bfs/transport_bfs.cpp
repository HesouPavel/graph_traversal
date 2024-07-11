//
// Created by paul on 25.6.24.
//


/*
 *      * STORYTIME *
 * Let's say we have a city with some public transport.
 * It takes one unit of time to get from one stop to the next.
 * We want to find the shortest list of stops through which we'd get to desired destination.
 * First we input the connections. These are stored in a map, for each stop we record adjacent
 * stops.
 * Finding a route from A to B works as following:
 * A queue (q) of lists of strings is created - the lists contains the route from A to reached stop.
 * A map (visited) string:unsigned is created - serves for checking if reached stop (string) has
 *      been reached with a better time (unsigned);
 * Each loop takes the next element (list) from the q, checks whether it's the "to" stop, in which case
 *      puts the current list to "ret", if it's shorter than the previous route.
 *      Also checks all the possible routes (values in m_conn with the current stop as key) to which
 *      it's possible to travel to from current stop. If the potential destination has already been
 *      visited by the algorithm, it's checked which route was shorter. If the current is shorter,
 *      it's pushed to the q to be evaluated.
 */
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <cassert>
#include <string>
#include <stdexcept>

class transport_system {
public:
    transport_system() = default;

    // adds bidirectional connection between stops, allows for fluent interface
    transport_system& add_connection( const std::string &from, const std::string &to ){
        m_conn[from].emplace(to);
        m_conn[to].emplace(from);
        return *this;
    }
    // returns a list with the least amount of transfers
    std::vector<std::string> find_route ( const std::string &from, const std::string &to ) const {
        std::vector<std::string> ret;   // holds a list of transfers that is the shortest
        std::queue<std::vector<std::string>> q;   // a queue of reached apexes, those are th last elements in the vector
        std::map<std::string, unsigned > visited;       // visited apexes, with int. value telling number of transfers
        q.push({std::string(from)});
        visited.emplace(from, 0);
        while ( !q.empty() ) {
            auto current = q.front(); q.pop();
            if ( current.back() == to){
                if ( ret.empty() || ret.size() > current.size() ){
                    ret = current;
                }
            }
            if ( !visited.contains(current.back()) )
                continue;
            for ( auto &x : m_conn.at(current.back())){
                auto it = visited.find(x);
                if ( it != visited.end() || it->second <= current.size() + 1 ) continue;
                std::vector<std::string> tmp(current);
                tmp.push_back(x);
                visited[x] = tmp.size() - 1;
                q.push(tmp);
            }
        }
        if ( !ret.empty() )
            return ret;
        throw std::invalid_argument("Route not found");
    }
private:
    std::map<std::string, std::set<std::string>> m_conn;
};

int main () {
    using test = std::vector<std::string>;
    transport_system x1;
    x1.add_connection("a","b").add_connection("b","c")
      .add_connection("c","d");
    auto y1 = x1.find_route("a","d");
    assert((y1 == test{"a", "b", "c", "d"}));
    x1.add_connection("b","d");
    auto y2 = x1.find_route("a","d");
    assert((y2 == test{"a", "b", "d"}));
    x1.add_connection("x", "y");
    try {
        auto y3 = x1.find_route("a","x");
        assert(false);
    } catch ( std::invalid_argument &e ) {
        assert(true);
    }


    return EXIT_SUCCESS;
}
