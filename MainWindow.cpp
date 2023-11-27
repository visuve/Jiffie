#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "JiffieVersion.h"
#include "FileListModel.hpp"
#include "JunkFileFinder.hpp"

#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	_ui(new Ui::MainWindow),
	_model(new FileListModel(this)),
	_jiffie(new JunkFileFinder(this))
{
	_ui->setupUi(this);

	_ui->statusBar->showMessage("Welcome to Jiffie!");

	initJiffie();
	initMenuBar();

	_ui->listViewFiles->setModel(_model);

	connect(_ui->pushButtonFindJunk, &QPushButton::clicked, this, &MainWindow::onStartSearch);
	connect(_ui->pushButtonSelectAll, &QPushButton::clicked, this, &MainWindow::onSelectAll);
	connect(_ui->pushButtonDeleteSelected, &QPushButton::clicked, this, &MainWindow::onRemoveSelected);
	connect(_ui->listViewFiles, &QListView::customContextMenuRequested, this, &MainWindow::onCreateFileContextMenu);
}

MainWindow::~MainWindow()
{
	delete _ui;
}

void MainWindow::openFileWithDefaultAssociation(const QString& filePath)
{
	const QUrl url = QUrl::fromLocalFile(filePath);

	if (!QDesktopServices::openUrl(url))
	{
		QMessageBox::warning(this, "Failed to open", "Failed to open file:\n\n" + filePath + "\n");
	}
}

void MainWindow::openParentDirectory(const QString& filePath)
{
	const QFileInfo fileInfo(filePath);

	if (!fileInfo.exists())
	{
		QMessageBox::warning(this, "Failed to open", "The file does not appear to exist:\n\n" + filePath + "\n");
		return;
	}

	const QUrl url = QUrl::fromLocalFile(fileInfo.dir().path());

	if (!QDesktopServices::openUrl(url))
	{
		QMessageBox::warning(this, "Failed to open", "Failed to open directory:\n\n" + filePath + "\n");
	}
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

void MainWindow::onOpenDirectoryDialog()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly, true);

	if (dialog.exec() == QFileDialog::Accepted)
	{
		_model->clear();
		const QString directory = dialog.selectedFiles().first();
		_ui->lineEditSelectedDirectory->setText(QDir::toNativeSeparators(directory));
	}
}

void MainWindow::onStartSearch()
{
	const QString selectedDirectory = _ui->lineEditSelectedDirectory->text();

	if (selectedDirectory.isEmpty())
	{
		onOpenDirectoryDialog();
		onStartSearch();
		return;
	}

	if (!QDir(selectedDirectory).exists())
	{
		QMessageBox::warning(this, "Invalid directory", '"' + selectedDirectory + '"' + " does not appear to exist!");
		onOpenDirectoryDialog();
		return;
	}

	const QString selectedWildcards = _ui->lineEditWildcards->text();

	if (selectedWildcards.isEmpty())
	{
		 QMessageBox::StandardButton reply = QMessageBox::warning(
			this,
			"No wildcards",
			"Having no wildcards will include all files in the selected directory.\nAre you sure you want to continue?",
			QMessageBox::Yes | QMessageBox::No);

		if (reply != QMessageBox::Yes)
		{
			return;
		}
	}

	_model->clear();
	_jiffie->setDirectory(selectedDirectory);
	_jiffie->setWildcards(selectedWildcards);
	_jiffie->start();

	const QString message = QString("%1 Jiffie started!").arg(QTime::currentTime().toString());
	_ui->statusBar->showMessage(message);
}

void MainWindow::onProgress(const QString& directoryPath)
{
	const QString message =
			QString("%1 Currently processing: %2")
			.arg(QTime::currentTime().toString())
			.arg(directoryPath);

	_ui->statusBar->showMessage(message);
}

void MainWindow::onFinished()
{
	if (_model->rowCount() <= 0)
	{
		QMessageBox::information(
			this,
			"No junk files",
			"No junk files were found.\n");
	}

	const QString message =
		QString("%1 Finished searching: %2")
			.arg(QTime::currentTime().toString())
			.arg(_ui->lineEditSelectedDirectory->text());

	_ui->statusBar->showMessage(message);
}

void MainWindow::onSelectAll()
{
	_model->selectAll();
}

void MainWindow::onRemoveSelected()
{
	const QStringList filePaths = _model->selectedPaths();

	if (filePaths.empty())
	{
		QMessageBox::warning(this, "Failed to remove file", "Nothing selected!\n");
		return;
	}

	if (QMessageBox::question(
			this,
			"Confirm delete?",
			"Are you sure you want to delete the following files:\n" + filePaths.join('\n')) !=
		QMessageBox::StandardButton::Yes)
	{
		return;
	}

	for (const QString& filePath : filePaths)
	{
		if (!QFile::remove(filePath))
		{
			if (QFile::exists(filePath))
			{
				QMessageBox::warning(this, "Failed to remove file", "Failed to remove:\n\n" + filePath + "\n");
				continue;
			}

			QMessageBox::warning(this, "Failed to remove file", filePath + "\n\ndoes not exist anymore!\n");
		}

		_model->removeFilePath(filePath);
	}
}

void MainWindow::onCreateFileContextMenu(const QPoint& pos)
{
	const QModelIndex selection = _ui->listViewFiles->indexAt(pos);

	if (!selection.isValid())
	{
		qDebug() << "Invalid selection";
		return;
	}

	const QVariant variant = _model->data(selection, Qt::DisplayRole);

	if (!variant.isValid())
	{
		qDebug() << "Invalid variant";
		return;
	}

	const QString filePath = variant.toString();

	if (filePath.isEmpty())
	{
		qDebug() << "File path is empty / no file selected";
		return;
	}

	auto openFileAction = new QAction("Open file", this);
	connect(openFileAction, &QAction::triggered, std::bind(&MainWindow::openFileWithDefaultAssociation, this, filePath));

	auto openParentDirAction = new QAction("Open parent directory", this);
	connect(openParentDirAction, &QAction::triggered, std::bind(&MainWindow::openParentDirectory, this, filePath));

	QMenu menu(this);
	menu.addActions({ openParentDirAction });
	menu.exec(_ui->listViewFiles->mapToGlobal(pos));
}

void MainWindow::initJiffie()
{
	connect(_jiffie, &JunkFileFinder::junkFound, _model, &FileListModel::addFilePath);
	connect(_jiffie, &JunkFileFinder::progress, this, &MainWindow::onProgress);
	connect(_jiffie, &JunkFileFinder::finished, this, &MainWindow::onFinished);
}

void MainWindow::initMenuBar()
{
	_ui->actionOpen->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
	connect(_ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenDirectoryDialog);

	_ui->actionExit->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
	connect(_ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);

	_ui->actionAbout->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
	connect(_ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

	_ui->actionLicenses->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton));
	connect(_ui->actionLicenses, &QAction::triggered, [this]()
	{
		QMessageBox::aboutQt(this, "Jiffie");
	});
}
