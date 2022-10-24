#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		/*-------------------------------------------------ARDUINO-VARIABLES---------------------------------------------------*/

        //Tilt consts
		static const int PIN_TILT_INPUT	= 13;
		int tiltState					= 0;

		//Joystick consts
		static const int PIN_SW_INPUT	= 8;	//?
		static const int PIN_VRX_ANALOG = 0;
		static const int PIN_VRY_ANALOG = 1;
		float m_analogX_inputVal;					//out value x
		float m_analogY_inputVal;					//out value y

		//Touch consts
		static const int PIN_TOUCH_INPUT = 4;

		//Aruino vars
        const string ARDUINO_DEVICE_NAME    = "COM3";   //name of port arduino connected to 
        ofArduino   m_arduino;							//arduino communication object
        bool        m_isSetup;							//checking if arduino was setup

        //Arduino functions
        void setupArduino(const int & version);								//called when arduino is ready
        void updateArduino();												//called to update arduino
        void digitalPinChanged(const int & pinNum);							//called when a digital INPUT value changes on arduino
		void analogPinChanged(const int & pinNum);							//called when an analog OUTPUT value changes on arduino
		

		/*--------------------------------------------------------------------------------------------------------------------*/



		/*-------------------------------------------------GAME-VARIABLES--------------==-------------------------------------*/
		
		//Player variables
		ofImage		m_playerImage;
		ofImage		m_playerNeutImage;
		ofImage		m_playerDuckImage;
		ofImage		m_playerJumpImage;
		ofVec2f		m_playerPos;
		float	m_playerXSpeed = 0.4;
		float	m_playerYSpeed = 0.3;
		bool	isJumping;
		bool	isDucking;
        bool    isHit;

		//Timers
		float	m_startTime;
		float	m_timer;
		float	m_levelStartTime;
		float	m_levelTimer;
        float   m_hitStartTime;
        float   m_hitTimer;

		//Scroll
		float	m_scrollSpeed = 10.0f;

        //Game variables
        int     m_playerLives;
        int		m_stageCount;       //score
        bool    endGame;
        bool    winGame;

		//Game functions
		void playerMovement();
		void levelGenerator(int checkNum);
		void collisionCheck();
		void scrollObstacles();
        void checkLives();
        void checkpointGenerator();

		/*--------------------------------------------------------------------------------------------------------------------*/
};