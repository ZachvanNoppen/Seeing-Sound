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
		vector<ofVec3f> m_rectPositions;
		vector<ofVec3f> m_userTrail;

		std::chrono::steady_clock::time_point m_kickTimer;
		std::chrono::steady_clock::time_point m_pulseTimer;


		ofColor m_backgorundColour;
		int bg_r, bg_g, bg_b;

		int m_mod;

		vector<ofColor> m_colourPallette;
		int currentColour;
		int userColour;
		int sphereColour;
		float m_volume;
};
