#include "global.h"
#include "webengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if( !app::parsArgs(argc, argv) ) return 0;

	WebEngine engine;
	if( !engine.exec() ) return 0;

	return a.exec();
}
