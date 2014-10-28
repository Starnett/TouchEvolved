#include <iostream>
#include <string.h>
#include "Leap.h"
#include "common.h"
#include "LeapControl.h"
#include <time.h>

extern void myKeyHandler(unsigned char, int, int);
extern void myDisplay(void);
extern void resetCamera(void);

using namespace Leap;
using namespace std;

string fingers[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
string bones[] = { "MetaCarpal", "Proximal", "Middle", "Distal" };
string state[] = { "INVALID", "STARTING", "UPDATING", "ENDING" };

Vector swipeStartPos;
Vector swipeEndPos;

//time_t lastGestTime;
//time(&lastGestTime);

const int SWIPE_MIN_LENGTH = 100;
const int SWIPE_MIN_VELOCITY = 500;


void LeapListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void LeapListener::onConnect(const Controller& controller) {
	cout << "Connected\n";
	//For testing purposes, only enable swipes for now.

	//controller.enableGesture(Gesture::TYPE_CIRCLE);
	//controller.enableGesture(Gesture::TYPE_KEY_TAP);
	//controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);

	//change the minimum swipe velocity and distance to a smaller interval so that the gesture doesn't 
	//need to be so rapid
	controller.config().setFloat("Gesture.Swipe.MinLength", SWIPE_MIN_LENGTH);
	controller.config().setFloat("Gesture.Swipe.MinVelocity", SWIPE_MIN_VELOCITY);
	controller.config().save();
}

void LeapListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	cout << "Disconnected\n";
}

void LeapListener::onExit(const Controller& controller) {
	cout << "Exited\n";
}


void LeapListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	//std::cout << "Frame id: " << frame.id()<< ", timestamp: " << frame.timestamp()<< ", hands: " << frame.hands().count()<< ", fingers: " << frame.fingers().count()<< ", tools: " << frame.tools().count() << ", gestures: " << frame.gestures().count() << std::endl;

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		// std::cout << std::string(2, ' ') << handType << ", id: " << hand.id() << ", palm position: " << hand.palmPosition() << std::endl;
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		// Calculate the hand's pitch, roll, and yaw angles
		//std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;

		// Get the Arm bone
		//Arm arm = hand.arm();
		//std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()<< " wrist position: " << arm.wristPosition()<< " elbow position: " << arm.elbowPosition() << std::endl;

		// Get fingers
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			//std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]<< " finger, id: " << finger.id()<< ", length: " << finger.length()<< "mm, width: " << finger.width() << std::endl;

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				//std::cout << std::string(6, ' ') <<  boneNames[boneType]<< " bone, start: " << bone.prevJoint()<< ", end: " << bone.nextJoint() << ", direction: " << bone.direction() << std::endl;
			}
		}
	}

	// Get tools
	const ToolList tools = frame.tools();
	for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
		const Tool tool = *tl;
		//std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id() << ", position: " << tool.tipPosition()<< ", direction: " << tool.direction() << std::endl;
	}

	// Get gestures
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < 1; ++g)
	{
		Gesture gesture = gestures[g];

		if (!gesture.isValid())
			continue;

		switch (gesture.type()) {
			case Gesture::TYPE_CIRCLE:
				handleCircleGesture(gesture);
				break;

			case Gesture::TYPE_SWIPE:			
				handleSwipeGesture(gesture);				
				break;

			case Gesture::TYPE_KEY_TAP:
				handleKeyTapGesture(gesture);
				break;

			case Gesture::TYPE_SCREEN_TAP:
				handleScreenTapGesture(gesture);
				break;

			default:
				//std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
				break;
		}
	}
	
	// myKeyHandler('C', 0, 0);

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		short x = 0;
		//std::cout << std::endl;
	}
}

void LeapListener::handleCircleGesture(const Gesture& gesture) {

	//CircleGesture circle = gesture;
	//std::string clockwiseness;
	//
	//if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
	//clockwiseness = "clockwise";
	//} else {
	//clockwiseness = "counterclockwise";
	//}
	//
	////Calculate angle swept since last frame
	//float sweptAngle = 0;
	//if (circle.state() != Gesture::STATE_START) {
	//CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
	//sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
	//}
	//std::cout << std::string(2, ' ') << "Circle id: " << gesture.id() << ", state: " << stateNames[gesture.state()] << ", progress: " << circle.progress() << ", radius: " << circle.radius()<< ", angle " << sweptAngle * RAD_TO_DEG <<  ", " << clockwiseness << std::endl;

}

void LeapListener::handleSwipeGesture(const Gesture& gesture) {
	
	SwipeGesture swipe = gesture;
	Gesture::State gestureState = gesture.state();

	//if ( > 0.75f)

	//only update the display once the swipe gesture is finished
	if (gestureState == Gesture::State::STATE_START)
		swipeStartPos = swipe.startPosition();

	//only update the display once the swipe gesture is finished
	if (gestureState == Gesture::State::STATE_STOP)
	{
		cout << "Swipe gesture recognized\n";
		Vector directionOfSwipe = swipe.direction();
		GLfloat xDirection = directionOfSwipe[0], yDirection = directionOfSwipe[1];
		unsigned char nextSymbol = 'd', prevSymbol = 'D';

		//swipeEndPos = swipe.position();
		
		//if swiping to the left
		if (xDirection < 0 /*|| yDirection > 0*/) {
			myKeyHandler(nextSymbol, 0, 0);
			cout << "Swiped left, go to next symbol\n";
		}

		//else if swiping to the right
		else if (xDirection > 0 /*|| yDirection < 0*/) {
			myKeyHandler(prevSymbol, 0, 0);
			cout << "Swiped right, go to prev symbol\n";
		}

		glutPostRedisplay();
	}
	//myKeyHandler('C', 0, 0);
	//resetCamera();
}
void LeapListener::handleKeyTapGesture(const Gesture& gesture) {
	KeyTapGesture tap = gesture;
	//std::cout << std::string(2, ' ') << "Key Tap id: " << gesture.id() << ", state: " << stateNames[gesture.state()]<< ", position: " << tap.position() << ", direction: " << tap.direction()<< std::endl;

}
void LeapListener::handleScreenTapGesture(const Gesture& gesture) {
	ScreenTapGesture screentap = gesture;
	//std::cout << std::string(2, ' ')<< "Screen Tap id: " << gesture.id()<< ", state: " << stateNames[gesture.state()]<< ", position: " << screentap.position()<< ", direction: " << screentap.direction()<< std::endl;

}
void LeapListener::onFocusGained(const Controller& controller) {
	cout << "Focus Gained\n";
}

void LeapListener::onFocusLost(const Controller& controller) {
	cout << "Focus Lost\n";
}

void LeapListener::onDeviceChange(const Controller& controller) {
	cout << "Device Changed\n";
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		// std::cout << "id: " << devices[i].toString() << std::endl;
		//std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void LeapListener::onServiceConnect(const Controller& controller) {
	cout << "Service Connected\n";
}

void LeapListener::onServiceDisconnect(const Controller& controller) {
	cout << "Service Disconnected\n";
}


/*
int main(int argc, char** argv) {
Create a sample listener and controller
LeapListener listener;
Controller controller;

Have the sample listener receive events from the controller
controller.addListener(listener);

if (argc > 1 && strcmp(argv[1], "--bg") == 0)
controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);

Keep this process running until Enter is pressed
std::cout << "Press Enter to quit..." << std::endl;
std::cin.get();

Remove the sample listener when done
controller.removeListener(listener);

return 0;
}
*/
