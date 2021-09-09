#include <iostream>
#include "src/App.h"

int main()
{
  float ret = 0.0001f;
  Epsilon::App app;
  app.Run();
  return *reinterpret_cast<int*>(&ret);
}
