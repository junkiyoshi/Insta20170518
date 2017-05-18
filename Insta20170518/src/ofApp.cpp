#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {
	for (ofxBulletBox* tmp : this->obs) { delete tmp; }
	this->obs.clear();

	for (ofxBulletBox* tmp : this->boxes) { delete tmp; }
	this->boxes.clear();
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofEnableDepthTest();
	ofBackground(255);
	ofSetWindowTitle("creater!");

	this->world.setup();
	this->world.setGravity(ofVec3f(0.0, -1024.0, 0));

	ofxBulletBox* obstacle = new ofxBulletBox();
	obstacle->create(this->world.world, ofVec3f(0, -ofGetHeight() / 3, 0), 0.0f, 512, 64, 512);
	obstacle->setProperties(1.0, 0.3);
	obstacle->add();
	this->obs.push_back(obstacle);

	this->light.setPosition(ofVec3f(512, 512, 512));

	this->right_pinch = false;
	this->left_pinch = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	this->world.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	this->cam.begin();

	ofEnableLighting();
	this->light.enable();

	for (int i = 0; i < this->obs.size(); i++) {
		ofSetColor(128);
		this->obs[i]->draw();
	}

	for (int i = 0; i < this->boxes.size(); i++) {
		ofSetColor(this->boxes_colors[i]);
		this->boxes[i]->draw();
	}

	Leap::Frame frame = leap.frame();
	Leap::HandList hands = frame.hands();
	for (int i = 0; i < hands.count(); i++) {
		float r = 0;
		float g = 0;
		float b = ofMap(hands[i].pinchStrength(), 0, 1, 0.0f, 255.0f);

		ofSetColor(r, g, b, 255);

		this->drawHand(hands[i]);

		if (hands[i].pinchStrength() > 0.9) {
			if (hands[i].isRight()) {
				this->right_position = ofVec3f(hands[i].palmPosition().x, hands[i].palmPosition().y, hands[i].palmPosition().z);
				if (!this->right_pinch) {
					this->right_pinch = true;
				}
			}

			if (hands[i].isLeft()) {
				this->left_position = ofVec3f(hands[i].palmPosition().x, hands[i].palmPosition().y, hands[i].palmPosition().z);
				if (!this->left_pinch) {
					this->left_pinch = true;
				}
			}
		} else {
			if (this->right_pinch && this->left_pinch) {
				float dist_x = this->right_position.x - this->left_position.x;
				float dist_y = this->right_position.y - this->left_position.y;
				float dist_z = this->right_position.z - this->left_position.z;
				ofVec3f position = ofVec3f(this->right_position.x - dist_x / 2, this->right_position.y - dist_y / 2, this->right_position.z - dist_z / 2);

				float size_x = dist_x > 0 ? dist_x : dist_x * -1;
				float size_y = dist_y > 0 ? dist_y : dist_y * -1;
				float size_z = dist_z > 0 ? dist_z : dist_z * -1;

				ofxBulletBox* box = new ofxBulletBox();
				box->create(this->world.world, ofVec3f(position), 1.0, size_x, size_y, size_z);
				box->add();
				this->boxes.push_back(box);

				ofColor c;
				c.setHsb(ofRandom(255), 255, 255);
				this->boxes_colors.push_back(c);

				this->right_pinch = false;
				this->left_pinch = false;
			}
		}

		if (hands[i].grabStrength() > 0.8 && hands[i].isRight()) {
			for (auto& box : this->boxes) {
				box->applyCentralForce(ofVec3f(hands[i].palmVelocity().x, hands[i].palmVelocity().y, hands[i].palmVelocity().z) * 32);
			}
		}

		if (this->right_pinch && this->left_pinch) {
			ofSetColor(0);

			float dist_x = this->right_position.x - this->left_position.x;
			float dist_y = this->right_position.y - this->left_position.y;
			float dist_z = this->right_position.z - this->left_position.z;
			ofVec3f position = ofVec3f(this->right_position.x - dist_x / 2, this->right_position.y - dist_y / 2, this->right_position.z - dist_z / 2);

			float size_x = dist_x > 0 ? dist_x : dist_x * -1;
			float size_y = dist_y > 0 ? dist_y : dist_y * -1;
			float size_z = dist_z > 0 ? dist_z : dist_z * -1;
			
			ofBox(ofVec3f(position), size_x, size_y, size_z);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::drawHand(Leap::Hand hand) {
	Leap::FingerList fingers = hand.fingers();
	for (int j = 0; j < fingers.count(); j++) {
		this->drawFinger(fingers[j]);
	}

	ofPushMatrix();
	ofVec3f palm_point = ofVec3f(hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);
	ofTranslate(palm_point);
	ofSphere(10);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawFinger(Leap::Finger finger) {

	ofVec3f tip_point = ofVec3f(finger.tipPosition().x, finger.tipPosition().y, finger.tipPosition().z);
	ofPushMatrix();
	ofTranslate(tip_point);
	ofSphere(5);
	ofPopMatrix();

	ofVec3f base_point = ofVec3f(tip_point.x + finger.direction().x * finger.length(),// * -1,
		tip_point.y + finger.direction().y * finger.length() - 1,
		tip_point.z + finger.direction().z * finger.length() - 1);
	ofPushMatrix();
	ofTranslate(base_point);
	ofSphere(5);
	ofPopMatrix();

	ofLine(tip_point, base_point);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
