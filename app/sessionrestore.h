/*
  SPDX-FileCopyrightText: 2026 Yakuake Contributors

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

class SessionStack;
class TabBar;

namespace SessionRestore
{
bool restore(SessionStack *sessions, TabBar *tabs);
void save(SessionStack *sessions, TabBar *tabs, bool enabled);
}
