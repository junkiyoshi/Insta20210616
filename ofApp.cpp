#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(255);
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	this->image.loadImage("images/lenna.png");
	this->image.resize(720, 720);
	this->fbo.allocate(720, 720);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	ofTranslate(this->fbo.getWidth() * 0.5, this->fbo.getHeight() * 0.5);

	for (int k = 0; k < 3; k++) {

		switch (k % 3) {

		case 0:
			ofSetColor(255, 255, 0);
			break;
		case 1:
			ofSetColor(0, 255, 255);
			break;
		case 2:
			ofSetColor(255, 0, 255);
			break;
		}
		
		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

		ofMesh mesh;
		vector<glm::vec3> right, left;

		glm::vec3 last_location;
		float last_theta;
		int len = 100;
		int head_size = 45;

		for (int i = 0; i < len; i++) {

			auto location = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.008), 0, 1, -280, 280), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.008), 0, 1, -280, 280), 0);
			auto next = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i + 1) * 0.008), 0, 1, -280, 280), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i + 1) * 0.008), 0, 1, -280, 280), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		auto tmp_head_size = ofMap(len - 2, 0, len, 0, head_size);
		mesh.addVertex(last_location);

		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		mesh.draw();
	}

	ofDisableBlendMode();
	this->fbo.end();

	this->image.getTextureReference().setAlphaMask(this->fbo.getTexture());
	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	this->image.draw(ofGetWindowSize() * 0.5);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}