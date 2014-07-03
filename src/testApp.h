#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSONElement.h"

// listen on port 12345

#define NUM_MSG_STRINGS 5

struct w_device{
    
    string host;
    string name;
    float delta;
    
};

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
        void resetSenders();

		ofTrueTypeFont font;
		ofxOscReceiver receiver;
        ofxJSONElement jsonObj;
 
        int fIndex;
        bool isFileWritten;
    
        int keyEnterMode;
    
        map<string, w_device> m_devices;

		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];
    
        string fileName, m_dirName;
        ofDirectory m_dir;
        
    

        ofxOscSender sender;
        
        int inport;
        int outport;
        string m_host;
    
        

};
