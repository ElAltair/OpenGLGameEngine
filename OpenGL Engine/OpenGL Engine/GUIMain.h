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
	virtual ~Event( ) { };
};


class MouseEvent : public Event
{
public:
	MouseEvent(float _xpos, float _ypos, int button) :xpos(_xpos),ypos(_ypos),MouseButton(button) { 
	//	cout << "Xpos = "<< xpos << " Ypos = "<<ypos<< " Button = "  << MouseButton << endl; 
	};
	MouseEvent( float _xpos, float _ypos ) : xpos( _xpos ), ypos( _ypos ), MouseButton( INT_MAX )
	{
	//	cout << "Xpos = "<< xpos << " Ypos = "<<ypos<< endl; 
	}
	inline float getXpos( void ) { return xpos; };
	inline float getYpos( void ) { return ypos; };
	~MouseEvent( ) { };


private:
	float xpos;
	float ypos;
	int MouseButton;
};


class KeyboardEvent : public Event
{
public:
	KeyboardEvent( ) { keyboardButton = 0; };

private: 
	float keyboardButton;

};

class EventQueue
{
public:
	EventQueue( ) { };
	void addEvent(Event* ev) { eventQueue.push(ev); };
	Event* getNextEvent( ) { Event* tmp = eventQueue.front( ); eventQueue.pop( ); return tmp; };
	void printEventQueue( void );

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

class GUIWindow;
class GUIManager;

class GUIBase
{
public:
	virtual ~GUIBase( );
	virtual void Draw( ) = 0;
	virtual bool colladeWithMouse(double MouseXpos,double MouseYpos) = 0;
//	virtual void onMouseMove( double MouseXpos,double MouseYpos) = 0;
	virtual GUIBase* checkEvent( Event* ) = 0;
	virtual void makeEvent(GUIWindow*,GUIWindow*,GUIManager* ) = 0;
	virtual string ReturnGUIName( void ) = 0;

protected:
	GUIBase( );
	void setVisible( bool flag );
	bool isVisible( void );

private:

	bool VisibleState;

};

typedef void (*WindowClickEvent)(GUIWindow *window);
typedef void (*WindowAllClickEvent)(GUIManager* mg);
typedef void (*WindowHideEvent)(GUIWindow* mg);
typedef void (*WindowUnHideEvent)(GUIManager* mg);

class GUIWindow :public GUIBase
{
public:

	GUIWindow(float posx, float posy, float _width, float _height, const char*_Name);

	void setBaseContent(GUIBaseContent*);
	inline GUIBaseContent* getBaseContent(void) { return BaseContent; };
	inline void addChild(GUIBase* child) { inheriterList.push_back(child); };

	inline void setBackGroundColor( glm::vec3 color ) { backgroundColor = color; };
	inline void setBackGroundColor( float c1, float c2, float c3 ) { backgroundColor.x = c1; backgroundColor.y = c2, backgroundColor.z = c3; };
	inline void setStandartBackGroundColor( glm::vec3 color ) { standardBackgoundColor = color; backgroundColor = color; };
	inline void setStandartBackGroundColor( float c1, float c2, float c3 ) { backgroundColor.x = c1; backgroundColor.y = c2, backgroundColor.z = c3; standardBackgoundColor.x = c1; standardBackgoundColor.y = c2; standardBackgoundColor.z = c3; };
	inline void getStandardBackGroundColor( void ) { backgroundColor = standardBackgoundColor; };
	inline void hide( ) { setVisible(false); };
	inline void unHide( ) { setVisible( true ); };

	void Draw( );
	bool colladeWithMouse(double MouseXPos, double MouseYpos);
//	void onMouseMove(double MouseXpos, double MouseYpos);
	GUIBase* checkEvent( Event* );
	void makeEvent(GUIWindow*,GUIWindow*,GUIManager*);
	string ReturnGUIName( void ) { return Name; };

	~GUIWindow( );


	glm::mat4 model;

	//Child functions
	glm::mat4 getModel(void) { return model; }
	glm::vec3 getBackgroundColor(void) { return backgroundColor; };
	GLuint getWindowVao(void) { return WindowVao; };

	
	WindowClickEvent onClickEvent;
	WindowAllClickEvent onClickAllEvent;
	WindowHideEvent onClickHideEvent;
	WindowUnHideEvent onClickUnhideAll;
private:


	GUIBaseContent* BaseContent;

	float Width;
	float Height;
	float PosX;
	float PosY;
	string Name;

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

	glm::vec3 standardBackgoundColor;
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
	void performEvent();

	inline void addLinkToEventQueue( EventQueue* qu ) { eventQueue = qu; };
private:
	list<GUIBase*>::const_iterator listIterator;
	list<GUIBase*> listGuiElements;

	EventQueue* eventQueue;

};
#endif