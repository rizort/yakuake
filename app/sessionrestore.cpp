/*
  SPDX-FileCopyrightText: 2026 Yakuake Contributors

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "sessionrestore.h"
#include "sessionstack.h"
#include "tabbar.h"

#include <KConfigGroup>
#include <KSharedConfig>

namespace SessionRestore
{
bool restore(SessionStack *sessions, TabBar *tabs)
{
    KConfigGroup restore(KSharedConfig::openConfig(), QStringLiteral("SessionRestore"));
    const int count = qBound(0, restore.readEntry("count", 0), 100);

    for (int index = 0; index < count; ++index) {
        KConfigGroup session(&restore, QString::number(index));
        const int type = session.readEntry("type", static_cast<int>(Session::Single));
        if (type < Session::Single || type > Session::Quad)
            continue;

        const SessionStack::PersistentState state{static_cast<Session::SessionType>(type),
                                                   session.readEntry("workingDirectory", QString()),
                                                   session.readEntry("closable", true),
                                                   session.readEntry("keyboardInputEnabled", true),
                                                   session.readEntry("monitorActivityEnabled", false),
                                                   session.readEntry("monitorSilenceEnabled", false)};
        const int sessionId = sessions->addRestoredSession(state);

        if (session.readEntry("customTitle", false))
            tabs->setTabTitle(sessionId, session.readEntry("title", QString()));
    }

    if (sessions->activeSessionId() == -1)
        return false;

    const int activeTab = qBound(0, restore.readEntry("activeTab", 0), tabs->tabCount() - 1);
    sessions->raiseSession(tabs->sessionAtTab(activeTab));
    return true;
}

void save(SessionStack *sessions, TabBar *tabs, bool enabled)
{
    const auto config = KSharedConfig::openConfig();
    KConfigGroup(config, QStringLiteral("SessionRestore")).deleteGroup();
    if (!enabled) {
        config->sync();
        return;
    }

    KConfigGroup restore(config, QStringLiteral("SessionRestore"));
    restore.writeEntry("count", tabs->tabCount());

    int activeTab = 0;
    for (int index = 0; index < tabs->tabCount(); ++index) {
        const int sessionId = tabs->sessionAtTab(index);
        const auto state = sessions->persistentState(sessionId);
        KConfigGroup session(&restore, QString::number(index));
        session.writeEntry("type", static_cast<int>(state.type));
        session.writeEntry("workingDirectory", state.workingDirectory);
        session.writeEntry("closable", state.closable);
        session.writeEntry("keyboardInputEnabled", state.keyboardInputEnabled);
        session.writeEntry("monitorActivityEnabled", state.monitorActivityEnabled);
        session.writeEntry("monitorSilenceEnabled", state.monitorSilenceEnabled);
        session.writeEntry("customTitle", tabs->tabTitleIsInteractive(sessionId));
        if (tabs->tabTitleIsInteractive(sessionId))
            session.writeEntry("title", tabs->tabTitle(sessionId));
        if (sessionId == sessions->activeSessionId())
            activeTab = index;
    }

    restore.writeEntry("activeTab", activeTab);
    config->sync();
}
}
