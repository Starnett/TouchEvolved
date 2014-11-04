#include <iostream>
#include <string.h>
#include "Leap.h"
#include "common.h"
#include "LeapControl.h"
#include "LeapMath.h"
#include <time.h>

extern void myIdleFunc();
extern void myKeyHandler(unsigned char, int, int);
extern void myDisplay(void);
extern void resetCamera(void);
extern void rotateCamera(double deg, int axis);

using namespace Leap;
using namespace std;


bool togglePinchZoom = false;
bool toggleTouchEmulation = false;
bool toggleSwipeCircle = false;
bool toggleSwipeLinear = false;
bool toggleKeyTap = false;
bool toggleScreenTap = false;
bool thumbActionFinished = false;

float currPinchStrength = 0;
float prevPinchStrength = 0;

Frame currFrame;
Frame prevFrame;

string fingers[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
string bones[] = { "MetaCarpal", "Proximal", "Middle", "Distal" };
string state[] = { "INVALID", "STARTING", "UPDATING", "ENDING" };

Vector swipeStartPos;
Vector swipeEndPos;
//Hand currentHand;

int numFingersExtended = 0;
FingerList extendedFingerList;

time_t lastGestureTime = time(NULL);
time_t currentTime = time(NULL);
//time(&lastGestTime);

const float LAST_GESTURE_DELAY = 0.5f;

//Grab & Rotate settings
const float MIN_GRAB_STRENGTH = 0.95f;
const float MIN_PITCH_THRESHOLD = 15.0f * DEG_TO_RAD;
const float MIN_YAW_THRESHOLD = 15.0f * DEG_TO_RAD;
const float MIN_ROLL_THRESHOLD = 15.0f * DEG_TO_RAD;

//Grab & Translate settings
const float TRANSLATE_MIN_DISTANCE = 3.0f; //mm
const float MIN_TRANSLATE_MULT = 1.0f;
const float MAX_TRANSLATE_MULT = 4.0f;
const float MIN_VELOCITY = 100.0f;
const float TRANSLATION_HAND_SPEED_MULT = 0.1f;

Vector translateStartPos;
Vector translateEndPos;
Vector translateCurrPos;
Vector translatePrevPos;
Vector handSpeed;
enum TranslateState { NONE, START, UPDATE, END };
TranslateState translateState = NONE;

//Pinch settings
const float MIN_PINCH_STRENGTH = 0.25f;
const float PINCH_MIN_ROLL_THRESHOLD = 0.0f * DEG_TO_RAD;
const float PINCH_MAX_ROLL_THRESHOLD = 13.0f * DEG_TO_RAD;

//SwipeGesture settings
const int SWIPE_MIN_LENGTH = 50;
const int SWIPE_MIN_VELOCITY = 200;
const float SWIPE_MAX_ROLL_THRESHOLD = 15.0f * DEG_TO_RAD;

//CircleGesture settings
const float CIRCLE_MIN_RADIUS = 5.0f; //mm
const float CIRCLE_MIN_ARC = 0.5f; //radians

//KeyTapGesture settings
const float KEYTAP_MIN_DOWN_VELOCITY = 40.0f; //mm/s
const float KEYTAP_HISTORY_SECONDS = 0.1f; //s
const float KEYTAP_MIN_DISTANCE = 3.0f; //mm


//ScreenTapGesture settings
const float SCREENTAP_MIN_FORWARD_VELOCITY = 30.0f; //mm/s
const float SCREENTAP_HISTORY_SECONDS = 0.1f; //s
const float SCREENTAP_MIN_DISTANCE = 5.0f; //mm

const float MIN_Z_IGNORE_DISTANCE = -73.5f; //interaction box min Z border

unsigned char nextSymbol = 'd', prevSymbol = 'D';
unsigned char rotatePosX = ',', rotateNegX = '<';
unsigned char rotatePosY = '.', rotateNegY = '>';
unsigned char rotatePosZ = '/', rotateNegZ = '?';
unsigned char zoomIn = '+', zoomOut = '=';

float lerp(float start, float end, float percent) {
	return (start + percent * (end - start));
}
float lerp(int start, int end, float percent) {
	return (start + percent * (end - start));
}

inline float clamp(float x, float a, float b) {
	return x < a ? a : (x > b ? b : x);
}

inline bool isClockwise(CircleGesture c) {
	return c.pointable().direction().angleTo(c.normal()) <= PI / 2;
}

void LeapListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void LeapListener::onConnect(const Controller& controller) {
	cout << "Connected\n";
	//For testing purposes, only enable swipes for now.

	//controller.enableGesture(Gesture::TYPE_CIRCLE);
	//controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	//controller.enableGesture(Gesture::TYPE_SWIPE);

	//change the minimum swipe velocity and distance to a smaller interval so that the gesture doesn't 
	//need to be so rapid
	controller.config().setFloat("Gesture.Swipe.MinLength", SWIPE_MIN_LENGTH);
	controller.config().setFloat("Gesture.Swipe.MinVelocity", SWIPE_MIN_VELOCITY);
	controller.config().setFloat("Gesture.Circle.MinRadius", CIRCLE_MIN_RADIUS);
	controller.config().setFloat("Gesture.Circle.MinArc", CIRCLE_MIN_ARC);
	controller.config().setFloat("Gesture.KeyTap.MinDownVelocity", KEYTAP_MIN_DISTANCE);
	controller.config().setFloat("Gesture.KeyTap.HistorySeconds", KEYTAP_HISTORY_SECONDS);
	controller.config().setFloat("Gesture.KeyTap.MinDistance", KEYTAP_MIN_DISTANCE);
	controller.config().setFloat("Gesture.ScreenTap.MinForwardVelocity", SCREENTAP_MIN_FORWARD_VELOCITY);
	controller.config().setFloat("Gesture.ScreenTap.HistorySeconds", SCREENTAP_HISTORY_SECONDS);
	controller.config().setFloat("Gesture.ScreenTap.MinDistance", SCREENTAP_MIN_DISTANCE);
	controller.config().save();
}

void LeapListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	cout << "Disconnected\n";
}

void LeapListener::onExit(const Controller& controller) {
	cout << "Exited\n";
}

//NOTE: as of 10/28/14, use Canvas::OnIdleFunc() to poll frames. -Jason
void LeapListener::onFrame(const Controller& controller) {
	

}

void LeapListener::handleGestures(const Frame& frame) {

	currFrame = frame;
	HandList hands = frame.hands();
	Hand hand = hands[0];
	FingerList fingers = hand.fingers();
	extendedFingerList = fingers.extended();
	numFingersExtended = extendedFingerList.count();
	// cout << "numFingersExtended = " << numFingersExtended << endl

	//Verify palm center has not breached back wall of interaction box
	//Otherwise ignore all input, since this means user want to interact
	//with the touch screen and not the Leap controller
	if (hand.palmPosition().z < MIN_Z_IGNORE_DISTANCE) {
		cout << "Palm center (" << hand.palmPosition().z;
		cout << ") < MIN_Z_IGNORE_DISTANCE" << endl;
		return;
	}

	handSpeed = hand.palmVelocity();
	
	//Get gestures	
	const GestureList gestures = frame.gestures();
	//cout << "gestures.count() = " << gestures.count() << endl;

	if (gestures.count() > 0) {
		for (int g = 0; g < 1; ++g)
		{
			Gesture gesture = gestures[g];

			if (!gesture.isValid())
				continue;

			switch (gesture.type()) {
			case Gesture::TYPE_CIRCLE:
				if (numFingersExtended == 1)
					handleCircleGesture(gesture);
				break;

			case Gesture::TYPE_SWIPE:

				//require at least 1 extended finger to trigger a swipe detect
				if (numFingersExtended == 2)
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
			}//switch
		}//for
	}//if


	if (1)
	if (numFingersExtended == 0) {
		//check if user forms a fist (for rotating objects)
		if (hand.grabStrength() > MIN_GRAB_STRENGTH) {
			handleRotation(hand);
		}
	}
	//check if user pinches thumb and index finger (for zooming objects)
	if (0) 
	if (hand.pinchStrength() > MIN_PINCH_STRENGTH) {
		handlePinchZoom(hand);
	}

	//check if user is using thumb and pinky to pierce imaginary touch wall	
	if (0) handleTouchEmulation(frame);

	// track which which quadrant/half hand is in
	// if thumb is extended, cycle forward/backward
	if (0) handleInteractionBox(frame);

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		short x = 0;
		//std::cout << std::endl;
	}
}

void LeapListener::handleInteractionBox(const Frame& frame) {
	// setup interaction box
	InteractionBox interactionBox = frame.interactionBox();

	// get basic info of box
	Vector boxCenter = interactionBox.center();
	float boxHeight = interactionBox.height();
	float boxWidth = interactionBox.width();

	// get hand info
	HandList hands = frame.hands();
	Hand hand = hands[0];
	Vector handCenter = hand.palmPosition();

	// get notion of thumb (and ONLY the thumb) being extended
	FingerList extendedFingerList = hand.fingers().extended();
	bool thumbExtended = extendedFingerList[0].type() == Finger::TYPE_THUMB ? true : false;

	// If the user retracts the thumb from a previously completed thumb-action,
	// reinitiate action capability (e.g. set thumbActionFinished equal to false).
	// This safeguards against initiating more than one action while holding the thumb out.
	if (thumbActionFinished && !thumbExtended){
		thumbActionFinished = false;
		cout << "thumb action RESET to false" << endl;
	}

	// get quadrant-based location of entity
	// NOTE: from the perspective of looking down on the Leap Motion device
	bool inQuadrantI = handCenter[0] > boxCenter[0] && handCenter[2] > boxCenter[2] && handCenter[1] < boxHeight;
	bool inQuadrantII = handCenter[0] < boxCenter[0] && handCenter[2] > boxCenter[2] && handCenter[1] < boxHeight;
	bool inQuadrantIII = handCenter[0] < boxCenter[0] && handCenter[2] < boxCenter[2] && handCenter[1] < boxHeight;
	bool inQuadrantIV = handCenter[0] > boxCenter[0] && handCenter[2] < boxCenter[2] && handCenter[1] < boxHeight;

	// execute action based on thumb being extended in specified half (left/right)
	if (thumbExtended && !thumbActionFinished){
		if (inQuadrantI || inQuadrantIV){
			// cycle forward
			cout << "cycle forward -->" << endl;
			myKeyHandler('d', 0, 0);
			thumbActionFinished = true;
		}
		else if (inQuadrantII || inQuadrantIII){
			// cycle backward
			cout << "cycle backward <--" << endl;
			myKeyHandler('D', 0, 0);
			thumbActionFinished = true;
		}
	}
}

//testing. now using globals.
bool LeapListener::checkExtendedFingersCount(int n) {
	HandList hands = currFrame.hands();
	FingerList fingers = hands[0].fingers();
	FingerList extendedFingerList = fingers.extended();
	return (extendedFingerList.count() == n);
}
void LeapListener::checkHandRoll(const Hand hand) {
	//may or may not go this route later.
}
unsigned char AssignKeyCode(float rotation, float threshold, unsigned char cPos, unsigned char cNeg) {
	if (rotation > threshold)
		return cNeg;
	else if (rotation < -threshold)
		return cPos;
	else
		return NULL;
}
void LeapListener::handleRotation(const Hand hand) {

	// Get the hand's normal vector and direction
	//const Vector normal = hand.palmNormal();
	//const Vector direction = hand.direction();

	float pitch = hand.direction().pitch();
	float yaw = hand.direction().yaw();
	float roll = hand.palmNormal().roll();
	float pitchDegree = pitch * RAD_TO_DEG;
	float yawDegree = yaw * RAD_TO_DEG;
	float rollDegree = roll * RAD_TO_DEG;
	bool pitchIsGreatest = abs(pitch) > abs(yaw) && abs(pitch) > abs(roll);
	bool yawIsGreatest = abs(yaw) > abs(pitch) && abs(yaw) > abs(roll);
	bool rollIsGreatest = abs(roll) > abs(yaw) && abs(roll) > abs(pitch);
	int rotateMult = 1;
	int minRotateMult = 1;
	int maxRotateMult = 3;
	unsigned char keyCode = NULL;
	float activeRotationDegrees = 0;
	float activeRotationRadians = 0;
	unsigned char maxRotType = ' ';

	if (pitchIsGreatest) {
		//if pitch angle > N deg then hand points upward, so rotate backwards
		keyCode = AssignKeyCode(pitch, MIN_PITCH_THRESHOLD, rotateNegX, rotatePosX);
		activeRotationRadians = pitch;
		maxRotType = 'P';
		//cout << "pitchIsGreatest: " << pitchDegree << endl;
	}
	 else if (yawIsGreatest) {
		 keyCode = AssignKeyCode(yaw, MIN_YAW_THRESHOLD, rotateNegZ, rotatePosZ);
		 activeRotationRadians = yaw;
		 maxRotType = 'Y';
		//cout << "yawIsGreatest: " << yawDegree << endl;
	}
	 else if (rollIsGreatest) {
		 keyCode = AssignKeyCode(roll, MIN_ROLL_THRESHOLD, rotateNegY, rotatePosY);
		 activeRotationRadians = roll;
		 maxRotType = 'R';
		 //cout << "rollIsGreatest: " << rollDegree << ", ";
	}

	 if (1 || keyCode == NULL) {
		 handleTranslation(hand);
	 }
	 else
	 {
		//check if ending a translation
		 if (translateState = UPDATE) {
			 translateState = NONE;
		 }

		//process rotation data
		activeRotationDegrees = activeRotationRadians * RAD_TO_DEG;		
		float percent = clamp(abs(activeRotationDegrees) / 90.0f, 0, 1.0f);
		rotateMult = lerp(minRotateMult, maxRotateMult, percent);
		myKeyHandler(keyCode, rotateMult, 0);

		if (0) {
			cout << "pitch: " << pitchDegree << " ";
			cout << "yaw: " << yawDegree << " ";
			cout << "roll: " << rollDegree << endl;
			cout << maxRotType << ", ";
			cout << "pct: " << percent << ", ";
			cout << "deg: " << activeRotationDegrees << ", ";
			cout << "mul: " << rotateMult << endl;
		}
	}
}
void LeapListener::handleTranslation(const Hand hand) {


	float distance, percent, translateMult;
	bool allowScale = false;
	Vector addVec;
	float mag = handSpeed.magnitude();

	switch (translateState) {
	case NONE:
		translateStartPos = hand.palmPosition();
		translatePrevPos = translateStartPos;
		translateState = START;
		break;
	case START:
		//method A of moving stuff around: velocity-based
		if (mag >= MIN_VELOCITY) {

			//todo: figure out some good clamp values as we scale up/down speed
			percent = clamp(mag / 1000.0f, 0, 1.0f);
			translateMult = lerp(MIN_TRANSLATE_MULT, MAX_TRANSLATE_MULT, percent);
			translatePrevPos = translateCurrPos;
			addVec = allowScale ? handSpeed * translateMult : Vector::zero();
			translateCurrPos = hand.palmPosition() + addVec;

			cout << "handSpeed: " << handSpeed << ", ";
			cout << "mag: " << mag << ", ";
			cout << "mult: " << translateMult << endl;
		}
		else {

			translateState = NONE;
		
		}

		break;
	case UPDATE:
		//method B of moving stuff around: pos delta. WIP
		distance = translatePrevPos.distanceTo(translateCurrPos);
		if (distance >= TRANSLATE_MIN_DISTANCE) {

			percent = clamp(distance, 0, 1.0f);
			translateMult = lerp(MIN_TRANSLATE_MULT, MAX_TRANSLATE_MULT, percent);

			translatePrevPos = translateCurrPos;
			translateCurrPos = hand.palmPosition();
			//cout << "handleTranslation() " << hand.palmPosition() << endl;
		}
		else {
			translateState = NONE;
		}
		break;
	case END:
		break;
	}


}

/*
	10/29/14: temporarily require user to orient hand s.t. palm faces floor.
	would eventually like to have user able to pinch zoom during a rotation.
	pinch strength gets in way of fist strength. might need other input combos.
*/
void LeapListener::handlePinchZoom(const Hand hand) {

	//require extended index finger
	//if (extendedFingerList[0].type() != Finger::Type::TYPE_THUMB) return;
	//if (extendedFingerList[1].type() != Finger::Type::TYPE_INDEX) return;
	if (!checkExtendedFingersCount(5)) return;

	float rollRadians = hand.palmNormal().roll();
	float rollDegrees = rollRadians * RAD_TO_DEG;
	
	//require palm to face ceiling to avoid overlap with object rotations
	if (abs(rollRadians) < PINCH_MIN_ROLL_THRESHOLD ||
		abs(rollRadians) > PINCH_MAX_ROLL_THRESHOLD) {
		// cout << "rollDegrees(" << rollDegrees << ") out of threshold range" << endl;
		return;
	}	

	
	prevPinchStrength = currPinchStrength;
	currPinchStrength = hand.pinchStrength();
	float threshold = 0.01f;
	int zoomMult = 2;
	//pinching closer means zooming out
	if (currPinchStrength > prevPinchStrength + threshold) {
		myKeyHandler(zoomOut, zoomMult, 0);
		cout << "handlePinchZoom OUT" << endl;
	}
	else if (currPinchStrength < prevPinchStrength - threshold) {
		myKeyHandler(zoomIn, zoomMult, 0);
		cout << "handlePinchZoom IN" << endl;
	}
}
void LeapListener::handleTouchEmulation(const Frame& frame) {

	/*
	The zones are “hovering,” “touching,” and “none.”
	The touch zone is reported by the touchZone attribute of the Pointable class. The zones are identified using the Zone enumeration, which defines the following states:

	NONE — the pointable is either too far from the touch surface to be considered touching, or it is pointing back toward the user.
	HOVERING — the pointable tip has crossed into the hovering zone, but isn’t considered touching.
	TOUCHING — the pointable has crossed the virtual touch surface.

	The distance ranges from +1 to -1 as the finger moves to and through the virtual touch surface.
	The distance does not represent a physical quantity, but rather how close to touching the Leap Software considers the pointable.
	*/


	if (numFingersExtended != 2) return;

	//testing
	//cout << "extendedFingerList.count() = " << extendedFingerList.count() << endl;
	//cout << "type[0] " << extendedFingerList[0].type() << ", ";
	//cout << "type[1] " << extendedFingerList[1].type() << endl;

	if (extendedFingerList[0].type() != Finger::Type::TYPE_INDEX) return;
	if (extendedFingerList[1].type() != Finger::Type::TYPE_PINKY) return;
	
	Leap::Pointable pointable = frame.pointables().frontmost();
	float distance = pointable.touchDistance();
	Pointable::Zone touchZone = pointable.touchZone();

	//cout << "handleTouchEmulation() d: " << distance << ", z: " << touchZone << endl;

	if (distance > 0 && touchZone != Leap::Pointable::Zone::ZONE_NONE)
	{
		toggleTouchEmulation = false;
	}
	else if (distance <= 0)
	{
		if (!toggleTouchEmulation) {
			//cycle object once when pointable enters touch zone.
			//user must remove pointable from touch zone prior to re-entry if another cycle is desired
			myKeyHandler(nextSymbol, 0, 0);
			toggleTouchEmulation = true;
			cout << "Pointable::Zone::TOUCHING, go to next symbol\n";
		}
	}
	else
	{
		toggleTouchEmulation = false;
	}
}
void LeapListener::handleCircleGesture(const Gesture& gesture) {

	HandList handsForGesture = gesture.hands();

	//require 1 hand
	if (handsForGesture.count() != 1) return; //does not prevent 2 hand gestures


	CircleGesture circle = gesture;
	bool clockwise = isClockwise(circle);
	float turns = circle.progress();
	//cout << "turns: " << turns << endl;
	
	switch (circle.state()) {
		case Gesture::STATE_START:
			//Handle starting gestures
			if (!toggleSwipeCircle)
				toggleSwipeCircle = true;
			break;
		case Gesture::STATE_UPDATE:
			//Handle continuing gestures
			break;
		case Gesture::STATE_STOP:
			if (!toggleSwipeCircle) return;

			//Handle ending gestures
			if (clockwise) {
				toggleSwipeCircle = false;
				myKeyHandler(nextSymbol, 0, 0);
				cout << "Swiped clockwise, go to next symbol\n";
			}
			else {
				toggleSwipeCircle = false;
				myKeyHandler(prevSymbol, 0, 0);
				cout << "Swiped ctrclockwise, go to prev symbol\n";
			}
			break;
		default:
			//Handle unrecognized states
			break;
	}
	//from SDK sample code:
	//float sweptAngle = 0;
	//if (circle.state() != Gesture::STATE_START) {
	//CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
	//sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
	//}
	//std::cout << std::string(2, ' ') << "Circle id: " << gesture.id() << ", state: " << stateNames[gesture.state()] << ", progress: " << circle.progress() << ", radius: " << circle.radius()<< ", angle " << sweptAngle * RAD_TO_DEG <<  ", " << clockwiseness << std::endl;

}
void LeapListener::handleSwipeGesture(const Gesture& gesture) {

	HandList handsForGesture = gesture.hands();

	//require 2 hands
	if (handsForGesture.count() != 1) return;


	//todo: verify hand roll < SWIPE_MAX_ROLL_THRESHOLD prior to computing
	
	//if (difftime(currentTime,lastGestureTime) < LAST_GESTURE_DELAY)
	//	return;
	//
	//cout << "difftime: " << difftime(currentTime, lastGestureTime) << endl;
	//cout << "timeDiff: " << time(NULL) - lastGestureTime << endl;
	//cout << "lastGestureTime: " << lastGestureTime << endl;

	SwipeGesture swipe = gesture;
	//Gesture::State gestureState = gesture.state();
	
	Vector directionOfSwipe;
	GLfloat xDirection;
	GLfloat yDirection;

	switch (swipe.state()) {
		case Gesture::STATE_START:
			//Handle starting gestures
			if (!toggleSwipeLinear) {
				toggleSwipeLinear = true;
				//only update the display once the swipe gesture is finished
				swipeStartPos = swipe.startPosition();

				cout << "start swipe gesture" << endl;
			}
			break;
		case Gesture::STATE_UPDATE:
			//Handle continuing gestures
			break;
		case Gesture::STATE_STOP:
			if (!toggleSwipeLinear) return;

			toggleSwipeLinear = false;

			//only update the display once the swipe gesture is finished
			cout << "stop swipe gesture" << endl;

			directionOfSwipe = swipe.direction();
			xDirection = directionOfSwipe[0];
			yDirection = directionOfSwipe[1];
			//swipeEndPos = swipe.position();

			//start a new timer when the gesture ends
			lastGestureTime = time(NULL);
			
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
			break;
		default:
			//Handle unrecognized states
			break;
	}//switch
}
/*
	Function: handleKeyTapGesture
	Tap with index finger to scroll forward
	Tap with middle finger to scroll backward
	All fingers must be extended
*/
void LeapListener::handleKeyTapGesture(const Gesture& gesture) {

	HandList handsForGesture = gesture.hands();

	//require 1 hand
	if (handsForGesture.count() != 1) return;


	if (numFingersExtended != 5) return;

	KeyTapGesture tap = gesture;
	//std::cout << std::string(2, ' ') << "Key Tap id: " << gesture.id() << ", state: " << stateNames[gesture.state()]<< ", position: " << tap.position() << ", direction: " << tap.direction()<< std::endl;

	//The finger performing the key tap gesture.
	Pointable tappingPointable = tap.pointable();

	Vector directionOfTap;
	GLfloat xDirection;
	GLfloat yDirection;

	switch (tap.state()) {
	case Gesture::STATE_START:
		//Handle starting gestures
		if (!toggleKeyTap) {
			toggleKeyTap = true;
			cout << "start tap gesture" << endl;
		}
		break;
	case Gesture::STATE_UPDATE:
		//Handle continuing gestures
		break;
	case Gesture::STATE_STOP:
		if (!toggleKeyTap) return;

		toggleKeyTap = false;

		//only update the display once the swipe gesture is finished
		cout << "stop tap gesture" << endl;
		
		directionOfTap = tap.direction();
		xDirection = directionOfTap[0];
		yDirection = directionOfTap[1];
		
		if (tappingPointable.isTool()) {
			Leap::Tool tool = Leap::Tool(tappingPointable); //temp for later testing
			myKeyHandler(nextSymbol, 0, 0);
			cout << "Tapped tool, go to next symbol\n";
		}
		else {
			Leap::Finger finger = Leap::Finger(tappingPointable);

			//if tapping with index finger
			if (finger.type() == Finger::Type::TYPE_INDEX) {
				myKeyHandler(nextSymbol, 0, 0);
				cout << "Tapped index finger, go to next symbol\n";
			}

			//else if tapping with middle finger
			if (finger.type() == Finger::Type::TYPE_MIDDLE) {
				myKeyHandler(prevSymbol, 0, 0);
				cout << "Tapped middle finger, go to prev symbol\n";
			}
		}

		glutPostRedisplay();
		break;
	default:
		//Handle unrecognized states
		break;
	}//switch

}
/*
	Function: handleScreenTapGesture
	Tap with both index finger to scroll forward
	Tap with both middle fingers to scroll backward
	One finger must be extended per hand.
	Known SDK issue: interferes with circle gesture.

	10/30/14 notes:
	SDK cannot detect more than 1 finger/hand at this time.
	This is not a reliable input method.
	Use handleTouchEmulation() as a workaround for "screen press/tap actions"
*/
void LeapListener::handleScreenTapGesture(const Gesture& gesture) {

	
	ScreenTapGesture tap = gesture;
	Pointable poker = tap.pointable();
	Leap::Finger finger = Leap::Finger(poker);
	//	cout << finger.type() << endl;
	//	return;
	
	if (0) { //does not detect multiple poking fingers.
		PointableList pointablesForGesture = tap.pointables();
		cout << pointablesForGesture.count() << endl;
		return;
	}

	cout << "dir: " << tap.direction() << ",";
	cout << "#hands: " << tap.hands().count() << ",";
	cout << "#pointables(): " << tap.pointables().count() << endl;
	
	return;
	//--------------------------------------
	HandList handsForGesture = gesture.hands();

	//require 2 hands
	if (handsForGesture.count() != 2) return;

	cout << "2 hands found" << endl;

	PointableList extended1 = handsForGesture[0].pointables().extended();
	PointableList extended2 = handsForGesture[0].pointables().extended();

	//require only 1 extended finger per hand
	if (extended1.count() != 1 && extended2.count() != 1) return;

	cout << "2 extended fingers found" << endl;

	//std::cout << std::string(2, ' ')<< "Screen Tap id: " << gesture.id()<< ", state: " << stateNames[gesture.state()]<< ", position: " << screentap.position()<< ", direction: " << screentap.direction()<< std::endl;


	//std::cout << std::string(2, ' ') << "Key Tap id: " << gesture.id() << ", state: " << stateNames[gesture.state()]<< ", position: " << tap.position() << ", direction: " << tap.direction()<< std::endl;

	//The fingers performing the screen tap gesture.
	Pointable poker1 = extended1[0];
	Pointable poker2 = extended2[0];

	Leap::Finger finger1 = Leap::Finger(poker1);
	Leap::Finger finger2 = Leap::Finger(poker2);

	Vector directionOfTap;
	GLfloat xDirection;
	GLfloat yDirection;

	switch (tap.state()) {
	case Gesture::STATE_START:
		//Handle starting gestures
		if (!toggleKeyTap) {
			toggleKeyTap = true;
			cout << "start tap gesture" << endl;
		}
		break;
	case Gesture::STATE_UPDATE:
		//Handle continuing gestures
		break;
	case Gesture::STATE_STOP:
		if (!toggleKeyTap) return;

		toggleKeyTap = false;

		//only update the display once the swipe gesture is finished
		cout << "stop tap gesture" << endl;

		directionOfTap = tap.direction();
		xDirection = directionOfTap[0];
		yDirection = directionOfTap[1];

		//if tapping with index fingers
		if (finger1.type() == Finger::Type::TYPE_INDEX && 
			finger2.type() == Finger::Type::TYPE_INDEX) {
			myKeyHandler(nextSymbol, 0, 0);
			cout << "Tapped index fingers, go to next symbol\n";
		}

		//else if tapping with middle fingers
		if (finger1.type() == Finger::Type::TYPE_MIDDLE &&
			finger2.type() == Finger::Type::TYPE_MIDDLE) {
			myKeyHandler(prevSymbol, 0, 0);
			cout << "Tapped middle fingers, go to prev symbol\n";
		}

		glutPostRedisplay();
		break;
	default:
		//Handle unrecognized states
		break;
	}//switch

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