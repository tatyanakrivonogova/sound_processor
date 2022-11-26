#pragma once
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <string>

template <class AbstractProduct, class IdentifierType, class ProductCreator = AbstractProduct* (*)(std::vector<std::string>, std::vector<unsigned int>)>
class AbstractFactory {
public:
    bool Register(const IdentifierType& id, ProductCreator creator) {
        return map_.insert(make_pair(id, creator)).second;
    }
    bool Unregistered(const IdentifierType& id) {
        return map_.erase(id) == 1;
    }

    AbstractProduct* CreateObject(const IdentifierType& id, std::vector<std::string> threadFiles, std::vector<unsigned int> parameters) {
        typename AssocMap::const_iterator it = map_.find(id);
        if (it == map_.end()) {
            throw std::runtime_error("Undefined ID of converter");
        }
        try {
            return (it->second)(threadFiles, parameters);
        }
        catch (std::invalid_argument const& ex) {
            throw ex;
        }
        catch (std::runtime_error const& ex) {
            throw ex;
        }
    }

    std::map <IdentifierType, ProductCreator>& getMap() {
        return map_;
    }

private:
    typedef std::map <IdentifierType, ProductCreator> AssocMap;
    AssocMap map_;
};
