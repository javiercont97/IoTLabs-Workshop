import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import Controls 1.0

// Styling imports
import QtQuick.Controls.Material 2.12


Item {
    id:joyStick;
    property int offset:30;


    signal pressed();
    signal released();
    signal positionReleased(double joyX, double joyY);

    property double xOutput: 0
    property double yOutput: 0

    property double pollingRate: 5

    Rectangle {
        id:totalArea
        // color: "gray"
        color: Material.primary
        radius: parent.width/2
        opacity: 0.5
        width: parent.width
        height: parent.height
    }

    Rectangle{
        id: stick
        width: totalArea.width/2
        height: width
        radius: width/2
        x: totalArea.width/2 - radius;
        y: totalArea.height/2 - radius;
        color: Material.foreground
        // color: joyStick.enabled? "black" : "darkgray"
    }

    // The timer will send Joystick position every {pollingRate} milliseconds
    Timer {
        interval: pollingRate
        running: true
        repeat: true
        onTriggered: {
            joyStick.positionReleased(xOutput, yOutput)
        }
    }

    MouseArea{
        id:mouseArea
        anchors.fill: parent

        onPressed: {
            joyStick.pressed();
        }

        onPositionChanged: {            
            // It is known that (x-center_x)^2 + (y - center_y)^2 < radius^2
            // Then if stick need to remain inside larger circle
            // var rad = (totalArea.radius - stick.radius);
            // Else if stick can go outside larger circle
            var rad = totalArea.radius;
            rad =  rad * rad;

            let centerX = totalArea.x + totalArea.radius;
            let centerY = totalArea.y + totalArea.radius;

            if ( mouseX < 0 || mouseX > totalArea.x + totalArea.width ) {
                return;
            }
            if ( mouseY < 0 || mouseY > totalArea.y + totalArea.height ) {
                return;
            }

            // calculate distance in x direction
            var xDist = mouseX - centerX;
            // var xDist = stick.x - (totalArea.x + totalArea.radius);

            // calculate distance in y direction
            var yDist = mouseY - centerY;
            // var yDist = stick.y - (totalArea.y + totalArea.radius);


            if( (xDist*xDist + yDist*yDist < rad/6 ) ){
                // No sensi
                xOutput = 0;
                yOutput = 0;
            } else if (xDist > 0 && yDist >= -xDist/4 && yDist < xDist/4 ) {
                // Right
                xOutput = 1;
                yOutput = 0;
            } else if (xDist > 0 && yDist >= xDist/4 && yDist < 3*xDist/2) {
                // Bottom right
                xOutput = 1;
                yOutput = -1;
            } else if (yDist > 0 && yDist >= 3*xDist/2 && yDist > -3*xDist/2) {
                // Bottom
                xOutput = 0;
                yOutput = -1;
            } else if (xDist < 0 && yDist <= -3*xDist/2 && yDist > -xDist/4) {
                // Bottom left
                xOutput = -1;
                yOutput = -1;
            } else if (xDist < 0 && yDist <= -xDist/4 && yDist > xDist/4) {
                // Left
                xOutput = -1;
                yOutput = 0;
            } else if (xDist < 0 && yDist <= xDist/4 && yDist > 3*xDist/2) {
                // Top left
                xOutput = -1;
                yOutput = 1;
            } else if (yDist < 0 && yDist <= 3*xDist/2 && yDist < -3*xDist/2) {
                // Top
                xOutput = 0;
                yOutput = 1;
            } else {
                // Top right
                xOutput = 1;
                yOutput = 1;
            }

            // console.log(yDist >= xDist/4 && yDist <= xDist/2)

            // xOutput = (xDist == 0? 0: xDist > 0? 1: -1);
            // yOutput = (yDist == 0? 0: yDist > 0? 1: -1);


            xDist = xDist * xDist;
            yDist = yDist * yDist;

            //total distance for inner circle
            var dist = xDist + yDist;

            // if distance if less than radius then inner circle is inside larger circle
            // Prevet inner circle from going outside the outer circle
            if( rad < dist ) {
                return;
            }

            // center of larger circle
            var oldX = stick.x; var oldY = stick.y;
            stick.x = mouseX - stick.radius;
            stick.y = mouseY - stick.radius;
        }

        onReleased: {
            //snap to center
            stick.x = totalArea.width / 2 - stick.radius;
            stick.y = totalArea.height / 2 - stick.radius;

            xOutput = 0
            yOutput = 0

            joyStick.released();
        }
    }
}