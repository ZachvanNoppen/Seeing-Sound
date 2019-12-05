
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//Setting up the window
	ofBackground(ofColor(200,200,200));
	ofSetVerticalSync(true);
	ofSetWindowShape(1400, 720);

	m_font.load("font.ttf", 12, true);
	//Loading in the music
	m_soundPlayer.load("dryhope.mp3");
	m_soundPlayer.setLoop(true);
	m_soundPlayer.play();

	//Starting at the beginning of the song
	m_soundPlayer.setPositionMS(0); 
	//Generalizing frequencies
	m_audioAnalyser.init(&m_soundPlayer, 30);
	//Not showing the analytics by default
	m_show_chart = false;

	//Setting up the camera/lighting
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableLighting();
	ofSetSmoothLighting(true);
	m_light.setPosition(500, 500, 0);
	ofSetGlobalAmbientColor(ofColor::white);
	//initializing the colours and gradient effect
	m_userPos = ofVec3f(0, 0, 0);
	m_mod = 1;
	bg_r = 1;
	bg_g = 1;
	bg_b = 1;

	//Setting up preset colours to make all elements look good together
	m_colourPallette.push_back(ofColor(2, 72, 115));
	m_colourPallette.push_back(ofColor(3, 150, 166));
	m_colourPallette.push_back(ofColor(4, 191, 173));
	m_colourPallette.push_back(ofColor(174, 230, 209));
	m_colourPallette.push_back(ofColor(242, 94, 94));
	currentColour = 0;
	sphereColour = 1;
	userColour = 2;
}

//--------------------------------------------------------------
void ofApp::update(){
	m_audioAnalyser.update();
	
	//Getting volume and track frequencies
	m_volume = m_audioAnalyser.getMixLevel();
	
	float kick = m_audioAnalyser.getLinearAverage(0);
	float keys = m_audioAnalyser.getLinearAverage(4);
	float synth = m_audioAnalyser.getLinearAverage(2);

	//Normalize values
	m_circleBrightness_1 = ofMap(kick, 60.0f, 80.0f, 0.0f, 1.0f, true);
	m_circleBrightness_2 = ofMap(keys, 23.0f, 45.0f, 0.0f, 1.0f, true);
	m_circleBrightness_3 = ofMap(synth, 50.0f, 100.0f, 0.0f, 120.0f, true);
	//Since the synth maps directly to the height of the user's trail through the formula (height = value - 30, giving ranges -30 to 30), a value outside the range would still map to 0, drawing a block at height = 0
	//We don't want to draw anything if there is no input from our syth so we will make a flag that tells us if the value is given
	if (synth < 50.0f) {
		m_circleBrightness_3 = -1.0f;
	}
	//Drawing the trail, and other objects in the scene
	generateObjects();
	
	//Making the gradient
	//If the r,g,b values go outside the range, add/subtract to keep them valid
	if (bg_r >= 255 || bg_r <= 0) { m_mod *= -1; }
	if (bg_g >= 255 || bg_g <= 0) { m_mod *= -1; }
	if (bg_b >= 255 || bg_b <= 0) { m_mod *= -1; }
	bg_r += 1 * m_mod;
	bg_g += 1 * m_mod;
	bg_b += 1 * m_mod;

	//Calculating the right time to change the background colour (there is input from the music, and it has not changed recently
	if (m_circleBrightness_2 >= 0.1f && m_pulseTimer < std::chrono::steady_clock::now() ) {
		m_backgorundColour = m_colourPallette[currentColour];
		
		//Chaning sphere and user colours
		if (currentColour == 4) {
			currentColour = 0;
		}
		else {
			currentColour += 1;
		}
		if (userColour == 4) {
			userColour = 0;
		}
		else {
			userColour += 1;
		}
		if (sphereColour == 4) {
			sphereColour = 0;
		}
		else {
			sphereColour += 1;
		}
		m_pulseTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(2000);
		m_circleBrightness_2 = 0.0f;

		//Increasing r,g,b to make the background gradient
		bg_r = m_backgorundColour.r;
		bg_g = m_backgorundColour.g;
		bg_b = m_backgorundColour.b;

	}
	else {
		m_circleBrightness_2 = 0.0f;
		
	}
	//Setting the colour
	m_backgorundColour = ofColor(bg_r, bg_g, bg_b);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//If the spacebar is pressed, toggle the GUI analytic board
	if (m_show_chart) {
		drawChart();
	}
	else {
		ofEnableDepthTest();
		m_cam.begin();
		m_light.enable();

		ofBackground(m_backgorundColour);
		//User position 
		//The camera always points at 0,0,0 so just make sure the other objects move above/below that axis
		ofSetColor(m_colourPallette[userColour]);
		ofDrawSphere(m_userPos, 30);

		//Drawing the user spheres
		ofSetColor(m_colourPallette[sphereColour]);
		for (ofVec3f pos : m_spherePositions) {

			ofDrawSphere(pos, 40 * (1 + m_volume));
		}
		ofSetColor(m_colourPallette[userColour]);
		for (ofVec3f pos : m_userTrail) {

			ofDrawBox(pos, 6);
		}

		m_light.disable();
		m_cam.end();
		ofDisableDepthTest();


		//Drawing the scrubbing timeline
		float position = ofMap(m_soundPlayer.getPositionMS(), 0.0f, 670000.0f, 30.0f, 1260.0f);
		ofPushMatrix();
		ofSetColor(ofColor::white);
		ofDrawCircle(position, ofGetWindowHeight()-40, 10);
		ofDrawRectangle(30, ofGetWindowHeight() - 40, ofGetWindowWidth()-60, 2);
		m_font.drawString(ofToString(m_soundPlayer.getPositionMS()/1000), ofGetWindowWidth()/2, ofGetWindowHeight()-60);

		m_font.drawString("<--", 30, ofGetWindowHeight()-60);
		m_font.drawString("-->", ofGetWindowWidth() - 70, ofGetWindowHeight() - 60);
		ofPopMatrix();

	}

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//Key press interaction
	if (key == ' ') {
		m_show_chart = !m_show_chart;
	}
	//Move the time of the song
	if (key == OF_KEY_LEFT) {
		float time = m_soundPlayer.getPositionMS();
		m_soundPlayer.setPositionMS(time-1000);
	}
	if (key == OF_KEY_RIGHT) {
		float time = m_soundPlayer.getPositionMS();
		m_soundPlayer.setPositionMS(time + 1000);
	}


}


void ofApp::drawChart() {
	ofBackground(m_backgorundColour);
	ofSetColor(255);
	//Volume levels
	m_font.drawString("Volume Level", 140, 50);

	ofDrawCircle(100, 100, m_audioAnalyser.getLeftLevel() * 100.0f);
	ofDrawCircle(200, 100, m_audioAnalyser.getRightLevel() * 100.0f);
	ofDrawCircle(300, 100, m_audioAnalyser.getMixLevel() * 100.0f);

	m_font.drawString("Left", 80, 170);
	m_font.drawString("Right", 180, 170);
	m_font.drawString("Mix", 290, 170);


	//FFT info by freq
	m_audioAnalyser.drawWaveform(40, 300, 1200, 90);//complex waves
	m_audioAnalyser.drawSpectrum(40, 460, 1200, 98); //Draw freq bins
	m_audioAnalyser.drawLinearAverages(40, 650, 1200, 128); //draw averaged bins
	
	//Getting the values again since changes are made to them when called in update
	float kick = m_audioAnalyser.getLinearAverage(0);
	float keys = m_audioAnalyser.getLinearAverage(4);
	float synth = m_audioAnalyser.getLinearAverage(2);

	//Normalize values
	m_circleBrightness_1 = ofMap(kick, 60.0f, 80.0f, 0.0f, 1.0f, true);
	m_circleBrightness_2 = ofMap(keys, 23.0f, 45.0f, 0.0f, 1.0f, true);
	m_circleBrightness_3 = ofMap(synth, 50.0f, 100.0f, 0.0f, 1.0f, true);

	//Draw the circles, based on value input
	//Note that the balls will not perfectly map to the sound because the ranges are specifically made to interact well with the environment. 
	//Values that map better in the GUI don't look as good in the generated world
	if (m_circleBrightness_1 != 0.0f) {
		ofSetColor(ofColor(255, 0, 0));
		ofDrawCircle(500, 100, 50);
		m_font.drawString("Drum", 500, 170);
	}
	if (m_circleBrightness_2 != 0.0f) {
		ofSetColor(ofColor(0, 255, 0));
		ofDrawCircle(650, 100, 50);
		m_font.drawString("Keyboard", 650, 170);
	}
	if (m_circleBrightness_3 != 0.0f) {
		ofSetColor(ofColor(0, 0, 255));
		ofDrawCircle(800, 100, 50);
		m_font.drawString("Synth", 800, 170);
	}

	//Getting the song time
	float timeInSeconds = m_soundPlayer.getPositionMS() / 1000.f;
	m_font.drawString("Song Time: " + ofToString(timeInSeconds), 400, 40);
}


void ofApp::generateObjects() {
	//Randomly Keep generating objects that move around the screen.
	if (std::chrono::steady_clock::now() > m_kickTimer && m_circleBrightness_1 >= 0.2)
	{
		m_circleBrightness_1 = 0.0f;
		ofVec3f randomPosition = { static_cast<float>(rand() % 2000 - 1000), static_cast<float>(rand() % 2000 - 1000), -1000.0f };
		m_kickTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);

		//Adding the new position to the array of locations to draw spheres
		m_spherePositions.push_back(randomPosition);
	}
	else {
		//always map the object value to 0 after it has been tested so the value is not kept in memory
		m_circleBrightness_1 = 0.0f;
	}

	//Editting the positions of every object so create the movement
	for (int pos = 0; pos < m_spherePositions.size(); pos++) {
		m_spherePositions[pos].set(m_spherePositions[pos].x, m_spherePositions[pos].y, m_spherePositions[pos].z += 5.0f);
	}

	//Creating the trail that goes behind the user
	//a box will be drawn at each position
	if (m_circleBrightness_3 != -1.0f) {
		m_userTrail.push_back(ofVec3f(0.0f, (m_circleBrightness_3 - 60.0f), 10.0f));
	}
	for (int pos = 0; pos < m_userTrail.size(); pos++) {
		m_userTrail[pos].set(m_userTrail[pos].x, m_userTrail[pos].y, m_userTrail[pos].z += 5.0f);

	}


	//Clearing the number of objects drawn to keep memory usage low
	if (m_spherePositions.size() > 100) {
		m_spherePositions.erase(m_spherePositions.begin());
	}
	if (m_userTrail.size() > 350) {
		m_userTrail.erase(m_userTrail.begin());
	}
}