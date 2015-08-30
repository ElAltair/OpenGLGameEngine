#ifndef MAIN_GUI_H
#define MAIN_GUI_H

#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <glm.hpp>
#include <glew.h>

using std::string;
using std::cout;
using std::endl;
using std::list;
using std::queue;


class Program;


class Event
{
public:
	Event( ) { };
	~Event( ) { };
};


class MouseEvent : public Event
{
public:
	MouseEvent(float _xpos, float _ypos, int button) :xpos(_xpos),ypos(_ypos),MouseButton(button) { cout << "Xpos = "<< xpos << " Ypos = "<<ypos<< " Button = "  << MouseButton << endl; };

private:
	float xpos;
	float ypos;
	int MouseButton;
};


class KeyboardEvent : public Event
{



};

class EventQueue
{
public:
	EventQueue( ) { };
	void addEvent(Event* ev) { eventQueue.push(ev); };
	Event* getNextEvent( ) { return eventQueue.front( ); };

private:
	queue<Event*> eventQueue;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GUIBaseVertices
{
public:
	GUIBaseVertices( );
	GLuint getBaseVbo( void );
	GLfloat* getVertices( void );
	~GUIBaseVertices( );
private:
	GLuint BaseVertexVbo;
	GLfloat* vertices;
};


class GUIBaseContent
{

public:
	GUIBaseContent( float WindowWidth, float WindowHeight, Program* pr );
	void updateWindowSize(float _Width, float _Height);
	Program* getGuiProgram( void );
	glm::mat4 getProjectionMatrix( void );
	float getWindowSizeWidth( void );
	float getWindowSizeHeight( void );
	GUIBaseVertices* getGUIBaseVertices( void );
	~GUIBaseContent( );
private:

	GUIBaseVertices BaseVertices;

	Program* GuiProgram;
	glm::mat4 projectionMatrix;
	float WindowSizeWidth;
	float WindowSizeHeight;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class GUIBase
{
public:
	virtual ~GUIBase( );
	virtual void Draw( ) = 0;
	virtual bool colladeWithMouse(double MouseXpos,double MouseYpos) = 0;
	virtual void onMouseMove( double MouseXpos,double MouseYpos) = 0;

protected:
	GUIBase( );
	void setVisible( bool flag );
	bool isVisible( void );

private:

	bool VisibleState;

};


class GUIWindow :public GUIBase
{
public:

	GUIWindow(float posx, float posy, float _width, float _height);

	void setBaseContent(GUIBaseContent*);
	inline GUIBaseContent* getBaseContent(void) { return BaseContent; };
	inline void addChild(GUIBase* child) { inheriterList.push_back(child); };

	inline void setBackGroundColor(glm::vec3 color) { backgroundColor = color; };
	inline void setBackGroundColor(float c1, float c2, float c3) { backgroundColor.x = c1; backgroundColor.y = c2, backgroundColor.z = c3; };
	inline void hide( ) { setVisible(false); };

	void Draw( );
	bool colladeWithMouse(double MouseXPos, double MouseYpos);
	void onMouseMove(double MouseXpos, double MouseYpos);

	~GUIWindow( );


	glm::mat4 model;

	//Child functions
	glm::mat4 getModel(void) { return model; }
	glm::vec3 getBackgroundColor(void) { return backgroundColor; };
	GLuint getWindowVao(void) { return WindowVao; };

private:

	GUIBaseContent* BaseContent;

	float Width;
	float Height;
	float PosX;
	float PosY;

	//GUI functions

	//Initialize openGL buffers
	void Initialize(void);
	void drawChilds(void );
	void drawParent(void);

	// GUI members
	bool isInitialized;

	list<GUIBase*>inheriterList;

	//Gl functions

	//Gl members
	GLuint WindowVao;
	GLuint WindowVbo;

	glm::vec3 backgroundColor;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GUIManager
{
public:
	GUIManager( ) {};
	void addElement( GUIBase* el );
	void makeFirst(void);
	bool isEnd( void );
	void nextElement(void);
	GUIBase* getElement(void);
	void performEvent(void);
private:
	list<GUIBase*>::const_iterator listIterator;
	list<GUIBase*> listGuiElements;

};
#endif