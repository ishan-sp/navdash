import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Item {
    width: parent.width
    height: parent.height

    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter {
            name: "osm.useragent"
            value: "MyGPSTracker"
        }
        PluginParameter {
            name: "osm.mapping.custom.host"
            value: "https://tile.openstreetmap.org/"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        zoomLevel: 10
        center: QtPositioning.coordinate(17.385044, 78.486671)
        copyrightsVisible: false

        MapPolyline {
            id: trackLine
            line.width: 3
            line.color: 'red'
            path: []
        }

        Component.onCompleted: {
            map.zoomLevel = 10
        }
    }

    // Follow mode toggle
    Rectangle {
        id: followButton
        width: 120
        height: 40
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 15
        color: followMode ? "#8800ff00" : "#88ffffff"
        border.color: "black"
        border.width: 2
        radius: 20

        property bool followMode: true

        Text {
            anchors.centerIn: parent
            text: parent.followMode ? "Following" : "Free View"
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.followMode = !parent.followMode
            }
        }
    }

    // Navigation controls (only visible in free view mode)
    Grid {
        id: navigationControls
        columns: 3
        spacing: 5
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 15
        visible: !followButton.followMode

        Button {
            width: 40
            height: 40
            enabled: false
        }

        Button {
            text: "↑"
            width: 40
            height: 40
            onClicked: {
                var coord = map.center
                coord.latitude += 0.01
                map.center = coord
            }
        }

        Button {
            width: 40
            height: 40
            enabled: false
        }

        Button {
            text: "←"
            width: 40
            height: 40
            onClicked: {
                var coord = map.center
                coord.longitude -= 0.01
                map.center = coord
            }
        }

        Button {
            text: "●"
            width: 40
            height: 40
            enabled: false
        }

        Button {
            text: "→"
            width: 40
            height: 40
            onClicked: {
                var coord = map.center
                coord.longitude += 0.01
                map.center = coord
            }
        }

        Button {
            width: 40
            height: 40
            enabled: false
        }

        Button {
            text: "↓"
            width: 40
            height: 40
            onClicked: {
                var coord = map.center
                coord.latitude -= 0.01
                map.center = coord
            }
        }

        Button {
            width: 40
            height: 40
            enabled: false
        }
    }

    // Zoom controls
    Column {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 15
        spacing: 10

        Button {
            text: "+"
            width: 40
            height: 40
            onClicked: {
                var newZoom = Math.min(map.maximumZoomLevel, map.zoomLevel + 1)
                map.zoomLevel = newZoom
            }
        }

        Button {
            text: "-"
            width: 40
            height: 40
            onClicked: {
                var newZoom = Math.max(map.minimumZoomLevel, map.zoomLevel - 1)
                map.zoomLevel = newZoom
            }
        }
    }

    function addTrackPoint(lat, lon) {
        var coordinate = QtPositioning.coordinate(lat, lon)
        var path = trackLine.path
        path.push(coordinate)
        trackLine.path = path

        // Only center the map if in follow mode
        if (followButton.followMode) {
            map.center = coordinate
        }
    }
}
