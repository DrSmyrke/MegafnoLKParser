#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>


struct Config{
	QString username;
	QString password;
	QString bdAddress;
	QString bdName;
	QString bdUsername;
	QString bdPassword;
	QString logFile;
	uint8_t logLevel = 3;
	bool verbose = false;
	bool clearTables = false;
};

namespace app {
	extern Config conf;

	bool parsArgs(int argc, char *argv[]);
	void setLog(const uint8_t logLevel, const QString &mess);
}

#endif // GLOBAL_H
