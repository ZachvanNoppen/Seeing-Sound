
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(ofColor(200,200,200));

	ofSetVerticalSync(true);
	ofSetWindowShape(1400, 720);

	m_font.load("font.ttf", 12, true);

	m_soundPlayer.load("dryhope.mp3");
	m_soundPlayer.setLoop(true);
	m_soundPlayer.play();

	//IMPORTANT FOR ASS
	m_soundPlayer.setPositionMS(10000); //Lets you jump to certain points in the song

	m_audioAnalyser.init(&m_soundPlayer, 30);

	m_show_chart = false;

	//Setting up the camera
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableLighting();
	ofSetSmoothLighting(true);
	m_light.setPosition(500, 500, 0);

	m_userPos = ofVec3f(0, 0, 0);
	m_mod = 1;
	bg_r = 1;
	bg_g = 1;
	bg_b = 1;

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
	
	m_volume = m_audioAnalyser.getMixLevel();
	//
	float kick = m_audioAnalyser.getLinearAverage(0);
	float tone1 = m_audioAnalyser.getLinearAverage(4);
	float tone2 = m_audioAnalyser.getLinearAverage(2);
	float snare = m_audioAnalyser.getLinearAverage(16);

	//normalize values
	m_circleBrightness_1 = ofMap(kick, 60.0f, 80.0f, 0.0f, 1.0f, true);
	m_circleBrightness_2 = ofMap(tone1, 23.0f, 45.0f, 0.0f, 1.0f, true);
	//m_circleBrightness_2 = ofMap(tone1, 30.0f, 170.0f, 0.0f, 1.0f, true);
	m_circleBrightness_3 = ofMap(tone2, 50.0f, 100.0f, 0.0f, 60.0f, true);
	m_circleBrightness_4 = ofMap(snare, 13.0f, 23.0f, 0.0f, 1.0f, true);
	if (tone2 < 50.0f) {
		m_circleBrightness_3 = 30.0f;
	}
	//

	generateObjects();
	

	
	if (bg_r >= 255 || bg_r <= 0) { m_mod *= -1; }
	if (bg_g >= 255 || bg_g <= 0) { m_mod *= -1; }
	if (bg_b >= 255 || bg_b <= 0) { m_mod *= -1; }

	bg_r += 1 * m_mod;
	bg_g += 1 * m_mod;
	bg_b += 1 * m_mod;


	if (m_circleBrightness_2 >= 0.1f && m_pulseTimer < std::chrono::steady_clock::now() ) {
		m_backgorundColour = m_colourPallette[currentColour];
		
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

		bg_r = m_backgorundColour.r;
		bg_g = m_backgorundColour.g;
		bg_b = m_backgorundColour.b;

	}
	else {
		m_circleBrightness_2 = 0.0f;
		
	}
	
	m_backgorundColour = ofColor(bg_r, bg_g, bg_b);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (m_show_chart) {
		drawChart();
	}
	else {
		ofEnableDepthTest();
		m_cam.begin();
		m_light.enable();

		ofBackground(m_backgorundColour);
		//User position 
		//The camera always points at 0,0,0 so just make sure the other objects move above/below
		//that axis
		ofSetColor(m_colourPallette[userColour]);
		ofFill();
		ofDrawSphere(m_userPos, 30);


		//Demo object
		ofSetColor(m_colourPallette[sphereColour]);
		ofFill();
		for (ofVec3f pos : m_rectPositions) {

			ofDrawSphere(pos, 40 * (1 + m_volume));
		}
		ofSetColor(m_colourPallette[userColour]);
		ofFill();
		for (ofVec3f pos : m_userTrail) {

			ofDrawBox(pos, 6);
		}

		m_light.disable();
		m_cam.end();
		ofDisableDepthTest();
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		m_show_chart = !m_show_chart;
	}
}


void ofApp::drawChart() {
	ofSetColor(255,255,255);

	//Volume levels
	m_font.drawString("Volume Level", 140, 50);
	ofBackground(0, 0, 0);


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


	//
	

	
	ofSetColor(ofFloatColor(m_circleBrightness_1, 0, 0));
	ofDrawCircle(500, 100, 50);
	m_font.drawString("Drum", 500, 170);

	ofSetColor(ofFloatColor(0, m_circleBrightness_2, 0));
	ofDrawCircle(650, 100, 50);
	m_font.drawString("Tone1", 650, 170);

	ofSetColor(ofFloatColor(0, 0, m_circleBrightness_3));
	ofDrawCircle(800, 100, 50);
	m_font.drawString("Tone2", 800, 170);

	ofSetColor(ofFloatColor(m_circleBrightness_4, m_circleBrightness_4, m_circleBrightness_4));
	ofDrawCircle(950, 100, 50);
	m_font.drawString("Snare", 900, 170);

	//song time
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

		ofSetColor(255, 0, 255);
		ofFill();

		m_rectPositions.push_back(randomPosition);
	}
	else {
		m_circleBrightness_1 = 0.0f;
	}


	if (m_rectPositions.size() > 100) {
		m_rectPositions.erase(m_rectPositions.begin());
	}


	//Moving all objects 
	//Here we will move the object forward
	for (int pos = 0; pos < m_rectPositions.size(); pos++) {
		m_rectPositions[pos].set(m_rectPositions[pos].x, m_rectPositions[pos].y, m_rectPositions[pos].z += 5.0f);
	}

	//User Trail
	cout << m_userTrail.size() << endl;
	if (m_circleBrightness_3 != 30.0f) {
		m_userTrail.push_back(ofVec3f(0.0f, (m_circleBrightness_3 - 30.0f), 10.0f));
	}
	for (int pos = 0; pos < m_userTrail.size(); pos++) {
		m_userTrail[pos].set(m_userTrail[pos].x, m_userTrail[pos].y, m_userTrail[pos].z += 5.0f);

	}
	if (m_userTrail.size() > 350) {
		m_userTrail.erase(m_userTrail.begin());
	}
}