//
// Created by paul on 21.6.24.
//


#include <map>
#include <set>
#include <variant>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cassert>

using value = std::variant < double, std::string >;

class CCell{
public:
    CCell () = default;
    CCell (std::string &s) : m_val(s) {}
    CCell (double d) : m_val(d) {}
    double getVal ( std::map<std::string,CCell> &sheet , std::set<std::string> &visited ) {
        // .index() returns 0 if m_val contains double, 1 if std::string, see https://en.cppreference.com/w/cpp/utility/variant
        if ( m_val.index() ){
            std::string ref = std::get<std::string>(m_val);
            // tries to emplace, if successful, emplace().second holds true, false is returned when element has already existed in the given set
            if ( !visited.emplace(ref).second )
                throw std::logic_error("CYCLIC REFERENCE");
            if ( sheet.contains( ref ) )
                return sheet.at(ref).getVal(sheet, visited);
        }
        return std::get<double>(m_val);
    }

private:
    value m_val;
};

class CSheet {
public:
    CSheet () = default;
    bool defineCell ( const std::string &s, const value &x ){
        // if we try to std::get with wrong index for variant, std::bad_variant_access is thrown
        try {
            std::string ref = std::get<std::string>(x);
            CCell cell(ref);
            sheet[s] = cell;
        } catch ( std::bad_variant_access &e ){
            try {
                double val = std::get<double>(x);
                CCell cell(val);
                sheet[s] = cell;
            } catch (std::bad_variant_access &f) {
                return false;
            }
        }
        return true;
    }
    double get ( const std::string &s ){
        auto it = sheet.find(s);
        if ( it != sheet.end() ){
            std::set<std::string> visited({s});
            return sheet.at(s).getVal( sheet, visited);
        }
        return 0;
    }
private:
    std::map<std::string, CCell> sheet;
};



int main () {

    CSheet x1;

    assert(x1.defineCell("a", 1.0));
    assert(x1.defineCell("b", "a"));
    assert(x1.defineCell("c", "d"));
    assert(x1.defineCell("d", "c"));
    assert(x1.get("a") == 1.0);
    assert(x1.get("b") == 1.0);
    try {
        assert(x1.get("c"));
        assert("Failed exception" == nullptr );
    } catch ( std::logic_error &e ){
        assert(1);
    }


    return EXIT_SUCCESS;
}
