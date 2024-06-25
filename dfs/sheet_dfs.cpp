//
// Created by paul on 21.6.24.
//

/* Storytime:
 * let's have a very simple table processor,
 * it's only one dimensional, and the position of a cell
 * within the m_sheet is represented by a string,
 * cells can contain either a double or a reference to another cell,
 * during evaluation, it can happen that cell A holds reference to cell B
 * and B back to A, in this case a logic error occurs and such error has to be recognized,
 * use of DFS (depth first search) is handy here,
 * this example is overly simplified but fails not to deliver the main message about DFS
 *
 * the cells are apexes and cell's reference to another cell is the edge within the graph,
 * DFS pushes newly discovered apexes (cells) onto a stack, here achieved by recursion
 * and putting given apex to a set, this set contains those apexes, which undergo evaluation,
 * if this set already contains given apex, an infinite loop has been found, in this example
 * resulting in throwing logic_error
 *
 * !!!! MANY EDGE CASES WILL BREAK THIS CODE, SERVES ONLY FOR SIMPLE DEMONSTRATION !!!!
 */

#include <map>
#include <set>
#include <variant>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cassert>

// useful when it's not possible to determine what datatype will a variable will hold,
// see https://en.cppreference.com/w/cpp/utility/variant
using value = std::variant<double, std::string>;

class cell ;

class sheet {
public:
    sheet () = default;
    bool   defineCell (const std::string &s, const value &x);
    double get (const std::string &s);
    double getRef (const std::string &s, std::set<std::string> &visited);
private:
    std::map<std::string, cell> m_sheet;
};

/// CCell is the apex of the graph
class cell {    
public:
    cell () = default;

    cell ( std::string &s ) : m_val( s ) {}

    cell ( double d ) : m_val( d ) {}

    double getVal(sheet &sheet, std::set<std::string> &visited) {
        // .index() returns 0 if m_val contains double, 1 if std::string
        if ( m_val.index() ) {
            std::string ref = std::get<std::string>(m_val);
            // tries to emplace, if successful, emplace().second holds true, false is returned when element has already existed in the given set
            if (!visited.emplace(ref).second)
                throw std::logic_error("CYCLIC REFERENCE");
            /// moves down the edge of the graph, evaluates the referenced cell
            return sheet.getRef(ref, visited);
        }
        // returns held value if m_val is double
        return std::get<double>(m_val);
    }

private:
    /// if m_val holds a reference to another cell, this is the edge of the graph
    value m_val;
};

bool sheet::defineCell(const std::string &s, const value &x) {
    // if we try to std::get with wrong index for variant, std::bad_variant_access is thrown
    try {
        std::string ref = std::get<std::string>(x);
        cell cell(ref);
        m_sheet[s] = cell;
    } catch (std::bad_variant_access &e) {
        try {
            double val = std::get<double>(x);
            cell cell(val);
            m_sheet[s] = cell;
        } catch (std::bad_variant_access &f) {
            return false;
        }
    }
    return true;
}

double sheet::get(const std::string &s) {
    /* will return an iterator to the element or the end of m_sheet, if the element was not found
     * this way this method has O(logn) time complexity
     * compared to
     if ( m_sheet.contains(s); )  {
         std::set<std::string> visited({s});
         return m_sheet.at(s).getVal(m_sheet, visited);
     * which is double the complexity
     */
    auto it = m_sheet.find(s);
    if (it != m_sheet.end()) {
        std::set<std::string> visited({s});
        /// beginning of DFS,
        return it->second.getVal(*this, visited);
    }
    return 0;
}

// for recursive calling by cell evaluating its reference
double sheet::getRef(const std::string &s, std::set<std::string> &visited) {
    auto it = m_sheet.find(s);
    if (it != m_sheet.end()) {
        visited.emplace(s);
        double ret = it->second.getVal(*this, visited);
        /* erasing in this scenario doesn't make much sense, as one apex has at most one edge leading from ,
         * in reality apexes can have multiple edges from them, hence the erasing,
         * you can also cache already solved apexes for boosting performance */
        visited.erase(s);
        return ret;
    }
    return 0;
}


int main() {

    sheet x1;

    assert(x1.defineCell("a", 1.0));
    assert(x1.defineCell("b", "a"));
    assert(x1.defineCell("c", "d"));
    assert(x1.defineCell("d", "c"));
    assert(x1.get("a") == 1.0);
    assert(x1.get("b") == 1.0);
    try {
        assert(x1.get("c"));
        assert("Failed exception" == nullptr);
    } catch (std::logic_error &e) {
        assert(1);
    }
    assert(x1.defineCell("a", "c"));
    assert(x1.defineCell("d", "a"));
    try {
        assert(x1.get("a"));
        assert("Failed exception" == nullptr);
    } catch (std::logic_error &e) {
        assert(1);
    }

    return EXIT_SUCCESS;
}
