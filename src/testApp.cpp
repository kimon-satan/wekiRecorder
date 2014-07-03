#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	// listen on the given port
    
    inport = 14159;
    outport = 57120;
	cout << "listening for osc messages on port " << inport << "\n";
    
    m_host = "localhost";

	current_msg_string = 0;
    
    resetSenders();
    
    //could add code to make directory here
    
    m_dirName = ofGetTimestampString();
    m_dir.createDirectory(m_dirName);
    fileName = ofGetTimestampString() + "_";
    fIndex = 0;
    isFileWritten = true;
    
    

}

void testApp::resetSenders(){
    receiver.setup(inport);
    sender.setup(m_host, outport);
}

//--------------------------------------------------------------
void testApp::update(){
    
    if(ofGetSeconds()%30 == 0){
        if(!isFileWritten){
            fIndex += 1;
            jsonObj.save(m_dirName + "/" + fileName + ofToString(fIndex, 0) + ".json" , true);
            jsonObj.clear();
            cout << "writeFile" << endl;
            isFileWritten = true;
        }
    }else{
        isFileWritten = false;
    }

	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
    
		receiver.getNextMessage(&m);
        
        vector<string> args;
        
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        for(int i = 0; i < m.getNumArgs(); i++){

            // display the argument - make sure we get the right type
            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                msg_string += ofToString(m.getArgAsInt32(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                msg_string += ofToString(m.getArgAsFloat(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                msg_string += m.getArgAsString(i);
            }
            
            msg_string += " , ";
            
        }
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
        
        vector<string> messObj;
        messObj.push_back(msg_string);
        messObj.push_back(ofToString(ofGetSystemTimeMicros()));

        if(m_devices.find(m.getRemoteIp()) != m_devices.end()){
            w_device td = m_devices[m.getRemoteIp()];
        
            jsonObj[td.host]["messages"][ofGetTimestampString()].append(msg_string);
           
        }else{
            w_device td;
            td.host = m.getRemoteIp();
            td.delta = ofGetElapsedTimef() - m.getArgAsFloat(1);
            m_devices[td.host] = td;
            
            jsonObj[td.host]["messages"][ofGetTimestampString()].append(msg_string);
         
        }
        
        sender.sendMessage(m);

	}
}





//--------------------------------------------------------------
void testApp::draw(){


    
    ofBackground(100);
    
    ofSetColor(255, 255, 0);
    
    ofTranslate(20, 20);
    ofDrawBitmapString("Press enter to reset senders and receivers", 0, 0);
    ofTranslate(0, 20);
    (keyEnterMode == 0)? ofSetColor(255) : ofSetColor(0); ofDrawBitmapString("inport: " + ofToString(inport,0), 0,0);
    ofTranslate(0, 20);
    (keyEnterMode == 1)? ofSetColor(255) : ofSetColor(0); ofDrawBitmapString("outport: " + ofToString(outport,0), 0,0);
    ofTranslate(0, 20);
    (keyEnterMode == 2)? ofSetColor(255) : ofSetColor(0); ofDrawBitmapString("host: " + m_host, 0,0);
    ofTranslate(0, 50);
    
    ofSetColor(255, 255, 0);
    ofDrawBitmapString("Registered Devices", 0, 0);
    ofTranslate(0, 30);
    map<string, w_device >::iterator it;
    for(it = m_devices.begin(); it !=  m_devices.end(); it++){
        ofDrawBitmapString(it->first, 0,0);
        ofTranslate(0, 20);
    }
    
    ofTranslate(0, 50);
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		ofDrawBitmapString(msg_strings[i], 0,20 * i);
	}



}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key == OF_KEY_DOWN){
        keyEnterMode = (keyEnterMode + 1)%3;
    }else if(key == OF_KEY_UP){
        keyEnterMode = max(0, keyEnterMode - 1);
    }else if(key == OF_KEY_RETURN){
        resetSenders();
    }else{
            
        if(keyEnterMode == 2){
            
            if(key == OF_KEY_BACKSPACE){
                
                if(m_host.size() > 0)m_host.erase(m_host.end()-1);
            }else{
                m_host += key;
            }
        }else if(keyEnterMode == 0){
            
            string str = ofToString(inport,0);
            
            if(key == OF_KEY_BACKSPACE){
                if(str.size() > 0)str.erase(str.end()-1);
            }else if(key >= 48 && key <= 57){
                if(str.size() < 6)str += key;
            }
            
            inport =  ofToInt(str);
            
            
        }else if(keyEnterMode == 1){
            
            string str = ofToString(outport,0);
            
            if(key == OF_KEY_BACKSPACE){
                if(str.size() > 0)str.erase(str.end()-1);
            }else if(key >= 48 && key <= 57){
                if(str.size() < 6)str += key;
            }
            
            outport =  ofToInt(str);
            
        }
        
    }
    
   
    

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

void testApp::exit(){
    
    fIndex += 1;
    jsonObj.save(m_dirName + "/" + fileName + ofToString(fIndex, 0) + ".json" , true);

}
