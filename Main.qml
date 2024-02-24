import QtQuick
import QtQuick.Controls.Material

ApplicationWindow{
    id: root

    visible: true

    // Width
    width: 785
    minimumWidth: 785
    maximumWidth: 785

    // Height
    height: 425
    minimumHeight: 425
    maximumHeight: 425

    // Theme
    Material.theme: Material.Dark
    Material.accent: "#039BE5"

    // Title
    title: qsTr("Video Quality Monitor 2.0.2")

    Column{
        anchors.fill: parent

        // File selection area
        Pane{
            // Size of file selection area.
            width: parent.width
            height: 85

            // Debug
            // Material.elevation: 6

            Column{
                width: parent.width
                height: parent.height

                // File path input box and analyse button
                Item{
                    id: file_parse_area
                    width: parent.width

                    TextField{
                        id: file_path

                        // Size
                        width: parent.width - parse.width - 15
                        placeholderText: qsTr("File Path")
                    }

                    Button{
                        id: parse

                        // Position
                        anchors.right: parent.right

                        // Theme
                        Material.background: "#039BE5"
                        Material.foreground: "#F5F5F5"

                        text: qsTr("Parse")

                        onClicked: {
                            // Debug
                            // Get the file path.
                            console.log("File path: ", file_path.text)
                            backend.getFilePath(file_path.text)
                            file_path.text = ""
                        }
                    }
                }
            }
        }

        // File size and video duration display area
        Pane{
            // Size
            width: parent.width
            height: 85

            // Debug
            // Material.elevation: 6

            Row{
                width: parent.width
                height: parent.height
                spacing: 15

                // File size
                TextField{
                    id: file_size
                    width: (parent.width - 45 - compute.width) / 2
                    placeholderText: qsTr("File Size (MB)")
                }

                // Video duration
                TextField{
                    id: video_duration
                    width: (parent.width - 45 - compute.width) / 2
                    placeholderText: qsTr("Video Duration (seconds)")
                }

                // Parse the parameters of the video.
                Connections{
                    target: backend
                    function onReturnFileParams(file_size_result, duration_result){
                        file_size.text = file_size_result
                        video_duration.text = duration_result
                    }
                }

                Button{
                    id: compute
                    text: qsTr("Compute")

                    // Theme
                    Material.background: "#039BE5"
                    Material.foreground: "#F5F5F5"

                    // Compute
                    onClicked: {
                        backend.compute(file_size.text, video_duration.text)
                    }
                }
            }
        }

        // Result display area
        Pane{
            // Size of result display area.
            width: parent.width
            height: parent.height - 85 - 85

            // Debug
            // Material.elevation: 6

            // Result display area
            Column{
                width: parent.width
                height: parent.height
                spacing: 15

                // Code ratio
                Text{
                    id: code_ratio

                    // Position
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: true
                    font.pixelSize: 24

                    // Theme
                    color: "White"

                    // Debug
                    // text: qsTr("码率：3.2Mbps")
                }

                // Result
                Text {
                    id: result

                    // Position
                    anchors.horizontalCenter: parent.horizontalCenter

                    // Theme
                    font.bold: true
                    font.pixelSize: 120
                    color: "Red"

                    // Debug
                    // text: qsTr("极佳")
                    Connections {
                        target: backend

                        function onReturnResult(compute_result, code_ratio_result) {
                            result.text = compute_result
                            code_ratio.text = code_ratio_result
                        }
                    }
                }
            }
            // Debug
            // Material.elevation: 6
        }
    }
}
