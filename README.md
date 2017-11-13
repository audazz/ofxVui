# ✨ View + UI System for openFrameworks ✨
###### *TELEPORT*
* **<a href='#ofxvui' class='anchor'>ofxVui Features</a>**
* **<a href='#code-examples' class='anchor'>Code Examples</a>**
  * **<a href='#view-manager-1' class='anchor'>View Manager</a>**
  * **<a href='#ui-animation' class='anchor'>UI Animation</a>**
  * **<a href='#ui-events' class='anchor'>UI Events</a>**
  * **<a href='#ui-styling' class='anchor'>UI Styling</a>**

***
###### *[DEPENDENCIES](#dependencies)*
* **ofxEasing** » https://github.com/arturoc/ofxEasing
* **ofxWinTouchHook** » https://github.com/trentbrooks/ofxWinTouchHook
  * OPTIONAL (Windows only)
    1. add Preprocessor Macro **USING_ofxWinTouchHook**
    2. call **VUI::EnableTouch()** in ofApp::setup()
***
###### *[WORKS / TESTED ON](#tested-on)*
* ***openFrameworks 0.9.8***
  * macOS High Sierra (10.13) » Xcode 9
  * Windows 10 » Visual Studio 2017 
***
###### *[QUICK NOTE](#quick-note)*

So everything works! 🎉 Although, the code under the hood is a little bit of spaghetti at the moment, uncommented, and there's a lot of legacy/debug stuff 😅. I was originally working on two different addons but it made more sense to combine them. Cleaning up the code is on my TODO list but first wanted to make this addon usable, tested, and make enough project/code examples for people to learn from. I have to get back to other projects though so yea... please feel free to enjoy this as-is 😋 and I'll continue to make improvements whenever I get the chance 😊
***
###### *[TODO](#todo)* 

```
  • Cleanup example-TouchEvents
      
  • Touch
      + Test multiple touch events / scenarios
      + Add - VUI_EVENT_TOUCH_DRAG
      
  • UI Elements
      + Add - Slider
      + Add - Slider2D
      
  • StyleSheet
      + width/height - percentage based values
      + width/height - calc() based values
      
  • VUI::SetRotation()
      + Add option - VUI_ROTATE_90_CW
      + Add option - VUI_ROTATE_180
      
  • Cleanup / add comments to ofxVui code
```
If you think of a feature that would be super useful in this addon or have a request to prioritize a feature, please feel free to email me  » ***hi.christophermiles@gmail.com*** -or- submit a pull request :)

***
# **[ofxVui](#ofxvui)**
### View Manager
*VUI Mouse/Touch Events still work when scaling/rotating view* 👍🏻
```
Resolution
Scale
Rotation
  • VUI_ROTATE_90_CCW
```
  
  
### UI System
```
Elements
  • Element
  • Element->MakeToggle()
  • ToggleGroup
  • Text
  • TextField
    
Animation Tween
  • x
  • y
  • width
  • height
  • scale
  • opacity
  • rotation
  • ease/easing
  
Animation Events
  • ANIMATE_START
  • ANIMATE_STEP
  • ANIMATE_COMPLETE
    
Mouse Events
  • MOUSE_OVER
  • MOUSE_OUT
  • MOUSE_PRESSED
  • MOUSE_MOVED
  • MOUSE_DRAGGED
  • MOUSE_RELEASED
  • MOUSE_CLICK
  • MOUSE_DOUBLE_CLICK
    
Touch Events (only single touch usage tested so far)
  • TOUCH_DOWN
  • TOUCH_UP
  • TOUCH_TAP
  • TOUCH_DOUBLE_TAP
    
Other Events
  • STATE_CHANGE
  • TEXT_CHANGE <Text/TextField>
  • VALUE_CHANGE <Element->MakeToggle()>
  • TOGGLE_CHANGE <ToggleGroup>
  • SUBMIT <TextField>
  • FOCUS <TextField>
  • UNFOCUS <TextField>
    
UI States
  • STATE_UP
  • STATE_OVER
  • STATE_DOWN
  
UI StyleSheet  
  • Manages fonts/image assets so there's no redundant loading
  
UI StyleSheet: Styles
  • width
  • height
  • scale
  • opacity
  • rotation
  • backgroundImage
  • backgroundColor
  • anchorPoint
  • offset
  • color <Text/TextField>
  • padding <Text/TextField>
  • textAlign <Text/TextField>
  • font <Text/TextField>
```
***
# **[Code Examples](#code-examples)**
There are a bunch of example projects/code in the repo but here are some basics.

# [UI Animation](#ui-animation)
*For more info checkout the **example-Animation** project/code*
* *Animate( Float seconds, String params );*

```c++
// basic
elementPtr->Animate(1.25, "{x: 160, width: 150, height: 50, scale: 1.75, rotation: -135, opacity: .75, ease: Elastic.easeOut }");

// add listener - method A
elementPtr->Animate(1.25, "{x: 160, width: 150, height: 50, scale: 1.75, rotation: -135, opacity: .75, ease: Elastic.easeOut }", this, &ListenerClass::vuiEventHandler);

// add listener - method B
Tween* tweenPtr = elementPtr->Animate(1.25, "{x: 160, width: 150, height: 50, scale: 1.75, rotation: -135, opacity: .75, ease: Elastic.easeOut }");
ofAddListener( tweenPtr->onStep, this, &ListenerClass::vuiEventHandler );
ofAddListener( tweenPtr->onComplete, this, &ListenerClass::vuiEventHandler );

// eventHandler
ListenerClass::vuiEventHandler(vuiEventArgs& evt){
  if ( evt.element == elementPtr ){
    if ( evt.eventType == VUI_EVENT_ANIMATE_COMPLETE ) /* do stuff */;
    else if ( evt.eventType == VUI_EVENT_ANIMATE_STEP ){
      ofLog() << evt.tween->GetProgress();
    }
  }
}

/* -------------------------------------------- easing functions

  Back.easeIn
  Back.easeOut
  Back.easeInOut
  Bounce.easeIn
  Bounce.easeOut
  Bounce.easeInOut
  Circ.easeIn
  Circ.easeOut
  Circ.easeInOut
  Cubic.easeIn
  Cubic.easeOut
  Cubic.easeInOut
  Elastic.easeIn
  Elastic.easeOut
  Elastic.easeInOut
  Exp.easeIn
  Exp.easeOut
  Exp.easeInOut
  Linear.easeIn
  Linear.easeOut
  Linear.easeInOut
  Linear.easeNone
  Quad.easeIn
  Quad.easeOut
  Quad.easeInOut
  
*/
  
```

# [UI Events](#ui-events)
*For more info checkout the **example-Events** project/code*

```c++
/*
  Some Mouse/Touch events are interchangeable so if you
  use VUI::EnableTouch() you don't have to change your code:
    - onMousePressed / onTouchDown
    - onMouseReleased / onTouchUp
    - onMouseClick / onTouchTap
    - onMouseDoubleClick / onTouchDoubleTap
*/

// add listener
ofAddListener( elementPtr->onMouseClick, this, &ListenerClass::vuiEventHandler );

// eventHandler
ListenerClass::vuiEventHandler(vuiEventArgs& evt){
  if ( evt.element == elementPtr ){
    if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ) {
      ofLog() << evt.localMousePos;
    };
  }
}

/* -------------------------------------------- vuiEventArgs
  
  Element* element;
  int eventType;
  int renderState;
  int virtualState;

  int value = -1;
  string text = "";

  ofVec2f localMousePos;
  ofVec2f localDragDelta;
  ofVec2f localDragStart;

  ofVec2f globalMousePos;

  Tween* tween;

*/

/* -------------------------------------------- events

  VUI_EVENT_MOUSE_OVER
  VUI_EVENT_MOUSE_OUT
  VUI_EVENT_MOUSE_PRESSED
  VUI_EVENT_MOUSE_MOVED
  VUI_EVENT_MOUSE_DRAGGED
  VUI_EVENT_MOUSE_RELEASED
  VUI_EVENT_MOUSE_CLICK
  VUI_EVENT_MOUSE_DOUBLE_CLICK

  VUI_EVENT_TOUCH_DOWN
  VUI_EVENT_TOUCH_UP
  VUI_EVENT_TOUCH_TAP
  VUI_EVENT_TOUCH_DOUBLE_TAP

  VUI_EVENT_STATE_CHANGE
  VUI_EVENT_TEXT_CHANGE
  VUI_EVENT_VALUE_CHANGE
  VUI_EVENT_TOGGLE_CHANGE
  VUI_EVENT_SUBMIT

  VUI_EVENT_FOCUS
  VUI_EVENT_UNFOCUS

  VUI_EVENT_ANIMATE_COMPLETE
  VUI_EVENT_ANIMATE_STEP
  VUI_EVENT_ANIMATE_START
  
*/

```

# [UI Styling](#ui-styling)
*For more info checkout the **example-StylingElements** project/code*
```c++
StyleSheet* ss;
Element* buttonA;
Element* buttonB;
Text* label;

// --------------------------------------------
void setup(){
  string styles = R"(
    [Images>
      btn-up: path/to/image.jpg;
      btn-over: path/to/image-over.jpg;
      btn-down: path/to/image-down.jpg;
    ]
    
    [.button>
      width: 120;
      height: 36;
      bgImage: btn-up, FIT;
      
      &:over{
        bgImage: btn-over, FIT;
      }
      
      &:down{
        bgImage: btn-down, FIT;
      }
    ]
    
    [#buttonB>
      scale: 1.25;
      opacity: .9;
    ]
    
    [.text>
      font: path/to/fontfile.ttf,16;
      color: #dddddd;
      padding: 10;
      bg: #ffffff;
    ]
  )";

  ss = new StyleSheet( styles );
  
  /*
  
    name stylesheet if you want to load/use it elsewhere
    
    ss = new StyleSheet( styles, "dope-stylesheet" );
    StyleSheet *elsewhere = VUI::GetStyleSheet( "dope-stylesheet" );
    
  */

  label = new Text( 20, 20, ss, ".text" );
  // true = automatically set width/height to text
  label->SetText( "Stuff", true );
  
  buttonA = new Element( 20, 40, ss, ".button" );
  buttonB = new Element( 20, 80, ss, ".button", "#buttonB" );
  
}

// --------------------------------------------
void draw(){
  label->Render();
  buttonA->Render();
  buttonB->Render();
}


```

# [View Manager](#example-view-manager)
*For more info checkout the **example-ViewManager** project/code*

```c++
// ---------------------------------------- main.cpp
VUI::SetResolution(1080,1920,.6);
// VUI::RotateView( VUI_ROTATE_90_CCW );
ofSetupOpenGL(VUI::GetWindowWidth(),VUI::GetWindowHeight(),OF_WINDOW);


// ---------------------------------------- create View
#include "ofxVui.h"
using namespace VUI;

class MyView : public View {
    // a few extra stuff
    /*
       if overriding BeforeExitView(), you must call ExitView()
       when you're done with what you need to do
     */
    virtual void BeforeExitView() {
      ExitView();
    };
    
    virtual void OnEnterView() {};
    
    // basic stuff
    int mouseX;
    int mouseY;
    
    void setup(){};
    void update(){};
    void draw(){};

    void keyPressed(int key){};
    void keyReleased(int key){};
    void mouseMoved(int x, int y ){};
    void mouseDragged(int x, int y, int button){};
    void mousePressed(int x, int y, int button){};
    void mouseReleased(int x, int y, int button){};
    void mouseEntered(int x, int y){};
    void mouseExited(int x, int y){};
    void windowResized(int w, int h){};
    void dragEvent(ofDragInfo dragInfo){};
    void gotMessage(ofMessage msg){};
    
}


// ---------------------------------------- ofApp.cpp
#include "MyView.h"

void setup(){
  //VUI::EnableTouch();
  
  VUI::AddView( "view-name", new MyView() );
  //VUI::AddView( "view-exampleA", new JustAnExample() );
  //VUI::AddView( "view-exampleB", new JustAnExampleB() );
  
  // can call from anywhere
  VUI::SetView( "view-name" );
}

void draw(){
  VUI::Render();
}

```


