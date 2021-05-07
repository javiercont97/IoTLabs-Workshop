import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import Controls 1.0
import IMT.IoTLabsWS 1.0
// Styling imports
import QtQuick.Controls.Material 2.12


Item {
	id: rootItem

	// Material.theme: Material.Dark
	// Material.accent: Material.Orange
	// Material.background: Material.Red
	// Material.primary: Material.Orange
	// Material.primary: Material.Dark
	// Material.foreground: Material.Red
	// Material.foreground: "red"

	Material.primary: "#3b3b3d"
	Material.foreground: "#000000"


	IoTLabsWS {
		id: client
		// host: "wss://imt-iotlabs.net"
		host: "ws://192.168.0.7"
		appID: "60958e6107e68bc0d8fcbb30"
		apiKey: "a44e4c69ff4316b1ab34e616ce7abf36b439a159f39bf3f7ff0e23968d4ec7a0"
	
		Component.onCompleted: {
			connectServer()
		}
	
		onConnectionEstablished: {
			console.log("Connection established")
		}
	
		onCredentialsRejected: {
			console.log("Provided credentials were rejected by the server")
		}
	
		onMessageRecieved: {
			// let parsedMessage = JSON.parse(message)
		}
	}

	Joystick {
		id: joystick
		// width: parent.height > parent.width? parent.width / 2 : parent.height / 2
		// height: parent.height > parent.width? parent.width / 2 : parent.height / 2
		width: 350
		height: 350

		anchors.centerIn: parent

		// get current position every 13.33[ms] or 10[Hz]
		pollingRate: 200
		// pollingRate: 1000

		onPositionReleased: {
			console.log(`out: (${joyX}, ${joyY})`)
			client.publish("60958e6107e68bc0d8fcbb30/joystick_X", joyX);
			client.publish("60958e6107e68bc0d8fcbb30/joystick_Y", joyY);
		}
	}
}