#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <QObject>
#include <QWidget>
#include <QtWebKit>
#include <QtWebKitWidgets/QWebFrame>
#include <QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "global.h"
#include "WebKit/webview.h"

class WebEngine : public QObject
{
	Q_OBJECT
public:
	explicit WebEngine(QObject *parent = nullptr);
	bool exec();
private slots:
	void slot_loadFinished();
	void slot_timerTimeout();
private:
	WebView* m_pWebView;
	QTimer* m_pTimer;
	QSqlDatabase db;

	void indexParser( const QString &string );
	void phoneParser( const QString &string );
};

#endif // WEBENGINE_H
