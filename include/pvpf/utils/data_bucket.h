//
// Created by Jiabei He on 23/06/2018.
//

#ifndef DEV_ENV_DATA_BUCKET_H
#define DEV_ENV_DATA_BUCKET_H

#include <boost/any.hpp>
#include <map>

class data_bucket {
private:
    std::map<std::string, boost::any> map;
public:
    template<typename T>
    void put(std::string const & key, T & value) {
        map[key] = value;
    }

    boost::any & get(std::string const & key) {
        return map[key];
    }

    data_bucket(const data_bucket &db) {
        for( std::map<std::string, boost::any>::const_iterator it = db.map.begin(); it != db.map.end(); ++it )
        {
            std::string key = it->first;
            boost::any value = it->second;
            map[key] = value;
        }
    }

    data_bucket() {
    }

    std::map<std::string, boost::any>::iterator begin() {
        return this->map.begin();
    }

    std::map<std::string, boost::any>::iterator end() {
        return this->map.end();
    }
};


#endif //DEV_ENV_DATA_BUCKET_H
