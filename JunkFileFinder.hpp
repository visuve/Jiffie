#pragma once

#include <QThread>

class JunkFileFinder : public QThread
{
	Q_OBJECT

public:
	JunkFileFinder(QObject* parent);
	~JunkFileFinder();

	void setDirectory(const QString& directory);
	void setWildcards(const QString& wildcards);

signals:
	void junkFound(const QString& filePath);
	void progress(const QString& directoryPath) const;

private:
	bool keepRunning() const;
	void run() override;
	void reportProgress() const;

	QString _directory;
	QStringList _wildcards;
};

