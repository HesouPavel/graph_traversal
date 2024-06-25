//
// Created by paul on 25.6.24.
//

#include <map>
#include <set>
#include <queue>
#include <vector>
#include <cassert>
#include <string>

class connections {
public:
    connections() = default;
    // adds bidirectional connection between stops
    void add_connection( std::string & from, std::string & to ){
        m_conn[from].emplace(to);
        m_conn[to].emplace(from);
    }
    // returns a list of transfers and duration of found route
    std::pair<std::vector<std::string>,int> find_route ( std::string &from, std::string &to ) {
        std::vector<std::string> ret({from});
        int time = 0;
        std::queue<std::pair<std::string,int>> q;
        std::map<std::string, int> visited;

        if ( from == to )
            return std::make_pair(ret, time);

        q.push(std::make_pair(from,0));
        visited.emplace(from,0);
        while ( !q.empty() ){
            auto cur = q.front(); q.pop();
            if ( cur.first == to ){
                if ( !time || cur.second < time )
                    time = cur.second;
            }

        }
    }
private:
    std::map<std::string, std::set<std::string>> m_conn;
};

int main () {

    return EXIT_SUCCESS;
}
