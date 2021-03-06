#include "ofApp.h"

//~~Let's get messy~~//

//Bubble constructor
Bubble::Bubble() {
	position.x = ofRandom((ofGetWidth() - 50.0));
	position.y = 0.0;
	color.r = 67;
	color.g = 199;
	color.b = 255;
	radius = 30;
	// variables usedful for isRemoved
    //only calculated once at creation time
//	leftX = position.x - (radius / 2);
//	topY = position.y - (radius / 2);
//	rightX = position.x + (radius / 2);
//	bottomY = position.y + (radius / 2);
}

//determines whether bubble stays or goes
//bool Bubble::isRemoved(ofxCvGrayscaleImage refImg, ofPixels pixels) {
//	int movementAmt = 0;
//	pixels = refImg.getPixels();
//	for (int y = topY; y < bottomY; y++) {
//		for (int x = leftX; x < rightX; x++) {
//			// changed line of code: it seems to have a problem with y* width of ref img
//			// because if it is just y + x, code runs fine
//			// if (pixels.getColor((y*refImg.getWidth())+x).getLightness() > 150) {
//			if (pixels.getColor(y+x).getLightness() > 150) {
//				movementAmt++;
//			}
//		}
//	}
//	if (movementAmt > 5) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}


//--------------------------------------------------------------
void ofApp::setup() {
	camWidth = ofGetWidth();
	camHeight = ofGetHeight();

	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(1280, 720);

	colorImg.allocate(1280, 720);
	previous.allocate(1280, 720);
	current.allocate(1280, 720);
	difference.allocate(1280, 720);
	forDrawing.allocate(1280, 720);
	forDrawing.setFromPixels(vidGrabber.getPixelsRef());
}

//--------------------------------------------------------------
void ofApp::update() {
	forDrawing.setFromPixels(vidGrabber.getPixelsRef());
	colorImg.setFromPixels(vidGrabber.getPixelsRef());
	previous = colorImg;
	vidGrabber.update();
	colorImg.setFromPixels(vidGrabber.getPixelsRef());
	current = colorImg;
	difference.absDiff(current, previous);
	difference.threshold(30);
    difference.mirror(false, true);
    difference.flagImageChanged();

	for (int i = 0; i < bubbles.size(); ++i) {
        int movementAmt = 0;
        pixels = difference.getPixels();
        float leftX = bubbles[i].position.x - (bubbles[i].radius / 2);
        float topY = bubbles[i].position.y - (bubbles[i].radius / 2);
        float rightX = bubbles[i].position.x + (bubbles[i].radius / 2);
        float bottomY = bubbles[i].position.y + (bubbles[i].radius / 2);
        for (int y = topY; y < bottomY; y++) {
            for (int x = leftX; x < rightX; x++) {
                if(pixels.getColor(x, y).getLightness() > 150) {
                    movementAmt++;
                }
            }
        }

		if (movementAmt > 5) {
			bubbles.erase((bubbles.begin() + i));
			--i; //make sure you don't skip over a bubble
		}
		else {
			bubbles[i].position.y += 3;
		}
	}

	for (int i = 0; i < bubbles.size(); ++i) {
		if (bubbles[i].position.y > ofGetHeight()) {
			bubbles.erase((bubbles.begin() + i));
		}
	}
	updated = true;
}

//--------------------------------------------------------------
void ofApp::draw() {
	forDrawing.setFromPixels(vidGrabber.getPixelsRef());
	forDrawing.mirror(false, true);
	forDrawing.draw(0, 0);
	
	int frameNum = ofGetFrameNum();
	if (frameNum % 30 == 0) {
		Bubble bubble;
		ofDrawCircle(bubble.position.x, bubble.position.y, bubble.radius);
		ofSetColor(bubble.color);
		ofSetLineWidth(5);
		ofNoFill();
		//add bubble to bubbles vector
		bubbles.push_back(bubble);
	}

	if (updated == true) {
		for (int i = 0; i < bubbles.size(); ++i) {
			ofDrawCircle(bubbles[i].position.x, bubbles[i].position.y, bubbles[i].radius);
		}
	}
//    difference.draw(0,0, 320, 240);
}

//Spacebar clears the bubbles if you feel like it
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		bubbles.clear();
	}
}

