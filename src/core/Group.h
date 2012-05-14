/*
 *  Copyright (C) 2010 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSX_GROUP_H
#define KEEPASSX_GROUP_H

#include <QtCore/QPointer>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QPixmapCache>

#include "core/Database.h"
#include "core/Entry.h"
#include "core/TimeInfo.h"
#include "core/Uuid.h"

class Group : public QObject
{
    Q_OBJECT

public:
    enum TriState { Inherit, Enable, Disable };

    Group();
    ~Group();
    Uuid uuid() const;
    QString name() const;
    QString notes() const;
    QImage icon() const;
    QPixmap iconPixmap() const;
    int iconNumber() const;
    Uuid iconUuid() const;
    TimeInfo timeInfo() const;
    bool isExpanded() const;
    QString defaultAutoTypeSequence() const;
    Group::TriState autoTypeEnabled() const;
    Group::TriState searchingEnabled() const;
    Entry* lastTopVisibleEntry() const;

    void setUuid(const Uuid& uuid);
    void setName(const QString& name);
    void setNotes(const QString& notes);
    void setIcon(int iconNumber);
    void setIcon(const Uuid& uuid);
    void setTimeInfo(const TimeInfo& timeInfo);
    void setExpanded(bool expanded);
    void setDefaultAutoTypeSequence(const QString& sequence);
    void setAutoTypeEnabled(TriState enable);
    void setSearchingEnabled(TriState enable);
    void setLastTopVisibleEntry(Entry* entry);

    void setUpdateTimeinfo(bool value);

    Group* parentGroup();
    const Group* parentGroup() const;
    void setParent(Group* parent, int index = -1);

    Database* database();
    const Database* database() const;
    QList<Group*> children();
    const QList<Group*>& children() const;
    QList<Entry*> entries();
    const QList<Entry*>& entries() const;
    QList<Entry*> entriesRecursive(bool includeHistoryItems = false) const;
    QList<const Group*> groupsRecursive(bool includeSelf) const;

    QList<Entry*> search(const QString& searchTerm, Qt::CaseSensitivity caseSensitivity,
                         bool resolveInherit = true);

Q_SIGNALS:
    void dataChanged(Group* group);

    void aboutToAdd(Group* group, int index);
    void added();
    void aboutToRemove(Group* group);
    void removed();
    /**
     * Group moved within the database.
     */
    void aboutToMove(Group* group, Group* toGroup, int index);
    void moved();

    void entryAboutToAdd(Entry* entry);
    void entryAdded();
    void entryAboutToRemove(Entry* entry);
    void entryRemoved();

    void entryDataChanged(Entry* entry);

    void modified();

private:
    template <class P, class V> inline bool set(P& property, const V& value);

    void addEntry(Entry* entry);
    void removeEntry(Entry* entry);
    void setParent(Database* db);

    void recSetDatabase(Database* db);
    void cleanupParent();
    void recCreateDelObjects();
    void updateTimeinfo();
    bool includeInSearch(bool resolveInherit);

    QPointer<Database> m_db;
    Uuid m_uuid;
    QString m_name;
    QString m_notes;
    int m_iconNumber;
    Uuid m_customIcon;
    TimeInfo m_timeInfo;
    bool m_isExpanded;
    QString m_defaultAutoTypeSequence;
    TriState m_autoTypeEnabled;
    TriState m_searchingEnabled;
    QPointer<Entry> m_lastTopVisibleEntry;
    QList<Group*> m_children;
    QList<Entry*> m_entries;

    QPointer<Group> m_parent;
    QPixmapCache::Key m_pixmapCacheKey;

    bool m_updateTimeinfo;

    friend void Database::setRootGroup(Group* group);
    friend Entry::~Entry();
    friend void Entry::setGroup(Group* group);
};

#endif // KEEPASSX_GROUP_H
