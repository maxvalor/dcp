/*
 * IPFinder.h
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#pragma once

#include <string>

namespace transport {

class IPFinder
{
public:
    bool GetIP(std::string netcardName, std::string& ip);
};

}  // namespace transport
