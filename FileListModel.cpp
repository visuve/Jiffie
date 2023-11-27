#include "FileListModel.hpp"

FileListModel::FileListModel(QObject* parent) :
	QAbstractListModel(parent)
{
	_files.emplaceBack(Qt::CheckState::Checked, "Example 1");
	_files.emplaceBack(Qt::CheckState::Unchecked, "Example 2");
	_files.emplaceBack(Qt::CheckState::Checked, "Example 3");
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
