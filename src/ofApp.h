#pragma once

#include "ofMain.h"
#include "AudioAnalyser.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void drawChart();

		ofFmodSoundPlayer m_soundPlayer; //loads and plays the music
		ofSoundPlayer m_soundPlayerOF;
		AudioAnalyser m_audioAnalyser;
		ofTrueTypeFont m_font;

		float m_circleBrightness_1;
		float m_circleBrightness_2;
		float m_circleBrightness_3;
		float m_circleBrightness_4;

		bool m_show_chart;

		ofEasyCam m_cam;

		ofLight m_light;

		ofVec3f m_userPos;
		
};
