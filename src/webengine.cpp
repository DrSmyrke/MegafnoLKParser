#include "webengine.h"
#include <QApplication>
#include <QJsonObject>

WebEngine::WebEngine(QObject *parent) : QObject(parent)
{
	db = QSqlDatabase::addDatabase("QMYSQL");

	m_pTimer = new QTimer(this);
		m_pTimer->setSingleShot(true);
		m_pTimer->setInterval(3000);

	m_pWebView = new WebView();
	m_pWebView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
	m_pWebView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages,true);

	m_pWebView->hide();

	connect(m_pWebView,&WebView::loadFinished,this,&WebEngine::slot_loadFinished);
	connect(m_pWebView,&WebView::loadProgress,this,[this](int prz){
		app::setLog(5,QString("Loading %1").arg(prz));
		if(prz == 100){
			//qDebug()<<m_pWebView->page()->currentFrame()->toPlainText();
			m_pTimer->start(1000);
		}
	});
	connect(m_pTimer,&QTimer::timeout,this,&WebEngine::slot_timerTimeout);
}

bool WebEngine::exec()
{
	db.setHostName( app::conf.bdAddress );
	db.setDatabaseName( app::conf.bdName );
	db.setUserName( app::conf.bdUsername );
	db.setPassword( app::conf.bdPassword );

	QTimer::singleShot(10000, this, [this](){
		if( !db.isOpen() ){
			app::setLog(1,QString("Connecting timeout [%1]").arg(db.lastError().text()));
			m_pWebView->deleteLater();
			QApplication::quit();
		}
	});

	app::setLog(3,QString("Connect to bd [%1][%2]").arg(app::conf.bdAddress).arg(app::conf.bdName));

	bool ok = db.open();
	if( !ok ){
		app::setLog(1,QString("Cannot connect to bd [%1]").arg(db.lastError().text()));
		return false;
	}else{
		app::setLog(3,QString("Connect to bd OK"));

		if( app::conf.clearTables ){

			QSqlQuery query(db);
			query.exec("TRUNCATE TABLE megafno_company");
			query.exec("TRUNCATE TABLE megafno_phones");
			app::setLog(1,QString("Cleaning tables in bd [%1]").arg(app::conf.bdName));
			db.close();
			m_pWebView->deleteLater();
			return false;
		}

		m_pWebView->load(QUrl("https://szf.b2blk.megafon.ru/b2b/login"));
	}

	return true;
}

void WebEngine::slot_loadFinished()
{
	//qDebug()<<m_pWebView->page()->mainFrame()->toHtml();
}

void WebEngine::slot_timerTimeout()
{
	static uint8_t action = 0;
	app::setLog(4,QString("action %1").arg(action));

	switch (action++) {
		case 0:
			m_pWebView->page()->mainFrame()->evaluateJavaScript("var evt = document.createEvent('HTMLEvents');evt.initEvent('change', true, false);");
			m_pWebView->page()->mainFrame()->evaluateJavaScript("var obj = document.getElementById('username'); obj.value='" + app::conf.username + "'; obj.dispatchEvent(evt);");
			m_pWebView->page()->mainFrame()->evaluateJavaScript("var obj = document.getElementById('password'); obj.value='" + app::conf.password + "'; obj.dispatchEvent(evt);");
			m_pWebView->page()->mainFrame()->evaluateJavaScript("var objs = document.getElementsByTagName('BUTTON'); objs[0].click();");
			//
		break;
		case 1:
			indexParser( m_pWebView->page()->currentFrame()->toPlainText() );
			m_pTimer->start(1000);
		break;
		case 2:
			m_pWebView->load(QUrl("https://szf.b2blk.megafon.ru/b2b/subscriber/mobile"));
		break;
		case 3:
			m_pWebView->load(QUrl("https://szf.b2blk.megafon.ru/b2b/subscriber/mobile/list?from=0&size=1000"));
		break;
		case 4:
			phoneParser( m_pWebView->page()->currentFrame()->toPlainText() );
			m_pTimer->start(1000);
		break;
		case 5:
			m_pWebView->load(QUrl("https://szf.b2blk.megafon.ru/b2b/logout"));
		break;
		case 6:
			db.close();
			m_pWebView->deleteLater();
			QApplication::quit();
		break;
	}
}

void WebEngine::indexParser(const QString &string)
{
	QSqlQuery query(db);
	query.prepare("INSERT INTO megafno_company (companyID, ballance, prewUsersMoneyCount) VALUES (:companyID, :ballance, :prewUsersMoneyCount)");
	query.bindValue(":companyID", app::conf.username);

	bool find = false;
	QString strParam;

	auto tmp = string.split("\n");
	for( auto str:tmp ){
		if( str.contains( "Текущий баланс", Qt::CaseInsensitive ) ){
			strParam = str;
			find = true;
			continue;
		}
		if( str.contains( "Начислено по всем абонентам с начала периода", Qt::CaseInsensitive ) ){
			strParam = str;
			find = true;
			continue;
		}
		if( find ){
			if( strParam.contains( "Текущий баланс", Qt::CaseInsensitive ) ) query.bindValue(":ballance", str);
			if( strParam.contains( "Начислено по всем абонентам с начала периода", Qt::CaseInsensitive ) ) query.bindValue(":prewUsersMoneyCount", str);
			app::setLog(3,QString("indexParser: %1 %2").arg(strParam).arg(str));
			find = false;
			continue;
		}
	}

	query.exec();
}

void WebEngine::phoneParser(const QString &string)
{
	QSqlQuery query(db);

	QJsonDocument jsonResponse = QJsonDocument::fromJson( string.toUtf8() );
	QJsonObject jsonObj = jsonResponse.object();

	app::setLog(3,QString("Total phone count: %1").arg(jsonObj["count"].toInt()));

	for( auto elem:jsonObj["mobile"].toArray() ){
		auto number = elem.toObject()["msisdn"].toString();
		auto user = elem.toObject()["name"].toString();
		auto ratePlan = elem.toObject()["ratePlan"].toObject()["def"].toString();
		auto lockStatus = elem.toObject()["lockStatusDict"].toObject()["def"].toString();

		query.exec("INSERT INTO megafno_phones (companyID, number, user, ratePlan, lockStatus)  VALUES ('" + app::conf.username + "', '" + number + "', '" + user + "', '" + ratePlan + "', '" + lockStatus + "')");

		app::setLog(3,QString("%1 %2 %3 %4").arg(number).arg(user).arg(ratePlan).arg(lockStatus));
	}
}
