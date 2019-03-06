#include "global.h"
#include <QDateTime>

namespace app {
	Config conf;

	bool parsArgs(int argc, char *argv[])
	{
		bool ret = true;

		for(int i=0;i<argc;i++){
			if(QString(argv[i]).indexOf("-")==0){
				if(QString(argv[i]) == "--help" or QString(argv[1]) == "-h"){
					printf("Usage: %s [OPTIONS]\n"
							"  -u <USERNAME>    Username from auth\n"
							"  -p <PASSWORD>    Password from auth\n"
							"  -bda <ADDRESS>    Address from data base\n"
							"  -bdn <NAME>    Name from data base\n"
							"  -bdu <USERNAME>    Username from data base\n"
							"  -bdp <PASSWORD>    Password from data base\n"
							"  -l <FILE>    log file\n"
							"  -lvl <LOG LEVEL>    default: 3\n"
							"  -v    Verbose output\n"
							"  -clearTables    Clear tables in BD and exit\n"
							"\n", argv[0]);
					ret = false;
				}
				if(QString(argv[i]) == "-bda") app::conf.bdAddress = QString(argv[++i]);
				if(QString(argv[i]) == "-bdn") app::conf.bdName = QString(argv[++i]);
				if(QString(argv[i]) == "-bdu") app::conf.bdUsername = QString(argv[++i]);
				if(QString(argv[i]) == "-bdp") app::conf.bdPassword = QString(argv[++i]);
				if(QString(argv[i]) == "-u") app::conf.username = QString(argv[++i]);
				if(QString(argv[i]) == "-p") app::conf.password = QString(argv[++i]);
				if(QString(argv[i]) == "-l") app::conf.logFile = QString(argv[++i]);
				if(QString(argv[i]) == "-lvl") app::conf.logLevel = QString(argv[++i]).toInt();
				if(QString(argv[i]) == "-v") app::conf.verbose = true;
				if(QString(argv[i]) == "-clearTables") app::conf.clearTables = true;
			}
		}
		return ret;
	}

	void setLog(const uint8_t logLevel, const QString &mess)
	{
		if(app::conf.logLevel < logLevel or app::conf.logLevel == 0) return;

		QDateTime dt = QDateTime::currentDateTime();
		QString str = dt.toString("yyyy.MM.dd [hh:mm:ss] ") + mess + "\n";

		if( app::conf.verbose ){
			printf( "%s\n", str.toUtf8().data() );
			fflush( stdout );
		}

		if( app::conf.logFile.isEmpty() ) return;
		FILE* f;
		f = fopen(app::conf.logFile.toUtf8().data(),"a+");
		fwrite(str.toUtf8().data(),str.length(),1,f);
		fclose(f);
	}

}
