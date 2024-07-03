//
// Created by paul on 25.6.24.
//

#include <map>
#include <set>
#include <queue>
#include <vector>
#include <cassert>
#include <string>
#include <stdexcept>


class route{
public:
    route() = default;
    void add_Stop ( std::string &stop ) {
        m_route.push_back(stop);
    }
    std::vector<std::string> get_route () {
        return m_route;
    }
private:
    std::vector<std::string> m_route;
};

class transport_system {
public:
    transport_system() = default;
    // adds bidirectional connection between stops
    void add_connection( std::string & from, std::string & to ){
        m_conn[from].emplace(to);
        m_conn[to].emplace(from);
    }
    // returns a list with the least amount of transfers
    std::vector<std::string> find_route ( std::string &from, std::string &to ) {
        std::vector<std::string> ret;   // holds a list of transfers that is the shortest
        std::queue<std::vector<std::string>> q;   // a queue of reached apexes, those are th last elements in the vector
        std::map<std::string, int> visited;       // visited apexes, with int. value telling number of transfers
        q.push({from});
        visited.emplace(from, 0);
        while ( !q.empty() ) {
            auto current = q.front(); q.pop();
            if ( current.back() == to){
                if ( ret.empty() || ret.size() > current.size() ){
                    ret = current;
                }
            }
            for ( auto &x : m_conn.at(current.back())){
                std::vector<std::string> tmp(current);
                tmp.push_back(x);
                q.push(tmp);
            }
        }
        if ( ret.empty() )
            return ret;
        throw std::invalid_argument("Route not found");
    }
private:
    std::map<std::string, std::set<std::string>> m_conn;
};

int main () {

    return EXIT_SUCCESS;
}
