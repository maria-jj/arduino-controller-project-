#include "ofApp.h"

struct Tree {
	ofImage image;
	ofVec2f pos;

}t1,t2,t3,t4,t5,t6,t7,t8,barrier,fall;


//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowShape(1280, 720);
	ofSetRectMode(OF_RECTMODE_CENTER);

	//ARDUINO SETUP
	m_arduino.connect(ARDUINO_DEVICE_NAME, 57600);

	//INIT vars
	m_analogX_inputVal = 0;
	m_analogY_inputVal = 0;
	m_isSetup   = false;

	//add event listener for when arduino is ready
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	//GAME SETUP
    m_playerLives = 3;
    endGame = false;
    winGame = false;

	//Player setup
	m_playerNeutImage.load("neutralSki.png");
	m_playerDuckImage.load("duckingSki.png");
	m_playerJumpImage.load("jumpingSki.png");
	m_playerPos = { 900.0f, 1000.0f };
	m_playerImage = m_playerNeutImage;

	//Tree/obstacle setup
	t1.image.load("tree1.png");
	t2.image.load("tree2.png");
	t3.image.load("tree3.png");
	t4.image.load("tree3.png");
	t5.image.load("tree1.png");
	t6.image.load("tree1.png");
	t7.image.load("tree2.png");
	t8.image.load("tree3.png");
	barrier.image.load("barrier.png");
	fall.image.load("fallenTree.png");


    //Debug
    t1.pos.x = { -400.0f };
    t2.pos.x = { -400.0f };
    t3.pos.x = { -400.0f };
    t4.pos.x = { -400.0f };
    t5.pos.x = { -400.0f };
    t6.pos.x = { -400.0f };
    t7.pos.x = { -400.0f };
    t8.pos.x = { -400.0f };
    barrier.pos.x = { -400.0f };
    fall.pos.x = { -400.0f };

    //Level timer setup
	m_levelStartTime = ofGetElapsedTimeMillis();

    //Hit timer setup
    m_hitStartTime = ofGetElapsedTimeMillis();

	//Stage count
	m_stageCount = 0;

}

//--------------------------------------------------------------
void ofApp::update()
{
	//Arduino
	updateArduino();

    //Player Lives check
    checkLives();

    if (endGame == false)
    {
        //Level
        
        m_levelTimer = ofGetElapsedTimeMillis() - m_levelStartTime;
        if (m_levelTimer >= 5000)
        {
            int x = rand() % 4 + 1;
            levelGenerator(x);
            m_levelStartTime = ofGetElapsedTimeMillis();
            m_stageCount++;
        }

        //Player
        if ((isDucking == true) || (isJumping == true))
        {
            m_timer = ofGetElapsedTimeMillis() - m_startTime;

            if (m_timer >= 700)
            {
                isDucking = false;
                isJumping = false;
                m_playerImage = m_playerNeutImage;
            }
        }
        else
        {
            playerMovement();
        }

        //Collision / Level movement
        collisionCheck();
        scrollObstacles();

        cout << "Player x: " << m_playerPos.x << endl;
        cout << "Player y: " << m_playerPos.y << endl;
        cout << endl;
    }
    else 
    {
        //endgame
    }
	
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (endGame == false)
    {
        //Draw Background
        ofPushMatrix();
            ofBackground(255, 250, 250);
        ofPopMatrix();

        //Draw tree
        ofPushMatrix();
            ofScale(0.5, 0.5);
            t1.image.draw(t1.pos.x, t1.pos.y);
            t2.image.draw(t2.pos.x, t2.pos.y);
            t3.image.draw(t3.pos.x, t3.pos.y);
            t4.image.draw(t4.pos.x, t4.pos.y);
            t5.image.draw(t5.pos.x, t5.pos.y);
            t6.image.draw(t6.pos.x, t6.pos.y);
            t7.image.draw(t7.pos.x, t7.pos.y);
            t8.image.draw(t8.pos.x, t8.pos.y);
            barrier.image.draw(barrier.pos.x, barrier.pos.y);
            fall.image.draw(fall.pos.x, fall.pos.y);

        ofPopMatrix();

        //Draw Player
        ofPushMatrix();
        ofScale(0.5, 0.5);
        if (isHit == true)
        {
            ofSetColor(255, 0, 0, 140);
        }
        m_playerImage.draw(m_playerPos.x, m_playerPos.y);
        ofSetColor(255, 255, 255, 255);
        ofPopMatrix();


        //Draw Debug/UI
        ofPushMatrix();
            ofSetColor(0, 0, 0, 127);
            ofDrawRectangle(510, 15, 300, 220);
            ofSetColor(255, 255, 255, 255);
            ofDrawBitmapString("Joystick X: " + ofToString(m_analogX_inputVal), 450, 20);
            ofDrawBitmapString("Joystick Y: " + ofToString(m_analogY_inputVal), 450, 50);
            ofDrawBitmapString("Jumping: " + ofToString(isJumping), 400, 80);
            ofDrawBitmapString("Ducking: " + ofToString(isDucking), 525, 80);
            ofDrawBitmapString("Lives: " + ofToString(m_playerLives), 462, 110);
            ofSetColor(0, 0, 0, 127);
            ofDrawRectangle(1150, 20, 300, 100);
            ofSetColor(255, 255, 255, 255);
            ofDrawBitmapString("Stage: " + ofToString(m_stageCount) + "/15", 1150, 50);
        ofPopMatrix();
    }
    else
    {
        //Draw game over
        ofPushMatrix();
            ofSetColor(0, 0, 0, 255);
            ofDrawRectangle(550, 400, 1500, 1400);
            ofSetColor(255, 255, 255, 255);
            ofDrawBitmapString("Game Over", 580, 300);
            ofSetColor(255, 255, 255, 255);
        ofPopMatrix();
    }
	
	
}




/*-------------------------------------------------ARDUINO-FUNCTIONS---------------------------------------------------*/
void ofApp::setupArduino(const int & version)
{
    cout << "ARDUINO SETUP" << endl;

    m_isSetup = true;
    ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);   //good practise to remove listener

    //this pin is an analog input
    m_arduino.sendAnalogPinReporting(PIN_VRX_ANALOG, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(PIN_VRY_ANALOG, ARD_ANALOG);

	//this pin is a digital input
	m_arduino.sendDigitalPinMode(PIN_TILT_INPUT, ARD_INPUT);
	m_arduino.sendDigitalPinMode(PIN_TOUCH_INPUT, ARD_INPUT);


    //add event listeners for when pins change
    ofAddListener(m_arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(m_arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
	ofAddListener(m_arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);

}

//--------------------------------------------------------------
void ofApp::updateArduino()
{
    m_arduino.update();
}

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum)
{
	if (pinNum == PIN_TILT_INPUT) {
		cout << "DIGITAL INPUT: ";
		cout << "used tilt";
		cout << " " << endl;

		//Timer start
		m_startTime = ofGetElapsedTimeMillis();

		//Player state
		isDucking = true;
		m_playerImage = m_playerDuckImage;
	}

	if (pinNum == PIN_TOUCH_INPUT) {
		cout << "DIGITAL INPUT: ";
		cout << "used TOUCH";
		cout << " " << endl;

		//Timer start
		m_startTime = ofGetElapsedTimeMillis();

		//Player state
		isJumping = true;
		m_playerImage = m_playerJumpImage;

	}

}

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum)
{
    if (pinNum == PIN_VRX_ANALOG) {
        //get analog X value
		m_analogX_inputVal = m_arduino.getAnalog(pinNum);

        //remap the value
		m_analogX_inputVal = (int)ofMap(m_analogX_inputVal, 0, 1023, -50, 50);
    }

	if (pinNum == PIN_VRY_ANALOG){
		//get analog Y value
		m_analogY_inputVal = m_arduino.getAnalog(pinNum);

		//remap the value
		m_analogY_inputVal = (int)ofMap(m_analogY_inputVal, 0, 1023, -50, 50);
	}
}

/*---------------------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------GAME-FUNCTIONS----------------------------------------------------*/
void ofApp::playerMovement()
{
	if ((m_analogX_inputVal < -1) || (m_analogX_inputVal > 1))
	{
		m_playerPos.x += (m_analogX_inputVal * m_playerXSpeed);
	}

	if ((m_analogY_inputVal < -1) || (m_analogY_inputVal > 1))
	{
		m_playerPos.y += (m_analogY_inputVal * m_playerYSpeed);
	}

	//Clamp Movement
	m_playerPos.x = ofClamp(m_playerPos.x, 0.0f, 2500.0f);
	m_playerPos.y = ofClamp(m_playerPos.y, 900.0f, 1350.0f);

}

void ofApp::levelGenerator(int checkNum)
{
	switch (checkNum)
	{
	case 1:
		t1.pos = { 200.0f, 1000.0f - 1200.0f };
		t2.pos = { 500.0f, 900.0f - 1200.0f };
		t3.pos = { 1200.0f, 450.0f - 1200.0f };
		t4.pos = { 1600.0f, 1200.0f - 1200.0f };
		t5.pos = { 1750.0f, 450.0f - 1200.0f };
		t6.pos = { 120.0f, -100.0f - 1200.0f };
		t7.pos = { 1600.0f, 150.0f - 1200.0f };
		t8.pos = { 2050.0f, 0.0f - 1200.0f };
		barrier.pos = { 900.0f, 750.0f - 1200.0f };
		fall.pos = { 2150.0f, 500.0f - 1200.0f };
		break;
	case 2:
		t1.pos = { 200.0f, 1100.0f - 1200.0f };
		t2.pos = { 500.0f, 1100.0f - 1200.0f };
		t3.pos = { 1200.0f, 1100.0f - 1200.0f };
		t4.pos = { 1600.0f, 1100.0f - 1200.0f };
		t5.pos = { 850.0f, 1100.0f - 1200.0f };
		t6.pos = { 2300.0f, -100.0f - 1200.0f };
		t7.pos = { 1600.0f, -100.0f - 1200.0f };
		t8.pos = { 2050.0f, -100.0f - 1200.0f };
		fall.pos = { 2150.0f, 1200.0f - 1200.0f };
		barrier.pos = { 1150.0f, 200.0f - 1200.0f };
		break;
	case 3:
		t1.pos = { 200.0f, 1100.0f - 1200.0f };
		t2.pos = { 500.0f, 1100.0f - 1200.0f };
		t3.pos = { 2300.0f, 1100.0f - 1200.0f };
		t4.pos = { 1850.0f, 1100.0f - 1200.0f };
		t5.pos = { 850.0f, 1100.0f - 1200.0f };
		t6.pos = { 1850.0f, 350.0f - 1200.0f };
		t7.pos = { 1000.0f, 500.0f - 1200.0f };
		t8.pos = { 2000.0f, 0.0f - 1200.0f };
		fall.pos = { 2150.0f, 1200.0f - 1200.0f };
		barrier.pos = { 1400.0f, 300.0f - 1200.0f };
		break;
	case 4:
		t1.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t2.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t3.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t4.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t5.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t6.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t7.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		t8.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		fall.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		barrier.pos = { (float)(rand() % 2300 + 1), (float)(rand() % 1200) - 1200.0f };
		break;
	default: 

		break;
	}
}

void ofApp::collisionCheck()
{

    m_hitTimer = ofGetElapsedTimeMillis() - m_hitStartTime;
    if (m_hitTimer >= 1500)
    {
        //Reset bool
        isHit = false;

        //Fallen tree hitbox
        if (isJumping == true)
        {
            //forget about it
        }
        else
        {
            if ((m_playerPos.x < fall.pos.x + 100.0f) && (m_playerPos.x > fall.pos.x - 100.0f))
            {
                if ((m_playerPos.y < fall.pos.y + 150.0f) && (m_playerPos.y > fall.pos.y))
                {
                    m_playerLives--;
                    m_hitStartTime = ofGetElapsedTimeMillis();
                    isHit = true;
                }
            }
        }

        //Barrier hitbox
        if (isDucking == true)
        {
            //forget about it
        }
        else
        {
            if ((m_playerPos.x < barrier.pos.x + 100.0f) && (m_playerPos.x > barrier.pos.x - 100.0f))
            {
                if ((m_playerPos.y < barrier.pos.y + 150.0f) && (m_playerPos.y > barrier.pos.y))
                {
                    m_playerLives--;
                    m_hitStartTime = ofGetElapsedTimeMillis();
                    isHit = true;
                }
            }
        }


        //Tree hitboxes
        if ((m_playerPos.x < t1.pos.x + 70.0f) && (m_playerPos.x > t1.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t1.pos.y + 200.0f) && (m_playerPos.y > t1.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t2.pos.x + 70.0f) && (m_playerPos.x > t2.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t2.pos.y + 200.0f) && (m_playerPos.y > t2.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t3.pos.x + 70.0f) && (m_playerPos.x > t3.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t3.pos.y + 200.0f) && (m_playerPos.y > t3.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t4.pos.x + 70.0f) && (m_playerPos.x > t4.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t4.pos.y + 200.0f) && (m_playerPos.y > t4.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t5.pos.x + 70.0f) && (m_playerPos.x > t5.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t5.pos.y + 200.0f) && (m_playerPos.y > t5.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t6.pos.x + 70.0f) && (m_playerPos.x > t6.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t6.pos.y + 200.0f) && (m_playerPos.y > t6.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t7.pos.x + 70.0f) && (m_playerPos.x > t7.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t7.pos.y + 200.0f) && (m_playerPos.y > t7.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

        if ((m_playerPos.x < t8.pos.x + 70.0f) && (m_playerPos.x > t8.pos.x - 70.0f))
        {
            if ((m_playerPos.y < t8.pos.y + 200.0f) && (m_playerPos.y > t8.pos.y + 50.0f))
            {
                m_playerLives--;
                m_hitStartTime = ofGetElapsedTimeMillis();
                isHit = true;
            }
        }

    }
   

}

void ofApp::scrollObstacles()
{
	t1.pos.y += m_scrollSpeed;
	t2.pos.y += m_scrollSpeed;
	t3.pos.y += m_scrollSpeed;
	t4.pos.y += m_scrollSpeed;
	t5.pos.y += m_scrollSpeed;
	t6.pos.y += m_scrollSpeed;
	t7.pos.y += m_scrollSpeed;
	t8.pos.y += m_scrollSpeed;
	barrier.pos.y += m_scrollSpeed;
	fall.pos.y += m_scrollSpeed;

}

void ofApp::checkLives()
{
    if (m_playerLives <= 0)
    {
        endGame = true;
    }
}


/*---------------------------------------------------------------------------------------------------------------------*/
