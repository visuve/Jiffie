#include "JunkFileFinder.hpp"

#include <QDebug>
#include <QDir>
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
	QDirIterator it(_directory, _wildcards, QDir::Files, QDirIterator::Subdirectories);

	while (keepRunning() && it.hasNext())
	{
		const QString path = QDir::toNativeSeparators(it.next());

		emit junkFound(path);
	}
}
