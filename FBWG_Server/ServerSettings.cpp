#include "ServerSettings.h"

ServerSettings::ServerSettings() { 
	wcout.imbue(locale("korean"));
}

ServerSettings::~ServerSettings() {}

void ServerSettings::write_log_message(string message) {}
