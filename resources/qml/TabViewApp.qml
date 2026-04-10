/****************************************************************************
**
** QuiteRSS is a open-source cross-platform news feed reader
** Copyright (C) 2011-2018 QuiteRSS Team <quiterssteam@gmail.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/
import QtQuick 2.8
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtWebEngine 1.3

Rectangle {
    id: tabBar
    width: parent.width
    height: parent.height
    color: "#f8f8f8"
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: tabBarControl
            Layout.fillWidth: true

            TabButton {
                text: "Feed 1"
            }
            TabButton {
                text: "Feed 2"
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBarControl.currentIndex

            WebEngineView {
                url: "https://google.com"
            }
            WebEngineView {
                url: "https://google.com"
            }
        }
    }
}
