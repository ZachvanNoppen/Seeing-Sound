
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
	m_soundPlayer.setPositionMS(1800); //Lets you jump to certain points in the song

	m_audioAnalyser.init(&m_soundPlayer, 30);

	m_show_chart = false;

	//Setting up the camera
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableLighting();
	ofSetSmoothLighting(true);
	m_light.setPosition(500, 0, 1500);

	m_userPos = ofVec3f(0, 0, 0);

}

//--------------------------------------------------------------
void ofApp::update(){
	m_audioAnalyser.update();

	//Here we will move the object forward
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (m_show_chart) {
		drawChart();
	}
	ofEnableDepthTest();
	m_cam.begin();
	m_light.enable();

	//User position 
	//The camera always points at 0,0,0 so just make sure the other objects move above/below
	//that axis
	ofSetColor(ofColor::blue);
	ofFill();
	ofDrawSphere(m_userPos, 30);

	//Demo object
	ofSetColor(ofColor::green);
	ofFill();
	ofDrawSphere(ofVec3f(0,200,0), 30);

	m_light.disable();
	m_cam.end();
	ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		m_show_chart = !m_show_chart;
	}
}


void ofApp::drawChart() {
	//ofBackground(ofColor(0, 0, 0, 128));
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


	//
	float kick = m_audioAnalyser.getLinearAverage(0);
	float tone1 = m_audioAnalyser.getLinearAverage(4);

	float tone2 = m_audioAnalyser.getLinearAverage(2);
	float snare = m_audioAnalyser.getLinearAverage(16);

	//normalize values
	m_circleBrightness_1 = ofMap(kick, 60.0f, 80.0f, 0.0f, 1.0f, true);
	m_circleBrightness_2 = ofMap(tone1, 23.0f, 45.0f, 0.0f, 1.0f, true);
	m_circleBrightness_3 = ofMap(tone2, 50.0f, 100.0f, 0.0f, 1.0f, true);
	m_circleBrightness_4 = ofMap(snare, 13.0f, 23.0f, 0.0f, 1.0f, true);


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