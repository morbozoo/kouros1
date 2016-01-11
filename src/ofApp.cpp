#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    editModeOn = false;
    enableDeleteOutConnect = false;
    enableDeleteInConnect  = false;
    
    
    
    for (int i = 0; i < 5; i++) {
        Modulo mod;
        modules.push_back(mod);
    }
    
    nodoCount = 0;
    count = 0;
    soundSelector = 1;
    for(int i = 0; i < modules.size(); i ++){
        soundBanks.push_back(soundSelector);
    }
//    mod.setup();
//    for (int i = 0; i < NUM_CASILLAS; i ++){
//        Casilla casilla(0, 0, i + 1, false);
//        Nodo nodo(true, true, nodoCount, false);
//        mod.casillas.push_back(casilla);
//        count++;
//        mod.casillas[i].inputs.push_back(nodo);
////        mod.casillas[i].inputs.back().isDefault = true;
////        mod.casillas[i].inputs.back().inOu = true;
////        mod.casillas[i].inputs[0].setId(nodoCount);
////        mod.casillas[i].inputs[0].isSelected = false;
////        nodoCount ++;
//    }
    
    selected.set(-1, .1);
    
    enableInConnect = false;
    enableOutConnect = false;
    first.set(-1, -1, -1);
    second.set(-1, -1, -1);
    loadJSON();
    setupGUI();

}

//--------------------------------------------------------------
void ofApp::update(){
    if (editModeOn) {
        ofBackground(180, 180, 180);
    }else{
        ofBackground(30, 30, 30);
    }
    for (int i = 0; i < cola.size(); i++) {
        ofDrawBitmapString(ofToString(cola[i]), 0, 10);
        if (ofGetElapsedTimef() >= cola[i][3]) {
            play(modules[cola[i][0]].casillas[cola[i][1]].play(editModeOn, cola[i][2]));
            cola.erase(cola.begin());
        }
    }
    for (int i = 0; i < modules.size(); i++) {
        soundBanks[i] = soundSlider[i];
    }
    
}

//--------------------------------------------------------------
void ofApp::loadSound(){
    ofLogNotice("ofApp::setup") << "loading sound.";
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < modules[i].casillas.size(); j++) {
            modules[i].casillas[j].updatePlayer(soundBanks[i]);
        }
    }
}


//--------------------------------------------------------------
void ofApp::setupGUI(){
    
    gui.setup("GUI");
    for (int i = 0; i < modules.size(); i++) {
        ofxSlider<int> slider;
        soundSlider.push_back(slider);
    }
    
    gui.add(velSlider.setup("Velocity", 0.5f, 0.0f, 1.0f));
    for (int i = 0; i < modules.size(); i++) {
        gui.add(soundSlider[i].setup("Sound" + ofToString(i), 1, 1, 16));
    }
    
    mHideGUI = true;

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofScale(.75, .75);
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < NUM_CASILLAS; j++) {
            modules[i].casillas[j].draw();
        }
    }
    
    
    ofPushStyle();
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < NUM_CASILLAS; j ++) {
            for (int k = 0; k < modules[i].casillas[j].outputs.size(); k++) {
                for (int l = 0; l < modules[i].casillas[j].outputs[k].outConnections.size(); l++) {
                    ofSetColor(0, 0, 153, 150);
                    float p1X = modules[i].casillas[j].x + SIZE_CASILLA + SIZE_CASILLA/4 + k*SIZE_CASILLA/2;
                    float p1Y = modules[i].casillas[j].y + SIZE_CASILLA/4;
                    float p1 = modules[i].casillas[j].outputs[k].outConnections[l].x;
                    float p2 = modules[i].casillas[j].outputs[k].outConnections[l].y;
                    float p3 = modules[i].casillas[j].outputs[k].outConnections[l].z;
                    float p2X = modules[p1].casillas[p2].x + SIZE_CASILLA + SIZE_CASILLA/4 + p3*SIZE_CASILLA/2;
                    float p2Y = modules[p1].casillas[p2].y + SIZE_CASILLA - SIZE_CASILLA/4;
                    ofSetLineWidth(3);
                    ofLine(p1X, p1Y, p2X, p2Y);
                }
            }
        }
    }
    
    ofPopStyle();
    
    ofPopMatrix();

    ofPushStyle();
    ofSetColor(255, 0, 0);
//    for (int i = 0; i < cola.size(); i++) {
//        ofDrawBitmapString(ofToString(cola[i]), 150, 20);
//    }
//    
//    for (int i = 0; i < copys.size(); i++) {
//        ofDrawBitmapString(ofToString(copys[i]), 150, 120);
//    }
//    //for (int i = 0; i < modules[0].casillas[9].outputs.size(); i++) {
//        for (int j = 0 ; j < modules[0].casillas[9].outputs[2].outConnections.size(); j++) {
//            ofDrawBitmapString(ofToString(modules[0].casillas[9].outputs[2].outConnections[j]), 0, 10 + 10);
//            ofDrawBitmapString(ofToString(modules[0].casillas[10].outputs[7].outConnections[j]), 70, 10 + 10);
//        }
//   // }
//    
//    //for (int i = 0; i < modules[0].casillas[9].inputs.size(); i++) {
//        for (int j = 0 ; j < modules[0].casillas[9].inputs[2].inConnections.size(); j++) {
//            ofDrawBitmapString(ofToString(modules[0].casillas[9].inputs[2].inConnections[j]) + ": " + ofToString(2), 0, 40 + 10);
//             ofDrawBitmapString(ofToString(modules[0].casillas[10].inputs[5].inConnections[j]) + ": " + ofToString(5), 70, 40 + 10);
//        }
//    //}

    ofPopStyle();
    
    if( !mHideGUI ){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'i'){
        enableInConnect = true;
    }else if(key == 'o'){
        enableOutConnect = true;
    }else if(key == 's'){
        saveJSON();
    }else if(key == 'e'){
        editModeOn = !editModeOn;
    }else if(key == 'p'){
        play(modules[cola[0][0]].casillas[cola[0][1]].play(editModeOn, cola[0][2]));
    }else if(key == 'd'){
        enableDeleteInConnect = true;
    }else if(key == 'r'){
        enableDeleteOutConnect = true;
    }else if(key == 'c'){
        copy();
    }else if(key == 'g'){
        mHideGUI = !mHideGUI;
    }else if(key == 'l'){
        loadSound();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'i'){
        enableInConnect = false;
        if (first.x != -1 && first.y != -1 && first.z != -1 && second.x != -1 && second.y != -1 && second.z != -1 && first.x == second.x && first.y == second.y) {
            modules[first.x].casillas[first.y].inputs[first.z].inConnections.push_back(second.z);
            first.set(-1, -1, -1);
            second.set(-1, -1, -1);
        }else{
            first.set(-1, -1, -1);
            second.set(-1, -1, -1);
        }
    }else if(key == 'o'){
        enableOutConnect = false;
        ofLogNotice("ofApp::setup") << "denrto del if.";
        if (first.x != -1 && first.y != -1 && first.z != -1 && second.x != -1 && second.y != -1 && second.z != -1 && (first.x != second.x || first.y != second.y) && second.z != 0) {
            ofLogNotice("ofApp::setup") << "denrto del if.2";
            modules[first.x].casillas[first.y].outputs[first.z].outConnections.push_back(second);
            first.set(-1, -1, -1);
            second.set(-1, -1, -1);
        }else{
            first.set(-1, -1, -1);
            second.set(-1, -1, -1);
        }
    }else if(key == 'd'){
        enableDeleteInConnect = false;
        if (first.x != -1 && first.y != -1 && first.z != -1 && second.x != -1 && second.y != -1 && second.z != -1 && first.x == second.x && first.y == second.y){
            for (int i = 0; i < modules[first.x].casillas[first.y].inputs[first.z].inConnections.size(); i++) {
                if(modules[first.x].casillas[first.y].inputs[first.z].inConnections[i] == second.z){
                  modules[first.x].casillas[first.y].inputs[first.z].inConnections.erase(modules[first.x].casillas[first.y].inputs[first.z].inConnections.begin() + i);
                }
            }
        }
        first.set(-1, -1, -1);
        second.set(-1, -1, -1);
        
    }else if(key == 'r'){
        enableDeleteOutConnect = false;
        if (first.x != -1 && first.y != -1 && first.z != -1 && second.x != -1 && second.y != -1 && second.z != -1){
            for (int i = 0; i < modules[first.x].casillas[first.y].outputs[first.z].outConnections.size(); i++) {
                if(modules[first.x].casillas[first.y].outputs[first.z].outConnections[i].x == second.x
                   && modules[first.x].casillas[first.y].outputs[first.z].outConnections[i].y == second.y
                   && modules[first.x].casillas[first.y].outputs[first.z].outConnections[i].z == second.z){
                    modules[first.x].casillas[first.y].outputs[first.x].outConnections.erase(modules[first.x].casillas[first.y].outputs[first.z].outConnections.begin() + i);
                }
            }
        }
        first.set(-1, -1, -1);
        second.set(-1, -1, -1);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    x=x*4/3;
    y=y*4/3;
    if (selected.x >= -1 && selected.y > -1 && editModeOn) {
        if(modules[selected.x].casillas[selected.y].isSelected){
            modules[selected.x].casillas[selected.y].updatePos(x - (SIZE_CASILLA/2), y - (SIZE_CASILLA/2));
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    x=x*4/3;
    y=y*4/3;
    if (editModeOn) {
        for (int i = 0; i < modules.size(); i++) {
            for (int j = 0; j < NUM_CASILLAS; j++) {
                if (x > modules[i].casillas[j].x && x < modules[i].casillas[j].x + SIZE_CASILLA
                    && y > modules[i].casillas[j].y && y < modules[i].casillas[j].y + SIZE_CASILLA) {
                    modules[i].casillas[j].isSelected = true;
                    modules[i].casillas[j].play(editModeOn, -1);
                    selected.set(i, j);
                    if (x > modules[i].casillas[j].x + INPUT_X_POS && x < modules[i].casillas[j].x + INPUT_X_POS + SIZE_CUAD
                        && y > modules[i].casillas[j].y + INPUT_Y_POS && y < modules[i].casillas[j].y + INPUT_Y_POS + SIZE_CUAD) {
                        Nodo nodo(true, true, nodoCount);
                        modules[i].casillas[j].inputs.push_back(nodo);
                        modules[i].casillas[j].inputs.back().setId(nodoCount);
                        modules[i].casillas[j].inputs.back().isDefault = false;
                        modules[i].casillas[j].inputs.back().inOu = true;
                        modules[i].casillas[j].inputs.back().isSelected = false;
                        nodoCount ++;
                    }
                    if (x > modules[i].casillas[j].x + OUTPUT_X_POS && x < modules[i].casillas[j].x + OUTPUT_X_POS + SIZE_CUAD
                        && y > modules[i].casillas[j].y + OUTPUT_Y_POS && y < modules[i].casillas[j].y + OUTPUT_Y_POS + SIZE_CUAD) {
                        Nodo nodo(true, true, nodoCount);
                        modules[i].casillas[j].outputs.push_back(nodo);
                        modules[i].casillas[j].outputs.back().setId(nodoCount);
                        modules[i].casillas[j].outputs.back().isDefault = false;
                        modules[i].casillas[j].outputs.back().inOu = false;
                        modules[i].casillas[j].outputs.back().isSelected =  false;
                        nodoCount ++;
                    }
                }
            }
        }
        
        if (enableInConnect || enableDeleteInConnect) {
            for (int i = 0; i < modules.size(); i++) {
                for (int j = 0; j < NUM_CASILLAS; j++) {
                    for (int k = 0; k < modules[i].casillas[j].inputs.size(); k++) {
                        if (x > modules[i].casillas[j].x + SIZE_CASILLA + k*SIZE_CASILLA/2 && x < modules[i].casillas[j].x + SIZE_CASILLA + SIZE_CASILLA/2 + k*SIZE_CASILLA/2
                            && y > modules[i].casillas[j].y + SIZE_CASILLA/2 && y < modules[i].casillas[j].y + SIZE_CASILLA) {
                            
                            if (first.x == -1 && first.y == -1 && first.z == -1) {
                                first.set(i, j, k);
                            }
                            break;
                        }
                    }
                    for (int k = 0; k < modules[i].casillas[j].outputs.size(); k++) {
                        if (x > modules[i].casillas[j].x + SIZE_CASILLA + k*SIZE_CASILLA/2 && x < modules[i].casillas[j].x + SIZE_CASILLA + SIZE_CASILLA/2 + k*SIZE_CASILLA/2
                            && y > modules[i].casillas[j].y && y < modules[i].casillas[j].y + SIZE_CASILLA/2) {
                            if (first.x != -1 && first.y != -1 && first.z != -1) {
                                second.set(i, j, k);
                            }
                            break;
                        }
                    }
                }
            }
        }
        
        if (enableOutConnect || enableDeleteOutConnect) {
            for (int i = 0; i < modules.size(); i++) {
                for (int j = 0; j < NUM_CASILLAS; j++) {
                    for (int k = 0; k < modules[i].casillas[j].outputs.size(); k++) {
                        if (x > modules[i].casillas[j].x + SIZE_CASILLA + k*SIZE_CASILLA/2 && x < modules[i].casillas[j].x + SIZE_CASILLA + SIZE_CASILLA/2 + k*SIZE_CASILLA/2
                            && y > modules[i].casillas[j].y && y < modules[i].casillas[j].y + SIZE_CASILLA/2) {
                            if (first.x == -1 && first.y == -1 && first.z == -1) {
                                first.set(i, j, k);
                            }
                            break;
                        }
                    }
                    for (int k = 0; k < modules[i].casillas[j].inputs.size(); k++) {
                        if (x > modules[i].casillas[j].x + SIZE_CASILLA + k*SIZE_CASILLA/2 && x < modules[i].casillas[j].x + SIZE_CASILLA + SIZE_CASILLA/2 + k*SIZE_CASILLA/2
                            && y > modules[i].casillas[j].y + SIZE_CASILLA/2 && y < modules[i].casillas[j].y + SIZE_CASILLA) {
                            if (first.x != -1 && first.y != -1 && first.z != -1) {
                                second.set(i, j, k);
                            }
                            break;
                        }
                    }
                }
            }
            
        }
    }else{
        for (int i = 0; i < modules.size(); i++) {
            for (int j = 0; j < NUM_CASILLAS; j++) {
                for (int k = 0; k < modules[i].casillas[j].inputs.size(); k++) {
                    if (x > modules[i].casillas[j].x + SIZE_CASILLA + k*SIZE_CASILLA/2 && x < modules[i].casillas[j].x + SIZE_CASILLA + SIZE_CASILLA/2 + k*SIZE_CASILLA/2 && y > modules[i].casillas[j].y + SIZE_CASILLA/2 && y < modules[i].casillas[j].y + SIZE_CASILLA) {
                        play(modules[i].casillas[j].play(editModeOn, k));
                        break;
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::play(vector<ofVec3f> targets){
    copys = targets;
    float when = ofGetElapsedTimef() + (1-velSlider);
    for (int i = 0; i < targets.size(); i++) {
        ofDrawBitmapString(ofToString(targets[i]),0, 120);
        vector<float> temp;
        temp.push_back(targets[i].x);
        temp.push_back(targets[i].y);
        temp.push_back(targets[i].z);
        temp.push_back(when);
        cola.push_back(temp);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < NUM_CASILLAS; j++) {
            modules[i].casillas[j].isSelected = false;
            selected.set(-1, -1);
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
void ofApp::copy(){
    for (int i = 1; i < modules.size(); i++) {
        for (int j = 0; j < modules[i].casillas.size(); j++) {
            modules[i].casillas[j].inputs = modules[0].casillas[j].inputs;
            modules[i].casillas[j].outputs = modules[0].casillas[j].outputs;
            for (int k = 0; k < modules[i].casillas[j].outputs.size(); k++) {
                for (int l = 0; l < modules[i].casillas[j].outputs[k].outConnections.size(); l++) {
                    modules[i].casillas[j].outputs[k].outConnections[l].x = i;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::saveJSON(){
    
    mJSON.clear();

    Json::Value points(Json::arrayValue);
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < modules[i].casillas.size(); j++){
            Json::Value pos(Json::objectValue);
            pos["id"] = modules[i].casillas[j].id;
            pos["sound"] = modules[i].casillas[j].soundBank;
            pos["mod"] = i;
            pos["x"] = modules[i].casillas[j].x;
            pos["y"] = modules[i].casillas[j].y;
            int out = modules[i].casillas[j].outputs.size();
            int in = modules[i].casillas[j].inputs.size();
            pos["out"] = out;
            pos["in"] = in;
            points.append(pos);
        }
    }
    mJSON["casillas"]  = points;
        
    Json::Value connections(Json::arrayValue);
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < modules[i].casillas.size(); j++){
            for (int k = 0; k < modules[i].casillas[j].inputs.size(); k++) {
                for (int l = 0; l < modules[i].casillas[j].inputs[k].inConnections.size(); l++) {
                    Json::Value conn(Json::objectValue);
                    conn["mod"] = i;
                    conn["cas"] = j;
                    conn["input"] = k;
                    conn["target"] = modules[i].casillas[j].inputs[k].inConnections[l];
                    connections.append(conn);
                }
            }
        }
    }
    mJSON["Connections"]  = connections;
        
    Json::Value outConnections(Json::arrayValue);
    for (int i = 0; i < modules.size(); i++) {
        for (int j = 0; j < modules[i].casillas.size(); j++){
            for (int k = 0; k < modules[i].casillas[j].outputs.size(); k++) {
                for (int l = 0; l < modules[i].casillas[j].outputs[k].outConnections.size(); l++) {
                    Json::Value oconn(Json::objectValue);
                    oconn["origenMod"] = i;
                    oconn["origenCas"] = j;
                    oconn["origenOut"] = k;
                    oconn["destinoMod"] = modules[i].casillas[j].outputs[k].outConnections[l].x;
                    oconn["destinoCas"] = modules[i].casillas[j].outputs[k].outConnections[l].y;
                    oconn["destinoIn"] = modules[i].casillas[j].outputs[k].outConnections[l].z;
                    outConnections.append(oconn);
                }
            }
        }
    }
    
        mJSON["outConnections"]  = outConnections;
    
    
    string newstr = "casillas.json";
    if (!mJSON.save(newstr, true)){
        ofLogNotice("ofApp::setup") << "casillas.json written unsuccessfully.";
    }
    else{
        ofLogNotice("ofApp::setup") << "casillas.json written successfully.";
    }
    
}

//--------------------------------------------------------------
void ofApp::loadJSON(){
    std::string file;

        file = "casillas.json";

    bool parsingSuccessful = mJSON.open(file);
    if(parsingSuccessful){
        ofLogNotice("ofApp::setup") << "successfully read: "<<file<<std::endl;
    }else{
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << std::endl;
    }

    for (const Json::Value & points : mJSON["casillas"])  // iterate over "points"
    {
        int   id = points["id"].asInt();
        int sound = points["sound"].asInt();
        int  mod = points["mod"].asInt();
        float px   = points["x"].asFloat();
        float py   = points["y"].asFloat();
        int out  = points["out"].asInt();
        int in  = points["in"].asInt();

        
        Casilla casilla(px, py, id, false);
        modules[mod].casillas.push_back(casilla);
        modules[mod].casillas.back().setup(sound);
        Nodo nodo(nodoCount, true, true, false);
        nodoCount++;
        modules[mod].casillas.back().inputs.push_back(nodo);
        for (int i = 0; i < out; i++) {
            Nodo nodo(nodoCount, false, false, false);
            modules[mod].casillas.back().outputs.push_back(nodo);
            nodoCount ++;
        }
        for (int i = 0; i < in -1; i++) {
            Nodo nodo(nodoCount, false, true, false);
            modules[mod].casillas.back().inputs.push_back(nodo);
            nodoCount++;
        }
        
    }
    
    for (const Json::Value & points : mJSON["Connections"]){
        int   mod = points["mod"].asInt();
        int   cas = points["cas"].asInt();
        int input = points["input"].asInt();
        int target = points["target"].asInt();

        modules[mod].casillas[cas].inputs[input].inConnections.push_back(target);

        
    }
    
    for (const Json::Value & points : mJSON["outConnections"]){
        int origenMod = points["origenMod"].asInt();
        int origenCas = points["origenCas"].asInt();
        int origenOut = points["origenOut"].asInt();
        int destinoMod = points["destinoMod"].asInt();
        int destinoCas = points["destinoCas"].asInt();
        int destinoIn = points["destinoIn"].asInt();
        ofVec3f temp;
        temp.set(destinoMod, destinoCas, destinoIn);

        modules[origenMod].casillas[origenCas].outputs[origenOut].outConnections.push_back(temp);
        
    }
}
