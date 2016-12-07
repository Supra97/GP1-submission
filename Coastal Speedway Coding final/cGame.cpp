/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	score = 0;

	// Store the textures
	textureName = { "Hatchback", "Hatchback2", "Hatchback3", "Hatchback4","theNissan","theBackground"," menuBkgd", "endBkgdd"};
	texturesToUse = { "Images/Hatchback.png", "Images/Hatchback2.png", "Images/Hatchback3.png", "Images/Hatchback4.png", "Images/Nissan.png", "Images/MainLevel.png", "Images/CoastalSpeedwayMenu.png","Images/GameOver.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "Score "};
	
	//theTextureMgr->addTexture("ScoreTxt", theFontMgr->getFont(fontList[1])->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 255, 0 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme", "car", "explosion" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/223475__zagi2_jolly-game-groove.wav", "Audio/34176__charliefarley_engine1.wav", "Audio/Explosion+1.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theGameState = MENU;
	// initialises the menu background
	menuBkgd.setSpritePos({ 0, 0 });
	menuBkgd.setTexture(theTextureMgr->getTexture(textureName[6]));
	menuBkgd.setSpriteDimensions(theTextureMgr->getTexture(textureName[6])->getTWidth(), theTextureMgr->getTexture(textureName[6])->getTHeight());
	// initialises level background
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture(textureName[5]));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture(textureName[5])->getTWidth(), theTextureMgr->getTexture(textureName[5])->getTHeight());
	//intialises the end background
	endBkgd.setSpritePos({ 0, 0 });
	endBkgd.setTexture(theTextureMgr->getTexture(textureName[7]));
	endBkgd.setSpriteDimensions(theTextureMgr->getTexture(textureName[7])->getTWidth(), theTextureMgr->getTexture(textureName[7])->getTHeight()); // initialises end background 
	 																																		 
	// initialises nissan sprite and positon and chnages it inot a vector so that it can be collide with the hatchs
	playerNissan.push_back(new cNissan);
	playerNissan[0]->setSpritePos({ 512 ,512 });
	playerNissan[0]->setTexture(theTextureMgr->getTexture(textureName[4]));
	playerNissan[0]->setSpriteDimensions(theTextureMgr->getTexture(textureName[4])->getTWidth(), theTextureMgr->getTexture(textureName[4])->getTHeight());
	playerNissan[0]->setNissanVelocity({ 0, 0 });

	// Create vector array of hatchs to use
	


	for (int hatch = 0; hatch < 4; hatch++)
	{
		theHatchs.push_back(new cHatch);
		int xPostion = 100 * (rand() % 5 + 1) +500;
		int yPostion = 100 * (rand() % 5 + 1) ;
		if (yPostion = 0) { xPostion = 700; }
		theHatchs[hatch]->setSpritePos({ 100 * (rand() % 5 + 1), 50 * (rand() % 5 + 1) });
		theHatchs[hatch]->setSpriteTranslation({ 0, (rand() % 8 + 1) });
		int randHatch = rand() % 4;
		theHatchs[hatch]->setTexture(theTextureMgr->getTexture(textureName[randHatch]));
		theHatchs[hatch]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randHatch])->getTWidth(), theTextureMgr->getTexture(textureName[randHatch])->getTHeight());
		theHatchs[hatch]->sethatchVelocity({ 0.0f, 3.0f });
		theHatchs[hatch]->setActive(true);
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderPresent(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		//SDL_RenderPresent(theRenderer);
		//renders the menu
		menuBkgd.render(theRenderer, NULL, NULL, menuBkgd.getSpriteScale());
	}
	break;
	case PLAYING:

	{
		update(theGameState = PLAYING);
		//SDL_RenderPresent(theRenderer);
		//renders the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// redners the nissan
		
		playerNissan[0]->render(theRenderer, &playerNissan[0]->getSpriteDimensions(), &playerNissan[0]->getSpritePos(), playerNissan[0]->getSpriteRotAngle(), &playerNissan[0]->getSpriteCentre(), playerNissan[0]->getSpriteScale());
		// Render each hatchbacks in the vector array
		
		for (int draw = 0; draw < theHatchs.size(); draw++)
		{
			theHatchs[draw]->render(theRenderer, &theHatchs[draw]->getSpriteDimensions(), &theHatchs[draw]->getSpritePos(), theHatchs[draw]->getSpriteRotAngle(), &theHatchs[draw]->getSpriteCentre(), theHatchs[draw]->getSpriteScale());
		}
		//renders the position and fotn for the score
		theTextureMgr->addTexture("ScoreTxt", theFontMgr->getFont(fontList[1])->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 255, 0 }, { 0, 0, 0, 0 }));
		cTexture* tempTextTexture = theTextureMgr->getTexture("ScoreTxt");
		SDL_Rect pos = {10 , 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		
	}
	break;
	case END:
	{
        // renders background
		endBkgd.render(theRenderer, NULL, NULL, endBkgd.getSpriteScale());
		//renders socre in new position
		cTexture* tempTextTexture = theTextureMgr->getTexture("ScoreTxt");
		SDL_Rect pos = { 630,334, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	break;
	//SDL_RenderPresent(theRenderer);
	}
	SDL_RenderPresent(theRenderer);

		}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{//switches the game state when called on
	switch (theGameState)
	{
	case MENU:
	{
		theSoundMgr->getSnd(soundList[0])->play(-1);//plays the theme song.
	}
	break;
	if (theGameState == PLAYING)
	{
		score = 0;
		
		//playerNissan[0]->setSpritePos({ 512, 512 });
		//playerNissan[0]->setNissanVelocity({ 0, 0 });

		theHatchs[4]->setSpritePos({ 700, 0 });
		theHatchs[4]->sethatchVelocity({ 0, 0 });
	}
	case PLAYING://game switches playstate

	{
		// Update the visibility and position of each hatch
		vector<cHatch*>::iterator hatchIterator = theHatchs.begin();
		while (hatchIterator != theHatchs.end())
		{
			if ((*hatchIterator)->isActive() == false)
			{
				hatchIterator = theHatchs.erase(hatchIterator);
			}
			else
			{
				(*hatchIterator)->update(deltaTime);
				if ((*hatchIterator)->getSpritePos().y > 768) // this returns the hatchs back to the top of the screen once they have gone past the point 768
				{
					(*hatchIterator)->setSpritePos({ (*hatchIterator)->getSpritePos().x, 0 });
				}
				++hatchIterator;
			}
			theSoundMgr->getSnd(soundList[0])->play(1);//plays theme music

			theSoundMgr->getSnd(soundList[1])->play(1);// plays car sfx
			
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cHatch*>::iterator hatchIterartor = theHatchs.begin(); hatchIterartor != theHatchs.end(); ++hatchIterartor)
		{
			if (playerNissan[0]->collidedWith(&playerNissan[0]->getBoundingRect(), &(*hatchIterartor)->getBoundingRect())) 
			{
				// if a collision set the hatch and  to false
				(*hatchIterartor)->setActive(false);
				playerNissan[0]->setActive(false);
				theSoundMgr->getSnd(soundList[0])->play(1);
				
				
				theGameState = END;
			}

			// Update the Nissan position
		}
			playerNissan[0]->update(deltaTime);

	}
	break;
	case END://gamestate switches end screen
	{
		theSoundMgr->getSnd(soundList[0])->play(1);
	}
	}

}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				
				theLoop = false;
				break;
			}
			case SDLK_RETURN:
			{
				
				switch(theGameState = PLAYING);
				score = 0;// switches game sate from menu and end screens to playing screen
				StrScore = "Score " + to_string(score);
				gameTextList[0] = StrScore.c_str();
				if (theTextureMgr->getTexture("ScoreTxt") != NULL)
				{
					theTextureMgr->deleteTexture("ScoreTxt");
				}
				

					
			}
			break;
			case SDLK_DOWN: // moves nissan down
			{
				{
					playerNissan[0]->setSpriteTranslation({ 0, 5 });
				}
			}
			break;

			case SDLK_UP: //moves nissan up
			{
				{
					playerNissan[0]->setSpriteTranslation({ 0, -5 });
				}
			}
			break;
			case SDLK_RIGHT: //moves nissan right
			{
				{
					playerNissan[0]->setSpriteTranslation({ 5, 0 });
				}
			}
			break;

			case SDLK_LEFT: //moves nissan left
			{
				{
					playerNissan[0]->setSpriteTranslation({ -5, 0 });
				}
			}
			break;
			case SDLK_SPACE://gives nissan spirte a speed boost up and increases speed to gain points.
			{
				if (playerNissan[0]->getSpritePos().y < (renderWidth - playerNissan[0]->getSpritePos().w))
				{
					playerNissan[0]->setSpriteTranslation({ 0, -15 });  
					score += 100;
					StrScore = "Score " + to_string(score);
					gameTextList[0] = StrScore.c_str();
					theTextureMgr->deleteTexture("ScoreTxt");
				}

			}
			break;
			default:
				break;
				}

			}
			}
			return theLoop;
		}
	
			
			

			



double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

