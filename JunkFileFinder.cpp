#include "JunkFileFinder.hpp"

#include <QDebug>
#include <QDirListing>

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
	QString currentDirectory;

	// The QDirListing is still very slow with wildcards
	QDirListing listing(_directory, _wildcards,
		QDirListing::IteratorFlag::FilesOnly |
		QDirListing::IteratorFlag::Recursive |
		QDirListing::IteratorFlag::IncludeHidden);

	for (const auto& entry: listing)
	{
		if (!keepRunning())
		{
			qDebug() << "Aborted";
			return;
		}

		const QString directoryPath = entry.absolutePath();
		const QString filePath = entry.absoluteFilePath();

		emit junkFound(filePath);

		if (currentDirectory != directoryPath)
		{
			emit progress(directoryPath);
			currentDirectory = directoryPath;
		}
	}

	qDebug() << "Finished";
}
