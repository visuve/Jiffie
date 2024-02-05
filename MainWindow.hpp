#pragma once

#include <QMainWindow>
#include <QStateMachine>

namespace Ui
{
	class MainWindow;
}

class FileListModel;
class JunkFileFinder;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

signals:
	void directorySelected();
	void searchStarted();
	void noResultsFound();
	void resultsFound();

private slots:
	void onAbout();
	void onOpenDirectoryDialog();
	void onStartStopSearch();
	void onProgress(const QString&);
	void onFinished();
	void onSelectAll();
	void onRemoveSelected();
	void onCreateFileContextMenu(const QPoint&);

private:
	void initJiffie();
	void initMenuBar();
	void initStateMachine();
	void openFileWithDefaultAssociation(const QString& filePath);
	void openParentDirectory(const QString& filePath);

	Ui::MainWindow* _ui;
	FileListModel* _model;
	JunkFileFinder* _jiffie;
	QStateMachine _machine;
};
