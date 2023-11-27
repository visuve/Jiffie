#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "JiffieVersion.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->statusBar->showMessage("Welcome to Jiffie!");

	ui->actionExit->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
	connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);

	ui->actionAbout->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

	ui->actionLicenses->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton));
	connect(ui->actionLicenses, &QAction::triggered, [this]()
	{
		QMessageBox::aboutQt(this, "Jiffie");
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onAbout()
{
	const QString commitUrl =
		QString("<a href='https://github.com/visuve/Jiffie/commit/%1'>%1</a>").arg(JIFFIE_COMMIT_HASH);

	const QString licenseUrl =
		QString("<a href='https://github.com/visuve/Jiffie/blob/%1/LICENSE.md'>LICENSE.md</a>").arg(JIFFIE_COMMIT_HASH);

	QStringList text;
	text << "<h3>Jiffie - Junk File Finder</h3>";
	text << "<p>Version " + QString(JIFFIE_VERSION) + ".</p>";
	text << "<p>Jiffie is yet another junk file finder.</p>";
	text << "<p>Jiffie is open source (GPLv2) and written in C++ and uses Qt framework.</p>";
	text << "<p>See Licenses menu and " << licenseUrl << " for more details.</p>";
	text << "<p>Git commit hash this build is from: " << commitUrl << "</p>";

	QMessageBox::about(this, "Jiffie", text.join('\n'));
}
