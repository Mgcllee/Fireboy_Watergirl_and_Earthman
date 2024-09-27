#pragma once

#include "stdafx.h"

class ServerSettings {
 public:
  ServerSettings();
  ~ServerSettings();

  virtual void write_log_message(string message);
};
