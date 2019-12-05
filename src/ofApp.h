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
		void generateObjects();

		ofFmodSoundPlayer m_soundPlayer;
		ofSoundPlayer m_soundPlayerOF;
		AudioAnalyser m_audioAnalyser;
		ofTrueTypeFont m_font;

		float m_volume;
		float m_circleBrightness_1;
		float m_circleBrightness_2;
		float m_circleBrightness_3;

		ofEasyCam m_cam;
		ofLight m_light;
		ofLight m_textLight;


		ofVec3f m_userPos;
		vector<ofVec3f> m_spherePositions;
		vector<ofVec3f> m_userTrail;

		std::chrono::steady_clock::time_point m_kickTimer;
		std::chrono::steady_clock::time_point m_pulseTimer;

		vector<ofColor> m_colourPallette;
		ofColor m_backgorundColour;
		int bg_r, bg_g, bg_b;
		int currentColour, userColour, sphereColour;
		int m_mod;
		
		bool m_show_chart;
		
		
};
