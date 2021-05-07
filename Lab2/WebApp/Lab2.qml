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
		appID: "60958e5407e68bc0d8fcbb2f"
		apiKey: "353a38b73ed3daff04070c49dfc1d77bf478ee045818d635c1ed1cefa68f9b74"
	
		Component.onCompleted: {
			connectServer()
		}
	
		onConnectionEstablished: {
			console.log("Connection established")
			// subscribe("topicToSubscribe")
		}
	
		onCredentialsRejected: {
			console.log("Provided credentials were rejected by the server")
		}
	
		onMessageRecieved: {
			console.log(message)
			let parsedMessage = JSON.parse(message)
			// console.log(parsedMessage.topic)
			// console.log(parsedMessage.message)
		}
	}
	
	Dial {
		value: 0
		from: 0
		to: 100

		onValueChanged: {
			// this is a javascript function provided as callback for on value changed event
			console.log(`${Math.round(value)}`)  // access to this dial value property
			client.publish("60958e5407e68bc0d8fcbb2f/pwm", `${Math.round(value)}`)
		}

		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		anchors.topMargin: parent.height / 4
	}
}