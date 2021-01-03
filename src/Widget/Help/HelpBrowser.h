#ifndef WIDGET_HELPBROWSER_H
#define WIDGET_HELPBROWSER_H

#include <QTextBrowser>
#include <QHelpEngine>

class HelpBrowser : public QTextBrowser
{
	public:
		HelpBrowser(QHelpEngine* helpEngine, QWidget* parent = 0);
		QVariant loadResource (int type, const QUrl& name);
	private:
		QHelpEngine* helpEngine;
};

#endif // WIDGET_HELPBROWSER_H
