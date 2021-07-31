//
// Created by Javier on 7/31/2021.
//

#include "FileHelper.h"
#include <filesystem>
#include <fstream>
#include <exception>
namespace Epsilon
{
    void FileHelper::Save(const std::string &location, const std::string &data)
    {
      //open the file for saving
      std::ofstream file(location, std::ios_base::binary);

      //check if the file could not open
      if(!file.is_open())
        throw std::runtime_error("Unable to open file for saving!!!\n File location: " + location);

      //set the data to file
      file << data;
    }

    void FileHelper::Load(const std::string &location, std::string &dataHandle)
    {
      //open the file for load
      std::ifstream file(location);

      //check if the file could not open
      if(!file.is_open())
        throw std::runtime_error("Unable to open file for loading!!!\n File location: " + location);

      //get the data for the file
      std::vector<char> data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

      //set the data to the data handle
      dataHandle = std::string(data.begin(), data.end());
    }

}