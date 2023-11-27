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
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void onAbout();
	void onOpenDirectoryDialog();
	void onStartSearch();
	void onProgress(const QString&);
	void onFinished();
	void onRemoveSelected();

private:
	void initJiffie();
	void initMenuBar();

	Ui::MainWindow* _ui;
	FileListModel* _model;
	JunkFileFinder* _jiffie;
};
