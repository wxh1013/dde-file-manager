/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "recentfileinfo.h"
#include "dfileservices.h"
#include "dfilesystemmodel.h"
#include "private/dabstractfileinfo_p.h"

#include <QXmlStreamReader>
#include <QFile>

RecentFileInfo::RecentFileInfo(const DUrl &url)
    : DAbstractFileInfo(url)
{
    setProxy(DFileService::instance()->createFileInfo(nullptr, DUrl::fromLocalFile(url.path())));

    QFile file(QDir::homePath() + "/.local/share/recently-used.xbel");
    if (file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);

        while (!reader.atEnd()) {
            if (!reader.readNextStartElement() ||
                 reader.name() != "bookmark") {
                continue;
            }

            const QStringRef &location = reader.attributes().value("href");
            const QStringRef &added = reader.attributes().value("added");

            if (!location.isEmpty()) {
                DUrl findUrl = DUrl(location.toString());

                if (findUrl.toLocalFile() == url.path()) {
                    setReadDateTime(added.toString());
                    break;
                }
            }
        }
    }
}

bool RecentFileInfo::makeAbsolute()
{
    return true;
}

bool RecentFileInfo::exists() const
{
    if (fileUrl() == DUrl(RECENT_ROOT))
        return true;

    return DAbstractFileInfo::exists();
}

bool RecentFileInfo::isDir() const
{
    return fileUrl() == DUrl(RECENT_ROOT);
}

bool RecentFileInfo::isReadable() const
{
    return true;
}

bool RecentFileInfo::isWritable() const
{
    return false;
}

QFileDevice::Permissions RecentFileInfo::permissions() const
{
    if (fileUrl() == DUrl(RECENT_ROOT)) {
        return QFileDevice::ReadGroup | QFileDevice::ReadOwner | QFileDevice::ReadOther;
    }

    QFileDevice::Permissions p = DAbstractFileInfo::permissions();

    return p;
}

QVector<MenuAction> RecentFileInfo::menuActionList(DAbstractFileInfo::MenuType type) const
{
    Q_D(const DAbstractFileInfo);

    QVector<MenuAction> actions;

    if (type == SpaceArea) {
        actions << MenuAction::DisplayAs;
        actions << MenuAction::SortBy;
        actions << MenuAction::SelectAll;

        return actions;
    }

    actions = d->proxy->menuActionList(type);

    qDebug() << actions;

    actions.insert(1, MenuAction::OpenFileLocation);
    actions.insert(actions.indexOf(MenuAction::Delete), MenuAction::RemoveFromRecent);
    actions.removeOne(MenuAction::Cut);
    actions.removeOne(MenuAction::Rename);
    actions.removeOne(MenuAction::Delete);
    actions.removeOne(MenuAction::DecompressHere);

    return actions;
}

QSet<MenuAction> RecentFileInfo::disableMenuActionList() const
{
    Q_D(const DAbstractFileInfo);

    if (fileUrl() == DUrl(RECENT_ROOT))
        return {};

    return d->proxy->disableMenuActionList();
}

QList<int> RecentFileInfo::userColumnRoles() const
{
    static QList<int> userColumnRoles = QList<int>() << DFileSystemModel::FileDisplayNameRole
                                                     << DFileSystemModel::FileLastReadRole
                                                     << DFileSystemModel::FileSizeRole
                                                     << DFileSystemModel::FileMimeTypeRole;

    return userColumnRoles;
}

QVariant RecentFileInfo::userColumnData(int userColumnRole) const
{
    if (userColumnRole == DFileSystemModel::FileLastReadRole) {
        return m_lastReadTimeStr;
    }

    return DAbstractFileInfo::userColumnData(userColumnRole);
}

int RecentFileInfo::userColumnWidth(int userColumnRole, const QFontMetrics &fontMetrics) const
{
    if (userColumnRole == DFileSystemModel::FileNameRole)
        return -1;

    return DAbstractFileInfo::userColumnWidth(userColumnRole, fontMetrics);
}

DUrl RecentFileInfo::mimeDataUrl() const
{
    return DUrl::fromLocalFile(toLocalFile());
}

DUrl RecentFileInfo::parentUrl() const
{
    return DUrl(RECENT_ROOT);
}

QString RecentFileInfo::toLocalFile() const
{
    return fileUrl().path();
}

void RecentFileInfo::setReadDateTime(const QString &time)
{
    m_lastReadTime = QDateTime::fromString(time, Qt::ISODate).toLocalTime();
    m_lastReadTimeStr = m_lastReadTime.toString(dateTimeFormat());
}
