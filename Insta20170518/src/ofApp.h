#pragma once

#include "ofMain.h"
#include "ofxBullet.h"

#include "Leap.h"
#pragma comment(lib, "Leap.lib")

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		~ofApp();
		void drawHand(Leap::Hand hand);
		void drawFinger(Leap::Finger finger);

		ofEasyCam cam;
		ofLight light;

		ofxBulletWorldRigid world;
		vector<ofxBulletBox*> obs;
		vector<ofxBulletBox*> boxes;
		vector<ofColor> boxes_colors;

		// Leap Motion
		Leap::Controller leap;
		bool right_pinch;
		bool left_pinch;
		ofVec3f right_position;
		ofVec3f left_position;
};
