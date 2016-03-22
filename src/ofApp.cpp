#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    initializeAudio();
    initializeModels();
    
    instructions.loadFont("RobotoTTF/Roboto-Bold.ttf", 20);
    header.loadFont("quantico/Quantico-Bold.ttf", 30);
    headerLG.loadFont("quantico/Quantico-Bold.ttf", 70);
    preludeFont.loadFont("RobotoTTF/Roboto-Bold.ttf", 20);
    
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    
    camera.setFov(80);
    camera.setDistance(600);
    
    //we need to call this for textures to work on models
    ofDisableArbTex();
    
    //this makes sure that the back of the model doesn't show through the front
    ofEnableDepthTest();
    
    //ofxVboParticles([max particle number], [particle size]);
    stars = new ofxVboParticles(10000, 1000);
    
    //set friction (0.0 - 1.0);
    stars->friction = 0.000;
    
    movie.loadMovie("displays/loading.gif");
    movie.setLoopState(OF_LOOP_NORMAL);
    movie.play();
    
    isIntro = true;
    skipPrelude = true; // only set to true when testing
    
    timers[0] = 0;
    timers[1] = 0;
    timers[2] = 0;
    
    time = ofGetElapsedTimef();
}

void ofApp::initializeModels(){
    
    satellite.loadModel("HST-3DS/hst.3ds");
    satellite.setScale(1.9, 1.9, 1.9);
    satellite.setPosition(0, 0, -1000);
    satellite.setRotation(0, 30, 1, 1, 0);
    
    float scale = 0.5;
    
    modelA.loadModel("astronaut1-A/astronaut1.3ds");
    modelA.setScale(scale, scale, scale);
    modelA.setRotation(0, 180, 0, 0, 1);
    
    modelB.loadModel("astronaut1-B/astronaut1.3ds");
    modelB.setScale(scale, scale, scale);
    modelB.setRotation(0, 180, 0, 0, 1);
    
    modelC.loadModel("astronaut1-C/astronaut1.3ds");
    modelC.setScale(scale, scale, scale);
    modelC.setRotation(0, 180, 0, 0, 1);
    
    astronauts[0] = modelA;
    astronauts[1] = modelB;
    astronauts[2] = modelC;
    
    hudA.loadImage("displays/alice2.png");
    hudB.loadImage("displays/bob2.png");
    hudC.loadImage("displays/charlie2.png");
    
    hud[0] = hudA;
    hud[1] = hudB;
    hud[2] = hudC;
    
    preludeDisplay.loadImage("displays/prelude.png");
    hintDisplay.loadImage("displays/hint.png");
}


//--------------------------------------------------------------
void ofApp::initializeAudio(){
    audioA.loadSound("sounds/alice.mp3");
    audioB.loadSound("sounds/bob.mp3");
    audioC.loadSound("sounds/charlie.mp3");
    
    prelude.loadSound("sounds/prelude.mp3");
    
    audioA.setVolume(1.0f);
    audioB.setVolume(0.0f);
    audioC.setVolume(0.0f);
    
    audioTracks[0] = audioA;
    audioTracks[1] = audioB;
    audioTracks[2] = audioC;
    
    currentAudio = 0;
    
    prelude.setVolume(1.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();
    updateStars();
    movie.update();
    
    satellite.setRotation(1, 270 + (ofGetElapsedTimef()*10), 1, 0, 1);
    
    time += ofGetElapsedTimef() - time;
    
    bool audioPlaying = false;
    for (int i = 0; i < 3; i++) {
        if (audioTracks[i].getIsPlaying()) { audioPlaying = true; }
    }
    
    // when all the audio has been played, print stats to the console
    if (isReady && !audioPlaying && !printedStats) {
        printStats();
        printedStats = true;
    }
    
    // checks to see if the prelude is over, if so start the main animation
    if (isPrelude && ((time-timer) > 15.0f)) {
        isPrelude = false;
        isReady = true;
        audioA.play();
        audioB.play();
        audioC.play();
    }
}

//--------------------------------------------------------------
void ofApp::updateStars(){
    for (int i = 0; i < 3; i++) {
        ofVec3f position = ofVec3f(0, 0, 0);
        ofVec3f velocity = ofVec3f(ofRandom(-2, 2), ofRandom(-2, 2), ofRandom(-2, 2));
        ofColor white = ofColor::fromHex(0xffffff);
        stars->addParticle(position, velocity, white); // add a particle
    }
    stars->update();
}

//--------------------------------------------------------------
void ofApp::updateAudio(int next){
    
    int curr = (currentAudio + next) % 3;
    if (curr == -1) { curr = 2; }
    
    currentAudio = curr;
    numSwitches++;
    
    for (int i = 0; i < 3; i++) {
        float volume = (i == curr) ? 1.0f : 0.0f;
        audioTracks[i].setVolume(volume);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float xOffsetA = (20.0f*(sin(time)));
    float yOffsetA = (20.0f*(cos(time)));
    
    float xOffsetB = (20.0f*(sin(time+1.0f)));
    float yOffsetB = (20.0f*(cos(time+2.0f)));
    
    float xOffsetC = (20.0f*(sin(time+3.0f)));
    float yOffsetC = (20.0f*(cos(time+4.0f)));
    
    modelA.setPosition(-300 + xOffsetA, 400 + yOffsetA, -250);
    modelB.setPosition(500 + xOffsetB, -350 + yOffsetB, -350);
    modelC.setPosition(400 + xOffsetC, 300 + yOffsetC, -100);
    
    if (isIntro) {
        drawIntroGraphics();
        return;
    }
    
    if (isPrelude) {
        drawPreludeGraphics();
        return;
    }
    
    ofColor black = ofColor::fromHex(0x000000);
    ofColor darkBlue = ofColor::fromHex(0x000040);
    
    ofBackgroundGradient(darkBlue, black);
    
    timers[currentAudio]++;  // used to time monologues
    
    ofSetColor(255,255,255);
    camera.begin();
    
    modelA.drawFaces();
    modelB.drawFaces();
    modelC.drawFaces();
    
    satellite.drawFaces();
    
    stars->draw();
    
    camera.end();
    
    double hudSize = ofGetWidth() * .08;
    double hudH = (hudSize * 3.0);
    double gutter = 20.0;
    
    hud[currentAudio].draw(gutter,ofGetHeight()-gutter-hudH, hudSize*4.0, hudH);
    
    double hintSize = 75;
    double g = 5.0;
    double hintH = (ofGetWidth() * 9.00) / hintSize;
    double hintW = (ofGetWidth() * 16.0) / hintSize;
    
    hintDisplay.draw(ofGetWidth()-g-hintW,g, hintW, hintH);
}

//--------------------------------------------------------------
void ofApp::drawIntroGraphics(){
    
    ofPushStyle();
    
    float xOffset = (20.0f*(sin(time)));
    float yOffset = (20.0f*(cos(time)));
    
    double xStart = ofGetWidth() / 2.0;
    double yStart = ofGetHeight() / 2.0;
    
    ofBackground(0,0,0);
    
    string strA = "welcome to";
    float aX = (xStart-(header.stringWidth(strA)*0.5f));
    string strB = "THE SPACEWALK";
    float bX = (xStart-(headerLG.stringWidth(strB)*0.5f));
    string strC = "please put on your headphones...";
    float cX = (xStart-(header.stringWidth(strC)*0.5f));
    string strD = "and click anywhere to begin...";
    float dX = (xStart-(header.stringWidth(strD)*0.5f));
    
    ofSetHexColor(0xffffff);
    header.drawString(strA,aX + xOffset, 100);
    headerLG.drawString(strB,bX + xOffset, 200);
    header.drawString(strC,cX + xOffset, 300);
    header.drawString(strD,dX + xOffset, 350);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawPreludeGraphics(){
    
    ofPushStyle();
    
    ofSetHexColor(0xffffff);
    
    double xStart = ofGetWidth() / 2.0;
    double yStart = ofGetHeight() / 2.0;
    
    double sizeConst = ofGetWidth() * .15;
    double hudW = (sizeConst * 4.0);
    double hudH = (sizeConst * 3.0);
    double xPos = xStart - (hudW / 2.0);
    //double yPos = yStart - (hudH / 2.0);
    double yPos = 20;
    
    preludeDisplay.draw(xPos, yPos, hudW, hudH);
    
    double gifW = movie.getWidth();
    double gifH = movie.getHeight();
    double gifX = xStart-(gifW / 2.0);
    double gifY = yPos + hudH;
    
    movie.draw(gifX,gifY);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::printStats(){
    cout << "# OF PERSPECTIVE SWITCHES: " << ofToString(numSwitches) << endl;
    cout << "TIMER - ALICE STORY:   " << ofToString(timers[0]) << endl;
    cout << "TIMER - BOB STORY:     " << ofToString(timers[1]) << endl;
    cout << "TIMER - CHARLIE STORY: " << ofToString(timers[2]) << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case OF_KEY_LEFT:
            updateAudio(-1);
            break;
        case OF_KEY_RIGHT:
            updateAudio(1);
            break;
    }
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
    if (isIntro) {
        isIntro = false;
        if (!skipPrelude) {
            isPrelude = true;
            prelude.play();
            timer = ofGetElapsedTimef();
        } else {
            isReady = true;
            audioA.play();
            audioB.play();
            audioC.play();
        }
    }
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
