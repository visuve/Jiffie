#include "FileListModel.hpp"

FileListModel::FileListModel(QObject* parent) :
	QAbstractListModel(parent)
{
}

int FileListModel::rowCount(const QModelIndex& parent) const
{
	return _files.size();
}

bool FileListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole)
	{
		FileItem& item = _files[index.row()];

		if (item.state != value)
		{
			item.state = value;
			emit dataChanged(index, index, { role });
			return true;
		}
	}

	return false;
}

QVariant FileListModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid() && index.column() == 0)
	{
		const FileItem& item = _files.at(index.row());

		switch (role)
		{
			case Qt::CheckStateRole:
				return item.state;
			case Qt::DisplayRole:
				return item.path;
		}
	}

	return QVariant();
}

Qt::ItemFlags FileListModel::flags(const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 0)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
	}

	return Qt::NoItemFlags;
}

void FileListModel::clear()
{
	beginResetModel();
	_files.clear();
	endResetModel();
}

void FileListModel::addFilePath(const QString& filePath)
{
	beginInsertRows(QModelIndex(), 0, 1);
	_files.emplaceBack(Qt::CheckState::Unchecked, filePath);
	endInsertRows();
}

QStringList FileListModel::selectedPaths() const
{
	QStringList result;

	for (const FileItem& item : _files)
	{
		if (item.state == Qt::CheckState::Checked)
		{
			result << item.path;
		}
	}

	return result;
}

void FileListModel::removeFilePath(const QString& filePath)
{
	beginResetModel();

	_files.removeIf([=](const FileItem& item)
	{
		return item.path == filePath;
	});

	endResetModel();
}
