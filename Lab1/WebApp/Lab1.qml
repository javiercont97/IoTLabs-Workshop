import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import Controls 1.0
import IMT.IoTLabsWS 1.0
// Styling imports
import QtQuick.Controls.Material 2.12


Item {
	id: rootItem
	IoTLabsWS {
		id: client
		// host: "wss://imt-iotlabs.net"
		host: "ws://192.168.0.7"
		appID: "60958e4507e68bc0d8fcbb2e"
		apiKey: "37089b28212609534d275732fbbd8923874a11417ffd682b8b6ee6ebca2774b6"
	
		Component.onCompleted: {
			connectServer()
		}
	
		onConnectionEstablished: {
			console.log("Connection was established")
			subscribe("60958e4507e68bc0d8fcbb2e/led/state")   // subscribe to login responses
		}
	
		onCredentialsRejected: {
			console.log("Provided credentials were rejected by the server")
		}
	
		onMessageRecieved: {
			console.log(message)
			let parsedMessage = JSON.parse(message)
			if (parsedMessage.topic == "60958e4507e68bc0d8fcbb2e/led/state") {
				led.ledState = parsedMessage.message == "ON"? true: false
				ledSwitch.checked = parsedMessage.message == "ON"? true: false
			}
			// console.log(parsedMessage.topic)
			// console.log(parsedMessage.message)
		}
	}

	Switch {
		id: ledSwitch
		text: checked? "LED off": "LED on"
		onCheckedChanged: {
			console.log(`Switch toggled`)
			// "checked" property holds switch state

			client.publish("60958e4507e68bc0d8fcbb2e/led/set", checked? "ON": "OFF")
			// led.ledState = !led.ledState
		}

		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		anchors.topMargin: parent.height / 5
	}

	Rectangle {
		id: led
		width: 50
		height: 50
		property bool ledState: false
		color: ledState? "blue": "#000033"
		radius: 25

		anchors.right: ledSwitch.left
		anchors.rightMargin: 25
		anchors.verticalCenter: ledSwitch.verticalCenter
	}
}