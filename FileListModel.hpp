#pragma once

#include <QAbstractListModel>
#include <QMap>

#include <functional>

class FileListModel : public QAbstractListModel
{
	Q_OBJECT

public:
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

	explicit FileListModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void clear();
	void selectAll();
	bool hasSelection() const;
	void removeIf(std::function<bool(const FileItem&)>);

public slots:
	void addFilePath(const QString& filePath);

private:
	QList<FileItem> _files;
};
