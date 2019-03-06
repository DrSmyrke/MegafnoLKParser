#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QtWebKit>
#include <QtWebKitWidgets/QWebPage>
#include <QAction>
#include <QMenu>

class WebPage : public QWebPage
{
	Q_OBJECT
public:
	explicit WebPage(QObject *parent = 0);
};

#endif // WEBPAGE_H
