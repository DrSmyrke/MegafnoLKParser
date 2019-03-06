#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QAction>
#include <QtWebKit>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include "webpage.h"

class WebView : public QWebView
{
	Q_OBJECT
public:
	explicit WebView(QWidget *parent = 0);
signals:
	void signal_linkHovered(const QString &link);
	void signal_createWindow(WebView** webView);
	void signal_printRequested(QWebFrame * frame);
	void signal_goToAbout();
	void signal_hideInspector();
private slots:
	void slot_downloadRequested(const QNetworkRequest &request);
private:
	WebPage* m_pWebPage;
protected:
	QWebView *createWindow(QWebPage::WebWindowType type) override;
	void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // WEBVIEW_H
