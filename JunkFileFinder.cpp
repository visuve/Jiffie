#include "JunkFileFinder.hpp"

#include <QDebug>
#include <QDirIterator>

JunkFileFinder::JunkFileFinder(QObject* parent) :
	QThread(parent)
{
}

JunkFileFinder::~JunkFileFinder()
{
	qDebug() << "Destroying...";
	requestInterruption();
	wait();
	qDebug() << "Destroyed.";
}

void JunkFileFinder::setDirectory(const QString& directory)
{
	_directory = directory;
}

void JunkFileFinder::setWildcards(const QString& wildcards)
{
	if (wildcards.isEmpty())
	{
		_wildcards.clear();
	}
	else
	{
		_wildcards = wildcards.split('|');
	}
}

bool JunkFileFinder::keepRunning() const
{
	return QThread::currentThread()->isInterruptionRequested() == false;
}

void JunkFileFinder::run()
{
	QDirIterator iter(_directory, _wildcards, QDir::Files, QDirIterator::Subdirectories);

	QString currentDirectory;

	while (keepRunning() && iter.hasNext())
	{
		const QFileInfo fileInfo = iter.nextFileInfo();
		const QString directoryPath = fileInfo.path();
		const QString filePath = fileInfo.filePath();

		emit junkFound(filePath);

		if (currentDirectory != directoryPath)
		{
			emit progress(directoryPath);
			currentDirectory = directoryPath;
		}
	}
}
