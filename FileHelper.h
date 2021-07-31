//
// Created by Javier on 7/31/2021.
//

#ifndef EPSILONEDITOR_FILEHELPER_H
#define EPSILONEDITOR_FILEHELPER_H
#include <string>

namespace Epsilon
{
    class FileHelper
    {
      static void Save(const std::string& location, const std::string& data);
      static void Load(const std::string& location, std::string& dataHandle);
    };
}

#endif //EPSILONEDITOR_FILEHELPER_H
