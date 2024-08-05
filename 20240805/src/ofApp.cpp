#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Load the image
    image.load("dahlia.jpg");
    
    // Allocate the mesh
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    // Get the image dimensions
    int width = image.getWidth();
    int height = image.getHeight();
    
    // Loop through each pixel in the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get the pixel color
            ofColor color = image.getColor(x, y);
            
            // Calculate brightness as a height value
            float brightness = color.getBrightness();
            float z = ofMap(brightness, 0, 255, -100, 100); // Map brightness to a height
            
            // Create a vertex with the x, y, z positions
            glm::vec3 position(x, y, z);
            mesh.addVertex(position);
            
            // Set the vertex color based on the image color
            mesh.addColor(color);
        }
    }
    
    // Enable depth testing for 3D effect
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update(){
    // Get the current time
    float time = ofGetElapsedTimef();
    
    // Update each vertex color to create a color animation
    for (int i = 0; i < mesh.getNumVertices(); ++i) {
        // Get current vertex position and color
        glm::vec3 vertex = mesh.getVertex(i);
        ofColor color = mesh.getColor(i);
        
        // Calculate distance from the center
        float distance = glm::length(glm::vec2(vertex.x - image.getWidth() / 2, vertex.y - image.getHeight() / 2));
        
        // Apply a sine wave function for color shift based on distance from the center
        float hueShift = sin(time + distance * 0.01) * 30; // Slight hue shift
        
        // Update the color hue with a smooth transition
        color.setHue(fmod(color.getHue() + hueShift, 255)); // Keep hue in the range [0, 255]
        mesh.setColor(i, color);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Set background color
    ofBackground(0);
    
    // Begin camera
    cam.begin();
    
    // Translate the mesh to center
    ofPushMatrix();
    // Translate mesh to be centered in the view
    ofTranslate(-image.getWidth() / 2, -image.getHeight() / 2, 0);
    
    // Draw the mesh
    glPointSize(3.0); // Set point size for the vertices
    mesh.draw();
    
    ofPopMatrix();
    
    // End camera
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
