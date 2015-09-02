#include "GUIMain.h"
#include "Program.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>



GUIBase::GUIBase( )
{
	VisibleState = true;
}
GUIBase::~GUIBase( )
{
	cout << "Delete Base" << endl;
}
void GUIBase::setVisible( bool flag )
{
	VisibleState = flag;
}

bool GUIBase::isVisible( void )
{
	return VisibleState;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


GUIWindow::GUIWindow( float posx, float posy, float _width, float _height, const char* _Name ) : PosX( posx ), PosY( posy ), Width( _width ), Height( _height ), GUIBase( )
{
	Name = _Name;

	isInitialized = false;
}

void GUIWindow::setBaseContent( GUIBaseContent* _content )
{
	BaseContent = _content;
}




void GUIWindow::Draw( )
{
	//cout << "Draw Window" << endl;
	if ( !isInitialized )
	{
		cout << "Start Initializing" << endl;
		Initialize( );

	}
	else
	{
		if ( isVisible( ) )
		{
			drawParent( );
			drawChilds( );

		}
		else
		{
		//	cout << "Invisible" << endl;
		}

	}
}

void GUIWindow::drawParent( )
{
			glClear( GL_DEPTH_BUFFER_BIT );
			glDepthFunc( GL_ALWAYS );
			Program* guiProgram = BaseContent->getGuiProgram( );
			guiProgram->use( );
			guiProgram->setUniformData( "projection", BaseContent->getProjectionMatrix( ) );
			guiProgram->setUniformData( "model", model );
			guiProgram->setUniformData( "color", backgroundColor );

			glBindVertexArray( WindowVao );
			glDrawArrays( GL_TRIANGLES, 0, 6 );
			glBindVertexArray( 0 );
			glDepthFunc( GL_LESS );

}


void GUIWindow::drawChilds( )
{
	for (auto iterator = inheriterList.begin( ); iterator != inheriterList.end(); ++iterator)
	{
		(*iterator)->Draw( );
	}
	
	

}

GUIBase* GUIWindow::checkEvent( Event* ev)
{
    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(ev);
	if ( mouseEvent != NULL )
	{
		float tmpXPos = mouseEvent->getXpos( );
		float tmpYPos = mouseEvent->getYpos( );
		if ( inheriterList.empty( ) )
		{
			if ( colladeWithMouse( tmpXPos, tmpYPos ) )
			{
				cout << Name << " collade (parent)" << endl;
//				cout << Name << " Width = " << Width << " Height = " << Height << " PosX = " << PosX << " PosY = " << PosY << endl;
//				cout << "Mouse X = " << tmpXPos << " Mouse Y = " << tmpYPos << endl;
				return this;
			}
			else
			{
				cout << Name << " not collade" << endl << endl;;
				return NULL;
			}

		}
		else
		{
			if ( !colladeWithMouse( tmpXPos, tmpYPos ) )
			{
				cout << Name << " not collade" << endl << endl;;
				return NULL;
			}
			/*
			else
			{
				cout << "Main window " << Name << " collade" << endl;
			}
			*/
			for ( auto inhIterator = inheriterList.rbegin( ); inhIterator != inheriterList.rend( ); ++inhIterator )
			{
				if ( (*inhIterator)->colladeWithMouse( tmpXPos, tmpYPos ) )
				{
					cout << (*inhIterator)->ReturnGUIName( ) << " collade" << endl;
				//	cout << (*inhIterator)->ReturnGUIName() << " Width = " << Width << " Height = " << Height << " PosX = " << PosX << " PosY = " << PosY << endl;
			//		cout << "Mouse X = " << tmpXPos << " Mouse Y = " << tmpYPos << endl;
					return *inhIterator;

				}
				else
				{
					cout << (*inhIterator)->ReturnGUIName( ) << " not collade" << endl << endl;
				}
			}
			cout << Name << " collade" << endl;
			return this;
			

		}
		
	}
	else
	{
		cout << "NULL" << endl;
		return NULL;
	}
	return NULL;
}

void GUIWindow::makeEvent( GUIWindow* source,GUIWindow* target,GUIManager* mg )
{
	if ( onClickHideEvent )
	{
		onClickHideEvent( target );
	}
	if ( source == target && onClickUnhideAll )
	{
		onClickUnhideAll( mg );
	}
	if ( source == target  && onClickAllEvent)
	{
		onClickAllEvent( mg );
	}
	else if ( onClickEvent )
	{
		onClickEvent( target );
	}
}

bool GUIWindow::colladeWithMouse( double xpos, double ypos )
{

	if ( xpos >= PosX && xpos <= PosX + Width )
	{
		if ( (900 - ypos) >= (PosY - Height) && 900 - ypos <= PosY )
		{
			return true;
		}
	}
	return false;
}

/*
void GUIWindow::onMouseMove( double MouseXpos, double MouseYpos )
{
	if ( colladeWithMouse( MouseXpos, MouseYpos ) )
		setBackGroundColor( 1.0, 0.0, 0.0 );
	else
		setBackGroundColor( 1.0, 1.0, 0.0 );
}
*/

void GUIWindow::Initialize( )
{


	glGenVertexArrays( 1, &WindowVao );
	glBindVertexArray( WindowVao );
	GLuint baseVbo = (BaseContent->getGUIBaseVertices( ))->getBaseVbo( );
	GLfloat* vertices = ((BaseContent->getGUIBaseVertices( ))->getVertices( ));
	GLfloat v [2 * 6];
	v [0] = vertices [0];
	v [1] = vertices [1] * Height;
	v [2] = vertices [2];
	v [3] = vertices [3];
	v [4] = vertices [4] * Width;
	v [5] = vertices [5];
	v [6] = vertices [6] * Width;
	v [7] = vertices [7];
	v [8] = vertices [8] * Width;
	v [9] = vertices [9] * Height;
	v [10] = vertices [10];
	v [11] = vertices [11] * Height;
	glGenBuffers(1, &WindowVbo);
	glBindBuffer( GL_ARRAY_BUFFER, WindowVbo );
	//glBindBuffer( GL_ARRAY_BUFFER, baseVbo );

	glBufferData( GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0 );
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );

	// Model Initialization
	model = glm::mat4( );
	model = glm::translate( model, glm::vec3( PosX, (BaseContent->getWindowSizeHeight( )) - PosY, 0.0 ) );

	isInitialized = true;
}



GUIWindow::~GUIWindow( )
{
	cout << "Delete Window" << endl;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


GUIBaseVertices::GUIBaseVertices( )
{
	vertices = new GLfloat [2 * 6];
	vertices [0] = 0.0;
	vertices [1] = 1.0;

	vertices [2] = 0.0;
	vertices [3] = 0.0;

	vertices [4] = 1.0;
	vertices [5] = 0.0;

	vertices [6] = 1.0;
	vertices [7] = 0.0;

	vertices [8] = 1.0;
	vertices [9] = 1.0;

	vertices [10] = 0.0;

	vertices [11] = 1.0;
	glGenBuffers( 1, &BaseVertexVbo );
	//glBindBuffer( GL_ARRAY_BUFFER, BaseVertexVbo );
	//glBufferData( GL_ARRAY_BUFFER, 2 * 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW );

}

GLuint GUIBaseVertices::getBaseVbo( void )
{
	return BaseVertexVbo;
}

GLfloat* GUIBaseVertices::getVertices( void )
{
	return vertices;
}

GUIBaseVertices::~GUIBaseVertices( )
{
	delete [ ] vertices;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GUIBaseContent::GUIBaseContent( float WindowWidth, float WindowHeight, Program* pr ) : BaseVertices( )
{
	WindowSizeWidth = WindowWidth;
	WindowSizeHeight = WindowHeight;
	projectionMatrix = glm::ortho( 0.0f, (float) (WindowSizeWidth), (float) (WindowSizeHeight), 0.0f, 1.0f, -1.0f );
	GuiProgram = pr;


}

void GUIBaseContent::updateWindowSize(float _Width, float _Height)
{
	WindowSizeHeight = _Height;
	WindowSizeWidth = _Width;
}

Program* GUIBaseContent::getGuiProgram( void )
{
	return GuiProgram;
}

glm::mat4 GUIBaseContent::getProjectionMatrix( void )
{
	return projectionMatrix;
}

float GUIBaseContent::getWindowSizeWidth( )
{
	return WindowSizeWidth;
}

float GUIBaseContent::getWindowSizeHeight( )
{
	return WindowSizeHeight;
}

GUIBaseVertices* GUIBaseContent::getGUIBaseVertices( void )
{
	return &BaseVertices;
}

GUIBaseContent::~GUIBaseContent( )
{

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GUIManager::addElement( GUIBase* el )
{
	listGuiElements.push_back( el );
	listIterator = listGuiElements.begin( );

}

void GUIManager::makeFirst( )
{
	listIterator = listGuiElements.begin( );
}

GUIBase* GUIManager::getElement(void)
{
		return *listIterator;
}

bool GUIManager::isEnd(void)
{
	if ( listIterator == listGuiElements.end( ) )
		return true;
	else
		return false;
}

void GUIManager::nextElement( )
{
	listIterator++;
}

void GUIManager::performEvent()
{
	Event* FirstEventInQueue = eventQueue->getNextEvent( );
	/*
	if ( dynamic_cast<MouseEvent*>(FirstEventInQueue) !=NULL )
	{
		cout << "MOUSE" << endl;
	}
	if ( dynamic_cast<KeyboardEvent*>(FirstEventInQueue) != NULL )
	{
		cout << "KEYBOARD" << endl;
	}
	*/
	for ( auto iter = listGuiElements.begin( ); iter != listGuiElements.end( ); ++iter )
	{
		GUIBase* tmp = (*iter)->checkEvent( FirstEventInQueue );
		if(tmp!=NULL)
		{
			cout << " MY name is " << tmp->ReturnGUIName( ) << endl;
//			(*iter)->makeEvent( FirstEventInQueue );
			GUIWindow* sourceWindow = dynamic_cast<GUIWindow*>(tmp);
			GUIWindow* tmpWindow = dynamic_cast<GUIWindow*>(*iter);
			tmp->makeEvent(sourceWindow,tmpWindow,this);
			break;
		}
		
	}

	cout << endl;
	cout << endl;
	cout << endl;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void EventQueue::printEventQueue( void )
{
	int i = 0;
	while ( !eventQueue.empty( ) )
	{
		cout << " i  - " << eventQueue.front( ) << endl;
		++i;
		eventQueue.pop( );
	}

}

