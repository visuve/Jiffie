#pragma once

#include <QAbstractListModel>
#include <QMap>

class FileListModel : public QAbstractListModel
{
	Q_OBJECT

	struct FileItem
	{
		inline FileItem(const QVariant& state, const QString& path) :
			state(state),
			path(path)
		{
		}

		QVariant state;
		QString path;
	};

public:
	explicit FileListModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

public slots:
	void clear();
	void addFilePath(const QString& filePath);
	void selectAll();
	QStringList selectedPaths() const;
	void removeFilePath(const QString&);

private:
	QList<FileItem> _files;
};
