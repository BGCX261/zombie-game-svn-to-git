/*
Zombie Project.  A game.  With Zombies.
Copyright (C) 2007  Matthew Chudleigh

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <string>
#include <ogre.h>
#include <OgreLogManager.h>
#include <cegui/CEGUIExceptions.h>
#include <NxOgre.h>

#include "AM_GUI.hpp"
#include "AM_Random.hpp"
#include "AM_RayCast.hpp"
#include "AM_Steering.hpp"
#include "AM_Quadtree.hpp"
#include "AM_Simulation.hpp"
#include "AM_InputHandler.hpp"

#include "AI/AM_AI_Test.hpp"
#include "City/AM_City.hpp"
#include "City/AM_Building.hpp"
#include "Gameplay/AM_Human.hpp"
#include "Gameplay/AM_Camera.hpp"

#include "ComponentObject/AM_Object.hpp"
#include "Serialisation/AM_Serialisation.hpp"

namespace AutoMatic
{
	class SimulationKeyCallback : public InputCallback
	{
	public:
		SimulationKeyCallback(Simulation * simulation_, Ogre::Camera * camera_, NxOgre::Scene * scene_
			, Object * human_, unsigned int priority_)
			: InputCallback(priority_), _simulation(simulation_), _camera(camera_), _scene(scene_)
			, _mouseX(0.0f), _mouseY(0.0f), _human(human_), mouseSelection(scene_)
		{
			mouseSelection.add(_human);
		}

		virtual bool keyReleased(OIS::KeyEvent const & keyboardEvent_)
		{
			if (keyboardEvent_.key == OIS::KC_ESCAPE)
			{
				_simulation->requestStateChange(Simulation::Shutdown);
				return true;
			}
			
			return false;
		}

		virtual bool mouseReleased(const OIS::MouseEvent & mouseEvent_, OIS::MouseButtonID mouseButton_)
		{
			mouseSelection.select(*_camera, _mouseX, _mouseY, 1000.0f);

			return false;
		}

		virtual bool mouseMoved(const OIS::MouseEvent & mouseEvent_)
		{
			_mouseX = mouseEvent_.state.X.abs / (float)mouseEvent_.state.width;
			_mouseY = mouseEvent_.state.Y.abs / (float)mouseEvent_.state.height;

			return false;
		}

	private:
		Simulation * _simulation;
		Ogre::Camera * _camera;
		NxOgre::Scene * _scene;
		float _mouseX, _mouseY;
		Object * _human;

		MouseSelection mouseSelection;
	};

	void SetupResources(Ogre::String const & resourcesConfigFilename_)
	{
		// Load resource paths from config file
		Ogre::ConfigFile configFile;
		configFile.load(resourcesConfigFilename_);

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator sectionIter = configFile.getSectionIterator();

		Ogre::String sectionName, typeName, archiveName;
		while (sectionIter.hasMoreElements())
		{
			sectionName = sectionIter.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator settingsIter;
			for (settingsIter = settings->begin(); settingsIter != settings->end(); ++settingsIter)
			{
				typeName = settingsIter->first;
				archiveName = settingsIter->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archiveName, typeName, sectionName);
			}
		}
	}

	void MakeManualObject(Ogre::SceneManager * sceneMgr_)
	{
		Building building;

		building.build(1);

		Ogre::ManualObject* manual = sceneMgr_->createManualObject("manual");
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

		float currentFloorY = 0.0f, floorHeight = 1.0f;
		int indexCount = 0, indexStart = 0;

		for (std::vector<Polygon>::reverse_iterator i = building.blueprints().rbegin(); 
			i != building.blueprints().rend(); ++i)
		{
			indexStart = indexCount;
			Vertices::VertexList::const_iterator j = (*i).getList().begin();
			while(j != (*i).getList().end())
			{
				manual->position((*j)._x, currentFloorY + floorHeight, (*j)._y); manual->index(indexCount++);
				manual->position((*j)._x, currentFloorY, (*j)._y); manual->index(indexCount++);
				++j;
			}

			manual->index(indexStart);	manual->index(indexStart + 1);
		}

		manual->end();
		sceneMgr_->getRootSceneNode()->createChildSceneNode()->attachObject(manual);
	}
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	AutoMatic::RunAITest();
	AutoMatic::RunSerialisationTest();
	AutoMatic::RunHumanSaveLoadTest();
	AutoMatic::RunInventorySaveLoadTest();
	AutoMatic::RunRandomSeedAndGenerationTest();
	AutoMatic::RunQuadtreeCreationAndNodeSelectionTest();
return EXIT_SUCCESS;
	Ogre::Root * root = new Ogre::Root("./plugins.cfg", "../configuration/ogre.cfg", "./logs/ogre.log");
	assert(Ogre::Root::getSingletonPtr() == root);
	if(!root->restoreConfig() && !root->showConfigDialog()){ return EXIT_FAILURE; }

	AutoMatic::SetupResources("../media/resources.cfg");

	Ogre::RenderWindow * window = root->initialise(true, "OgreProject");

	Ogre::SceneManager * sceneMgr = root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

		// Resetting the window requires destroying the root...
	//window->setVisible(false);
	//window->destroy(); // Destroying the window, then recreating it, fails when rendering.
	//window->create("OgreProject", 1280, 1024, false, 0);
	//window->destroy();																										
	//window = root->createRenderWindow("OgreProject", 640, 480, false, 0); // "Window with that name already exists"

	unsigned long hWnd;
	window->getCustomAttribute("WINDOW", &hWnd);
	window->swapBuffers();

	AutoMatic::InputHandler inputHandler(hWnd);
	inputHandler.setWindowExtents(window->getWidth(), window->getHeight());
	// Create the camera
	
	AutoMatic::Camera camera("MainCamera", sceneMgr, window);
	camera.position(0,10,15);
	camera.target(0,0,0);

	AutoMatic::SteeringManager steeringTest;
	AutoMatic::Simulation * simulation = new AutoMatic::Simulation(&steeringTest);
	NxOgre::Scene* scene = simulation->world().world()->createScene("Main", sceneMgr, "gravity: yes, floor: yes");
	NxOgre::Body* body = scene->createBody("cube.1m.mesh", new NxOgre::CubeShape(1,1,1), Ogre::Vector3(0,10.25f,0), "Mass: 100");
	
		// Character stuff is still under construction in NxOgre 0.9
	//NxOgre::Pose pose;
	//NxOgre::CharacterParams characterParams; characterParams.setToDefault();
	//pose.v = NxVec3(0,5,0);
	//NxOgre::Character * character = scene->createCharacter("MyCharacter", pose, characterParams);
	//character->attachMesh("ninja.mesh");
	//character->getNode()->scale(0.1,0.1,0.1);

	AutoMatic::HumanFactory humanFactory;
	AutoMatic::Object * human = humanFactory.create(scene, body);
	human->disable<AutoMatic::Physics>();
	AutoMatic::SimulationKeyCallback simkeyCB(simulation, camera.get(), scene, human, 0);

	simulation->world().add(human);
	steeringTest.add(human);

	bool addedOk = inputHandler.addInputCallBack(&simkeyCB);
	assert(addedOk);

	AutoMatic::MakeManualObject(sceneMgr);

	AutoMatic::GUI * gui = new AutoMatic::GUI(window, sceneMgr, 1);
	try
	{
		gui->load("Zombie.scheme", "Zombie", "Trashed-12");
		addedOk = inputHandler.addInputCallBack(gui);
		assert(addedOk);
	}
	catch (Ogre::Exception & ogre_e)
	{
		return EXIT_FAILURE;
	}
	catch (CEGUI::GenericException & cegui_e)
	{
		return EXIT_FAILURE;
	}

	Ogre::Vector3 pos;

	unsigned long timestep = 0;
	float fTimestep = 0.0f, minIdle = 0.03f, minRender = 0.015f, idleAccum = 0.0f, 
		renderAccum = 0.0f, currDt = 0.0f;
	
	root->getTimer()->reset();


	while(simulation->getCurrentState() != AutoMatic::Simulation::Shutdown)
	{
		timestep = root->getTimer()->getMilliseconds(); // timestep does not include time to calculate simulation.
		fTimestep = (timestep / 1000.0f);
		currDt = fTimestep;
		renderAccum += fTimestep;

		simulation->requestStateChange(AutoMatic::Simulation::Update_World);
		simulation->update(fTimestep);

		simulation->requestStateChange(AutoMatic::Simulation::User_Input);
		Ogre::WindowEventUtilities::messagePump();
		inputHandler.update(currDt);
		
		simulation->requestStateChange(AutoMatic::Simulation::Render_World);
		root->getTimer()->reset();
		//if(renderAccum >= minRender)
		{
			root->renderOneFrame();
			//renderAccum -= minRender; // TODO: maxRender?
		}
	}

	//simulation->world().world()->destroyScene(scene->getName()); // Causes crash, started after Character code was added.
	delete simulation;
	delete gui;

	// window->destroy(); // This works, but is unnecessary.
	//window->removeViewport(viewPort->getZOrder());
	//window->destroy();

	// viewPort->setCamera(0); // This line causes it to crash at sceneMgr->destroyCamera
	// viewPort->setCamera(camera2); // This line causes it to crash at root->destroySceneManager

	//sceneMgr->destroyCamera(camera);

	//root->destroySceneManager(sceneMgr);

	delete root;

	return EXIT_SUCCESS;
}

