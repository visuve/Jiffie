#pragma once

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class FileListModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void onAbout();

private:
	Ui::MainWindow* _ui;
	FileListModel* _model;
};
