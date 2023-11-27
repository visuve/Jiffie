#pragma once

#include <QMainWindow>

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

private slots:
	void onAbout();
	void onOpenDirectoryDialog();
	void onStartSearch();
	void onProgress(const QString&);
	void onFinished();
	void onRemoveSelected();
	void onCreateFileContextMenu(const QPoint&);

private:
	void initJiffie();
	void initMenuBar();
	void openFileWithDefaultAssociation(const QString& filePath);
	void openParentDirectory(const QString& filePath);

	Ui::MainWindow* _ui;
	FileListModel* _model;
	JunkFileFinder* _jiffie;
};
