#pragma once

#include "ofMain.h"
#include "ofxVboParticles.h"
#include "ofxAssimpModelLoader.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void drawIntroGraphics();
        void drawPreludeGraphics();
    
        void initializeAudio();
        void initializeModels();
    
        void updateAudio(int next);
        void updateStars();
    
        void printStats();
        ofVideoPlayer  movie;
    
        ofSoundPlayer  audioA;
        ofSoundPlayer  audioB;
        ofSoundPlayer  audioC;
        ofSoundPlayer  prelude;
    
        ofSoundPlayer  audioTracks[3];
        ofxAssimpModelLoader astronauts[3];
        ofImage hud[3];
        long unsigned timers[3];

        ofxVboParticles *stars;
    
        ofxAssimpModelLoader modelA;
        ofxAssimpModelLoader modelB;
        ofxAssimpModelLoader modelC;
        ofxAssimpModelLoader satellite;
    
        int numSwitches;
        long unsigned timerA;
        long unsigned timerB;
        long unsigned timerC;
    
        int currentAudio;
    
        ofImage hudA;
        ofImage hudB;
        ofImage hudC;
    
        ofImage preludeDisplay;
        ofImage hintDisplay;
    
        ofLight pointLight;
        ofLight pointLight2;
        ofLight pointLight3;
    
        bool isIntro;
        bool isPrelude;
        bool isReady;
        bool skipPrelude;
        bool printedStats;
    
        ofEasyCam camera;
    
        float time;             // keeps track of the elapsed time
        float timer;            // timer that starts after the into
    
        ofTrueTypeFont header;
        ofTrueTypeFont headerLG;
        ofTrueTypeFont instructions;
        ofTrueTypeFont preludeFont;
};
